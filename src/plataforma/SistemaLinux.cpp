// ============================================================================
// SistemaLinux.cpp -- Implementacion de Sistema.h para Linux.
//
// Es el gemelo de SistemaWindows.cpp: la MISMA interfaz, resuelta con las
// herramientas del otro sistema.
//
//   Memoria  -> se lee /proc/meminfo
//   Procesos -> se ejecuta `ps` y se parsea su salida
//
// ESTE FICHERO SE COMPILA Y SE EJECUTA DE VERDAD. No es una rama declarada y
// no probada: scripts/verificar_linux.ps1 usa una distribucion Linux de WSL2
// (Ubuntu), compila este codigo con g++ y corre la suite completa dentro. La
// evidencia queda en results/compatibilidad_linux.txt. Dockerfile.linux
// ofrece la misma verificacion en un contenedor Debian, como alternativa.
//
// POR QUE /proc/meminfo Y NO sysinfo(): sysinfo() es una llamada al sistema y
// seria mas directa, pero devuelve "memoria libre", que NO es lo mismo que
// "memoria disponible". Linux usa como cache toda la RAM que sobra, asi que
// `free` suele ser bajisimo aunque el sistema este holgado. MemAvailable, que
// existe en /proc/meminfo desde el kernel 3.14, es la estimacion del propio
// kernel de cuanto puede pedir una aplicacion sin provocar swap: es el numero
// honesto y es el que reporta `free -h` en la columna "available".
// ============================================================================

#include "plataforma/Sistema.h"

#include <array>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <unistd.h>   // getpid

namespace rec {
namespace {

// Ejecuta un comando y devuelve su salida estandar completa.
std::string ejecutarComando(const std::string& comando) {
    FILE* tuberia = popen(comando.c_str(), "r");
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
        pclose(tuberia);
        throw;
    }

    const int codigo = pclose(tuberia);
    if (codigo != 0) {
        throw std::runtime_error(
            "'" + comando + "' termino con codigo " + std::to_string(codigo) +
            ". Puede ser falta de permisos o que la herramienta no este"
            " disponible en este sistema");
    }
    return salida;
}

// Devuelve el valor en kB de una clave de /proc/meminfo.
//
// El formato de cada linea es:  "MemTotal:        8072404 kB"
// Se busca la clave al principio de la linea y se toma el numero.
unsigned long long leerClaveMeminfo(const std::string& contenido, const std::string& clave) {
    std::istringstream flujo(contenido);
    std::string linea;

    while (std::getline(flujo, linea)) {
        if (linea.rfind(clave, 0) != 0) continue;   // no empieza por la clave

        std::istringstream partes(linea);
        std::string etiqueta;
        unsigned long long valor = 0;
        partes >> etiqueta >> valor;   // "MemTotal:"  8072404
        return valor;
    }
    return 0;
}

} // namespace

std::string nombrePlataforma() { return "Linux"; }

unsigned long pidPropio() { return static_cast<unsigned long>(getpid()); }

// -o pid=,comm=,rss=   pide exactamente tres columnas SIN encabezado (el '='
//                      vacia el titulo), asi no hay que saltar una cabecera
//                      cuyo texto depende del idioma.
// -e                   todos los procesos del sistema.
// rss                  Resident Set Size, en kilobytes: la memoria fisica que
//                      el proceso ocupa de verdad. Es el equivalente mas
//                      cercano a lo que reporta tasklist en Windows.
std::string comandoDeProcesos() { return "ps -e -o pid=,comm=,rss="; }

InfoMemoria consultarMemoria() {
    std::ifstream archivo("/proc/meminfo");
    if (!archivo) {
        throw std::runtime_error(
            "no se pudo abrir /proc/meminfo. En Linux es la fuente de las"
            " metricas de memoria; su ausencia indica un sistema de ficheros"
            " proc no montado");
    }

    std::ostringstream buffer;
    buffer << archivo.rdbuf();
    const std::string contenido = buffer.str();

    InfoMemoria info;
    info.totalKb = leerClaveMeminfo(contenido, "MemTotal:");
    info.disponibleKb = leerClaveMeminfo(contenido, "MemAvailable:");

    if (info.totalKb == 0) {
        throw std::runtime_error(
            "/proc/meminfo no contiene MemTotal; no se pudo determinar la"
            " memoria del sistema");
    }

    // MemAvailable no existe en kernels anteriores al 3.14. En ese caso se cae
    // a MemFree, que subestima lo disponible --no cuenta la cache reclamable--
    // pero es preferible a reportar cero. Se prefiere un numero conservador y
    // explicable a uno inventado.
    if (info.disponibleKb == 0) {
        info.disponibleKb = leerClaveMeminfo(contenido, "MemFree:");
    }
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

        // Formato: "<pid> <nombre> <rss>", con relleno de espacios.
        //
        // El nombre PUEDE contener espacios (Firefox lanza procesos llamados
        // "Web Content"), asi que no se puede leer con tres >> seguidos: el
        // tercero intentaria convertir "Content" en numero y la fila se
        // descartaria en silencio. Se toma el PID del principio, la memoria
        // del final, y el nombre es todo lo que queda en medio.
        const std::size_t ini = linea.find_first_not_of(" \t");
        const std::size_t finPid = linea.find_first_of(" \t", ini);
        const std::size_t fin = linea.find_last_not_of(" \t\r");
        if (ini == std::string::npos || finPid == std::string::npos ||
            fin == std::string::npos || fin <= finPid) {
            continue;
        }
        const std::size_t iniRss = linea.find_last_of(" \t", fin);
        if (iniRss == std::string::npos || iniRss <= finPid) continue;

        ProcesoInfo p;
        try {
            p.pid = std::stoul(linea.substr(ini, finPid - ini));
            p.memoriaKb = std::stoull(linea.substr(iniRss + 1, fin - iniRss));
        } catch (const std::exception&) {
            continue;  // fila sin numeros validos: se descarta
        }

        const std::size_t iniNombre = linea.find_first_not_of(" \t", finPid);
        const std::size_t finNombre = linea.find_last_not_of(" \t", iniRss);
        if (iniNombre == std::string::npos || finNombre == std::string::npos ||
            finNombre < iniNombre) {
            continue;
        }
        p.nombre = linea.substr(iniNombre, finNombre - iniNombre + 1);
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
