#ifndef PLATAFORMA_SISTEMA_H
#define PLATAFORMA_SISTEMA_H

#include <string>
#include <vector>

// Consultas al sistema operativo anfitrion: memoria y procesos.
//
// ============================================================================
// LA DECISION DE DISENO CENTRAL DE ESTE PROYECTO
// ============================================================================
//
// Este encabezado NO incluye <windows.h> ni nada especifico de un sistema.
// Declara QUE se puede preguntar al sistema, no COMO se le pregunta.
//
// El "como" vive en dos ficheros hermanos que implementan esta misma interfaz:
//
//     SistemaWindows.cpp   -> tasklist  +  GlobalMemoryStatusEx
//     SistemaLinux.cpp     -> ps        +  /proc/meminfo
//
// El sistema de compilacion elige UNO de los dos. El resto del programa --el
// menu, los reportes, las metricas-- no sabe en cual esta corriendo.
//
// POR QUE IMPORTA: la pregunta de defensa publicada para este proyecto es
// "si tuvieras que portar tu herramienta de Windows a Linux, que partes
// especificas tendrias que reescribir?". Con esta separacion la respuesta no
// es una estimacion, es una lista de ficheros: los de src/plataforma/, y nada
// mas. Se comprueba con un grep de <windows.h> sobre el resto del proyecto,
// cuya salida vacia es la evidencia.
namespace rec {

// --- Memoria del sistema (RF-3) --------------------------------------------

// Se reporta en kilobytes y no en bytes porque es la unidad en que lo entrega
// /proc/meminfo en Linux; convertir a bytes y volver a dividir para mostrar
// solo anadiria redondeos sin ganar precision.
struct InfoMemoria {
    unsigned long long totalKb = 0;
    unsigned long long disponibleKb = 0;

    unsigned long long usadaKb() const {
        // Resta protegida: si por alguna razon el sistema reportara mas
        // disponible que total, devolver 0 es preferible a un numero enorme
        // por desbordamiento (son enteros SIN signo: 5 - 10 no da -5, da un
        // numero astronomico).
        return disponibleKb > totalKb ? 0 : totalKb - disponibleKb;
    }

    double porcentajeUsado() const {
        if (totalKb == 0) return 0.0;
        return 100.0 * static_cast<double>(usadaKb()) / static_cast<double>(totalKb);
    }
};

// LANZA std::runtime_error si el sistema no puede responder.
InfoMemoria consultarMemoria();

// --- Procesos del sistema (RF-2) -------------------------------------------

struct ProcesoInfo {
    unsigned long pid = 0;
    std::string nombre;
    unsigned long long memoriaKb = 0;   // consumo aproximado
};

// Lista los procesos activos del anfitrion.
//
// LANZA std::runtime_error si el comando del sistema no esta disponible o
// devuelve un codigo de error. Ese caso es uno de los tres que el requisito no
// funcional 1 pide manejar explicitamente ("comando no disponible"), y por eso
// se prueba en la suite forzando un comando inexistente.
std::vector<ProcesoInfo> listarProcesos();

// Variante que permite indicar OTRO comando. Existe por una razon concreta y
// no por generalidad: el requisito no funcional 1 pide manejar el caso
// "comando no disponible", y sin esta costura ese caso no se podria PROBAR,
// solo afirmar. La suite la llama con un comando inexistente y comprueba que
// se lanza una excepcion con un mensaje util.
//
// `listarProcesos()` es exactamente esta funcion con comandoDeProcesos().
std::vector<ProcesoInfo> listarProcesosCon(const std::string& comando);

// --- Identificacion de la plataforma ---------------------------------------

// Nombre legible de la plataforma para la que se compilo ("Windows" o
// "Linux"). Se usa en la cabecera de los reportes para que la evidencia diga
// SIEMPRE en que sistema se genero, sin depender de que alguien lo anote.
std::string nombrePlataforma();

// PID del proceso que ejecuta esta herramienta.
//
// Se necesita para el resultado experimental 3 (consumo de la propia
// herramienta): con el PID se busca la fila correspondiente en la lista de
// procesos que ya se sabe obtener, sin escribir ningun mecanismo nuevo de
// medicion. Es la via mas barata y ademas mide lo mismo que se mide para los
// demas procesos, asi que las cifras son comparables entre si.
unsigned long pidPropio();

// Comando concreto que este binario usa para listar procesos. Se expone para
// poder mostrarlo en el reporte y en los mensajes de error: si falla, el
// usuario ve exactamente que se intento ejecutar.
std::string comandoDeProcesos();

} // namespace rec
#endif
