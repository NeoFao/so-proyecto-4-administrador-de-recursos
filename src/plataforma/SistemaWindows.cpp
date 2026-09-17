// ============================================================================
// SistemaWindows.cpp -- Implementacion de Sistema.h para Windows.
//
// Es UNO de los dos ficheros que el enunciado permite que sean especificos de
// plataforma. El sistema de compilacion elige este o SistemaLinux.cpp; nunca
// los dos. El resto del proyecto no sabe cual se compilo.
//
//   Memoria  -> GlobalMemoryStatusEx, de la API de Windows
//   Procesos -> se ejecuta `tasklist` y se parsea su salida
//
// POR QUE tasklist Y NO LA API DE PROCESOS: el enunciado nombra `ps`/`tasklist`
// explicitamente como via valida. Parsear la salida de un comando mantiene el
// modulo pequeno y hace que el puerto a Linux sea casi literal --cambia el
// comando y el formato, no la estructura--. Ademas ejercita de verdad el caso
// de error "comando no disponible" que pide el requisito no funcional 1.
// ============================================================================

#include "plataforma/Sistema.h"

#include <array>
#include <cstdio>
#include <sstream>
#include <stdexcept>

// windows.h SOLO puede aparecer en este directorio. Se comprueba con un grep
// sobre src/core/ y src/io/, y la salida vacia es la evidencia.
#include <windows.h>

namespace rec {
namespace {

// Ejecuta un comando y devuelve su salida estandar completa.
//
// _popen es la version de Microsoft de popen; el guion bajo inicial es la
// convencion de MSVC para funciones POSIX no estandar. Se usa el modo "r"
// (lectura) porque solo interesa lo que el comando imprime.
std::string ejecutarComando(const std::string& comando) {
    // El pipe se cierra con _pclose SIEMPRE, incluso si algo lanza en medio:
    // por eso la lectura va dentro de un try. Sin esto, una excepcion dejaria
    // el descriptor abierto y el proceso hijo zombi.
    FILE* tuberia = _popen(comando.c_str(), "r");
    if (tuberia == nullptr) {
        throw std::runtime_error(
            "no se pudo ejecutar '" + comando +
            "'. Es el caso 'comando no disponible': compruebe que la"
            " herramienta del sistema existe y esta en el PATH");
    }

    std::string salida;
    try {
        std::array<char, 512> buffer{};
        while (std::fgets(buffer.data(), static_cast<int>(buffer.size()), tuberia) != nullptr) {
            salida += buffer.data();
        }
    } catch (...) {
        _pclose(tuberia);
        throw;
    }

    const int codigo = _pclose(tuberia);
    if (codigo != 0) {
        throw std::runtime_error(
            "'" + comando + "' termino con codigo " + std::to_string(codigo) +
            ". Puede ser falta de permisos o que la herramienta no este"
            " disponible en este sistema");
    }
    return salida;
}

// Quita comillas envolventes y espacios de un campo CSV.
std::string limpiarCampo(std::string campo) {
    while (!campo.empty() && (campo.front() == ' ' || campo.front() == '"')) campo.erase(campo.begin());
    while (!campo.empty() && (campo.back() == ' ' || campo.back() == '"' ||
                              campo.back() == '\r' || campo.back() == '\n')) {
        campo.pop_back();
    }
    return campo;
}

// Convierte el campo de memoria de tasklist a kilobytes.
//
// tasklist lo entrega como "12.345 KB" o "12,345 K" segun el idioma de
// Windows: el separador de miles cambia con la configuracion regional. Por eso
// se descartan TODOS los caracteres que no son digitos en vez de intentar
// interpretar el separador. Si se asumiera la coma, en un Windows en espanol
// el numero saldria mal y nadie se enteraria.
unsigned long long memoriaAKb(const std::string& campo) {
    std::string digitos;
    for (char c : campo) {
        if (c >= '0' && c <= '9') digitos += c;
    }
    if (digitos.empty()) return 0;
    try {
        return std::stoull(digitos);
    } catch (const std::exception&) {
        return 0;
    }
}

// Parte una linea CSV respetando las comillas.
std::vector<std::string> partirCsv(const std::string& linea) {
    std::vector<std::string> campos;
    std::string actual;
    bool dentroDeComillas = false;

    for (char c : linea) {
        if (c == '"') {
            dentroDeComillas = !dentroDeComillas;
            actual += c;
        } else if (c == ',' && !dentroDeComillas) {
            campos.push_back(limpiarCampo(actual));
            actual.clear();
        } else {
            actual += c;
        }
    }
    if (!actual.empty()) campos.push_back(limpiarCampo(actual));
    return campos;
}

} // namespace

std::string nombrePlataforma() { return "Windows"; }

unsigned long pidPropio() { return GetCurrentProcessId(); }

// /FO CSV  -> formato de salida separado por comas, estable entre idiomas
// /NH      -> sin fila de encabezado, para no tener que saltarla ni depender
//             de como se traduzcan los titulos de columna
std::string comandoDeProcesos() { return "tasklist /FO CSV /NH"; }

InfoMemoria consultarMemoria() {
    MEMORYSTATUSEX estado;
    // La API exige que el campo dwLength venga relleno con el tamano de la
    // estructura ANTES de llamar: asi sabe que version del struct le pasan.
    // Olvidarlo es el error clasico con esta funcion y hace que falle sin
    // explicacion.
    estado.dwLength = sizeof(estado);

    if (GlobalMemoryStatusEx(&estado) == 0) {
        throw std::runtime_error(
            "GlobalMemoryStatusEx fallo con codigo " + std::to_string(GetLastError()) +
            "; no se pudo consultar la memoria del sistema");
    }

    InfoMemoria info;
    info.totalKb = estado.ullTotalPhys / 1024ULL;
    info.disponibleKb = estado.ullAvailPhys / 1024ULL;
    return info;
}

std::vector<ProcesoInfo> listarProcesos() { return listarProcesosCon(comandoDeProcesos()); }

std::vector<ProcesoInfo> listarProcesosCon(const std::string& comando) {
    const std::string salida = ejecutarComando(comando);

    std::vector<ProcesoInfo> procesos;
    std::istringstream flujo(salida);
    std::string linea;

    while (std::getline(flujo, linea)) {
        if (linea.empty()) continue;

        // Formato de tasklist /FO CSV /NH:
        //   "nombre.exe","PID","sesion","num","memoria"
        const std::vector<std::string> campos = partirCsv(linea);
        if (campos.size() < 5) continue;   // linea incompleta: se descarta

        ProcesoInfo p;
        p.nombre = campos[0];
        try {
            p.pid = std::stoul(campos[1]);
        } catch (const std::exception&) {
            continue;  // sin PID valido la fila no sirve
        }
        p.memoriaKb = memoriaAKb(campos[4]);
        procesos.push_back(p);
    }

    if (procesos.empty()) {
        throw std::runtime_error(
            "'" + comando +
            "' no devolvio ningun proceso. Puede ser falta de permisos para"
            " consultar la lista de procesos del sistema");
    }
    return procesos;
}

} // namespace rec
