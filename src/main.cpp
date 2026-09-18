// ============================================================================
// main.cpp -- Administrador Simplificado de Recursos (Proyecto 4)
// Universidad Invenio, TIIT2007 Sistemas Operativos
// Autor: Fabrizio Espinoza Arce
//
// Orquesta. No consulta al sistema operativo por su cuenta: para eso esta
// src/plataforma/. No formatea tablas: para eso esta src/io/.
//
// DOS MODOS DE USO, Y POR QUE HACEN FALTA LOS DOS
//
//   Sin argumentos  -> menu interactivo. Es el requisito funcional 4.
//   Con argumentos  -> una sola accion y salir. Lo necesitan los scripts de
//                      experimentos y las demostraciones, que no pueden
//                      teclear en un menu.
//
// El menu no es un envoltorio del modo de argumentos ni al reves: los dos
// llaman a los mismos modulos. Cambiar una funcionalidad la cambia en ambos.
// ============================================================================

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "core/Cronometro.h"
#include "core/Medicion.h"
#include "io/Menu.h"
#include "io/ReporteConsola.h"
#include "plataforma/Archivos.h"
#include "plataforma/Sistema.h"

namespace {

struct Opciones {
    std::string directorio = "trabajo";
    std::string accion;             // vacio = menu interactivo
    std::size_t cantidad = 100;     // para --benchmark
    int repeticiones = 5;
    std::string archivoCsv;
    std::size_t topProcesos = 15;   // para --procesos
    bool todosLosProcesos = false;
    bool mostrarAyuda = false;
};

const char* AYUDA =
    "Administrador Simplificado de Recursos -- Proyecto 4\n"
    "\n"
    "USO\n"
    "  recursos [opciones]\n"
    "\n"
    "  Sin opciones abre el MENU INTERACTIVO, que es la interfaz principal\n"
    "  (requisito 4). Las opciones sirven para scripts y demostraciones.\n"
    "\n"
    "ACCIONES DIRECTAS (una y sale)\n"
    "  --archivos            lista los archivos del directorio de trabajo\n"
    "  --procesos            lista los procesos activos del sistema\n"
    "  --memoria             muestra memoria total, usada y disponible\n"
    "  --autoconsumo         consumo de recursos de esta misma herramienta\n"
    "  --benchmark           mide los tiempos de las operaciones de archivo\n"
    "\n"
    "OPCIONES\n"
    "  --dir RUTA            directorio de trabajo (por omision: trabajo)\n"
    "  --top N               procesos a mostrar con --procesos (por omision: 15)\n"
    "  --todos               con --procesos, los muestra todos sin recortar\n"
    "  --cantidad N          archivos a usar en --benchmark (por omision: 100)\n"
    "  --repeticiones N      repeticiones de cada medicion (por omision: 5)\n"
    "  --csv RUTA            escribe los resultados de --benchmark en CSV\n"
    "  -h, --help            muestra esta ayuda\n"
    "\n"
    "EJEMPLOS\n"
    "  recursos\n"
    "      Abre el menu interactivo.\n"
    "\n"
    "  recursos --benchmark --cantidad 1000 --repeticiones 5\n"
    "      Mide crear, listar, metadatos y eliminar con 1000 archivos.\n"
    "\n"
    "CODIGOS DE SALIDA\n"
    "  0  todo correcto\n"
    "  1  error de ejecucion (permisos, comando no disponible, ruta invalida)\n"
    "  2  error de uso (opcion desconocida o valor invalido)\n";

// Interpreta argv. Devuelve false si hay un error de uso.
bool interpretar(int argc, char** argv, Opciones& op, std::string& error) {
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        auto valorSiguiente = [&](std::string& destino) {
            if (i + 1 >= argc) {
                error = "la opcion " + arg + " necesita un valor";
                return false;
            }
            destino = argv[++i];
            return true;
        };

        if (arg == "-h" || arg == "--help") {
            op.mostrarAyuda = true;
            return true;
        } else if (arg == "--archivos" || arg == "--procesos" || arg == "--memoria" ||
                   arg == "--autoconsumo" || arg == "--benchmark") {
            op.accion = arg.substr(2);
        } else if (arg == "--dir") {
            if (!valorSiguiente(op.directorio)) return false;
        } else if (arg == "--csv") {
            if (!valorSiguiente(op.archivoCsv)) return false;
        } else if (arg == "--todos") {
            op.todosLosProcesos = true;
        } else if (arg == "--cantidad" || arg == "--repeticiones" || arg == "--top") {
            std::string valor;
            if (!valorSiguiente(valor)) return false;
            long numero = 0;
            std::size_t consumidos = 0;
            try {
                numero = std::stol(valor, &consumidos);
            } catch (const std::exception&) {
                consumidos = 0;
            }
            // stol acepta un prefijo numerico y descarta el resto en silencio:
            // "5abc" daria 5. Se exige que el valor entero sea el numero.
            if (consumidos == 0 || consumidos != valor.size()) {
                error = "'" + valor + "' no es un numero entero";
                return false;
            }
            if (numero <= 0) {
                error = arg + " debe ser mayor que cero (se recibio " + valor + ")";
                return false;
            }
            if (arg == "--cantidad") {
                op.cantidad = static_cast<std::size_t>(numero);
            } else if (arg == "--top") {
                op.topProcesos = static_cast<std::size_t>(numero);
            } else {
                op.repeticiones = static_cast<int>(numero);
            }
        } else {
            error = "opcion no reconocida: '" + arg + "'";
            return false;
        }
    }
    return true;
}

// --- Resultado experimental 1: tiempos de las operaciones de archivo --------
//
// Se cronometran las cuatro operaciones sobre el mismo lote de archivos, y
// cada una se repite `repeticiones` veces para poder dar media y desviacion.
// Medir una sola vez daria ruido del reloj y no un dato: es la leccion directa
// del Proyecto 3.
std::vector<rec::Medicion> medirOperaciones(const std::string& directorio,
                                            std::size_t cantidad, int repeticiones) {
    rec::Medicion crear{"crear", cantidad, repeticiones, {}};
    rec::Medicion listar{"listar", cantidad, repeticiones, {}};
    rec::Medicion metadatos{"metadatos", cantidad, repeticiones, {}};
    rec::Medicion eliminar{"eliminar", cantidad, repeticiones, {}};

    // Contenido de 1 KB, que es el tamano que fija el Dataset 4 del curso.
    const std::string contenido(1024, 'x');

    for (int r = 0; r < repeticiones; ++r) {
        rec::GestorArchivos gestor(directorio);

        // Se parte siempre de un directorio vacio: si quedaran archivos de la
        // repeticion anterior, la segunda medicion de "crear" fallaria por
        // nombre duplicado y la de "listar" contaria de mas.
        for (const rec::MetadatosArchivo& m : gestor.listar()) {
            gestor.eliminar(m.nombre);
        }

        rec::Cronometro c;
        for (std::size_t i = 0; i < cantidad; ++i) {
            gestor.crear("archivo_" + std::to_string(i) + ".dat", contenido);
        }
        crear.muestras.push_back(c.milisegundos());

        c.reiniciar();
        const std::vector<rec::MetadatosArchivo> entradas = gestor.listar();
        listar.muestras.push_back(c.milisegundos());

        c.reiniciar();
        for (const rec::MetadatosArchivo& m : entradas) {
            const rec::MetadatosArchivo detalle = gestor.metadatos(m.nombre);
            (void)detalle;   // solo interesa el tiempo de consultarlos
        }
        metadatos.muestras.push_back(c.milisegundos());

        c.reiniciar();
        for (const rec::MetadatosArchivo& m : entradas) {
            gestor.eliminar(m.nombre);
        }
        eliminar.muestras.push_back(c.milisegundos());
    }

    return {crear, listar, metadatos, eliminar};
}

// Escribe las mediciones en CSV.
//
// Los campos de texto van entrecomillados segun RFC 4180. Es la correccion del
// defecto DEF-001 del Proyecto 3, donde un campo con una coma desplazaba todas
// las columnas siguientes y los numeros publicados salian mal sin que nada
// fallara ruidosamente.
void exportarCsv(const std::string& ruta, const std::vector<rec::Medicion>& mediciones) {
    std::ofstream salida(ruta);
    if (!salida) {
        throw std::runtime_error(
            "no se pudo escribir '" + ruta + "'. Compruebe que el directorio existe");
    }

    salida << "plataforma,operacion,archivos,repeticiones,ms_media,ms_desv,ms_por_archivo\n";
    for (const rec::Medicion& m : mediciones) {
        salida << "\"" << rec::nombrePlataforma() << "\",\"" << m.operacion << "\","
               << m.cantidad << "," << m.repeticiones << ","
               << m.media() << "," << m.desviacion() << "," << m.msPorArchivo() << "\n";
    }
    if (!salida) throw std::runtime_error("fallo la escritura de '" + ruta + "'");
}

} // namespace

int main(int argc, char** argv) {
    Opciones op;
    std::string error;

    if (!interpretar(argc, argv, op, error)) {
        std::cerr << "Error de uso: " << error << "\n";
        std::cerr << "Use --help para ver las opciones disponibles.\n";
        return 2;
    }
    if (op.mostrarAyuda) {
        std::cout << AYUDA;
        return 0;
    }

    try {
        rec::GestorArchivos gestor(op.directorio);

        if (op.accion.empty()) {
            // Modo principal: el menu interactivo del requisito 4.
            rec::Menu menu(std::cin, std::cout, gestor);
            menu.ejecutar();
            return 0;
        }

        rec::imprimirEncabezado(std::cout, gestor.directorio());

        if (op.accion == "archivos") {
            rec::imprimirArchivos(std::cout, gestor.listar(), gestor.directorio());
        } else if (op.accion == "procesos") {
            rec::imprimirProcesos(std::cout, rec::listarProcesos(),
                                  op.todosLosProcesos ? rec::sinLimite() : op.topProcesos);
        } else if (op.accion == "memoria") {
            rec::imprimirMemoria(std::cout, rec::consultarMemoria());
        } else if (op.accion == "autoconsumo") {
            // La misma funcion que usa la opcion 4 del menu (RE-3).
            rec::imprimirAutoconsumoActual(std::cout);
        } else if (op.accion == "benchmark") {
            std::cout << "\n   Midiendo con " << op.cantidad << " archivos de 1 KB, "
                      << op.repeticiones << " repeticiones...\n";
            const std::vector<rec::Medicion> mediciones =
                medirOperaciones(op.directorio, op.cantidad, op.repeticiones);
            rec::imprimirMediciones(std::cout, mediciones);

            if (!op.archivoCsv.empty()) {
                exportarCsv(op.archivoCsv, mediciones);
                std::cout << "\n   CSV escrito en: " << op.archivoCsv << "\n";
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "\nERROR: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
