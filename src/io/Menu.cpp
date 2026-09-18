#include "io/Menu.h"

#include <exception>

#include "io/ReporteConsola.h"
#include "plataforma/Sistema.h"

namespace rec {
namespace {

// Cuantos procesos entran en una pagina. La lista completa de un escritorio
// pasa de 300 entradas y no cabe en pantalla; 15 caben sin tener que
// desplazarse, y el resto se ve pasando de pagina.
constexpr std::size_t PROCESOS_POR_PAGINA = 15;

// Quita espacios y retornos de carro de los extremos.
//
// El \r importa: si el script que conduce el menu tiene finales de linea de
// Windows, cada opcion llegaria como "1\r" y ninguna coincidiria. Es un fallo
// que solo aparece al automatizar y es desconcertante cuando pasa.
std::string recortar(std::string s) {
    // Quita el BOM de UTF-8 (EF BB BF) si viene al frente.
    //
    // DEFECTO REAL, hallado al automatizar el menu: cuando se conduce por
    // una tuberia desde PowerShell, la primera linea llega con esa marca
    // invisible delante y la opcion "1" no coincide con "1". El sintoma es
    // desconcertante --el programa dice que no es una opcion valida
    // mostrando lo que parece un 1-- y solo se ve mirando los bytes.
    if (s.size() >= 3 &&
        static_cast<unsigned char>(s[0]) == 0xEF &&
        static_cast<unsigned char>(s[1]) == 0xBB &&
        static_cast<unsigned char>(s[2]) == 0xBF) {
        s.erase(0, 3);
    }
    while (!s.empty() && (s.front() == ' ' || s.front() == '\t')) s.erase(s.begin());
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' ||
                          s.back() == '\r' || s.back() == '\n')) {
        s.pop_back();
    }
    return s;
}

} // namespace

Menu::Menu(std::istream& entrada, std::ostream& salida, GestorArchivos& gestor)
    : entrada_(entrada), salida_(salida), gestor_(gestor) {}

bool Menu::leerLinea(std::string& destino) {
    if (!std::getline(entrada_, destino)) return false;
    destino = recortar(destino);
    return true;
}

bool Menu::pedir(const std::string& mensaje, std::string& destino) {
    salida_ << mensaje;
    salida_.flush();
    return leerLinea(destino);
}

void Menu::conManejoDeErrores(const std::string& queSeIntentaba, void (Menu::*accion)()) {
    try {
        (this->*accion)();
    } catch (const std::exception& e) {
        // El mensaje dice QUE se intentaba y POR QUE fallo. Sin la primera
        // parte, un error suelto obliga al usuario a adivinar de donde salio.
        salida_ << "\n   ERROR al " << queSeIntentaba << ":\n";
        salida_ << "   " << e.what() << "\n";
    }
}

void Menu::mostrarMenuPrincipal() {
    salida_ << "\n";
    salida_ << "==================================================================\n";
    salida_ << " MENU PRINCIPAL\n";
    salida_ << "==================================================================\n";
    salida_ << "   1) Gestion de archivos      (crear, listar, eliminar, metadatos)\n";
    salida_ << "   2) Monitoreo de procesos    (PID, nombre, memoria)\n";
    salida_ << "   3) Monitoreo de memoria     (total, usada, disponible)\n";
    salida_ << "   4) Consumo de esta herramienta\n";
    salida_ << "   0) Salir\n";
    salida_ << "------------------------------------------------------------------\n";
}

void Menu::mostrarMenuArchivos() {
    salida_ << "\n";
    salida_ << "   --- Gestion de archivos ---------------------------------------\n";
    salida_ << "     1) Listar archivos y metadatos\n";
    salida_ << "     2) Crear archivo\n";
    salida_ << "     3) Ver metadatos de un archivo\n";
    salida_ << "     4) Eliminar archivo\n";
    salida_ << "     0) Volver al menu principal\n";
    salida_ << "   ---------------------------------------------------------------\n";
}

void Menu::ejecutar() {
    imprimirEncabezado(salida_, gestor_.directorio());

    std::string opcion;
    while (true) {
        mostrarMenuPrincipal();
        if (!pedir(" Opcion: ", opcion)) {
            // Entrada agotada. Ocurre cuando el menu se conduce desde un
            // script: se sale limpiamente en vez de quedarse esperando.
            salida_ << "\n(entrada agotada; se cierra la herramienta)\n";
            return;
        }

        if (opcion == "0" || opcion == "salir") {
            salida_ << "\nHasta luego.\n";
            return;
        }
        if (opcion == "1") {
            opcionArchivos();
        } else if (opcion == "2") {
            conManejoDeErrores("consultar los procesos", &Menu::opcionProcesos);
        } else if (opcion == "3") {
            conManejoDeErrores("consultar la memoria", &Menu::opcionMemoria);
        } else if (opcion == "4") {
            conManejoDeErrores("medir el consumo propio", &Menu::opcionAutoconsumo);
        } else if (opcion.empty()) {
            // Una linea en blanco solo vuelve a mostrar el menu: no es un
            // error del usuario, es haber pulsado Enter.
            continue;
        } else {
            salida_ << "\n   '" << opcion << "' no es una opcion valida."
                    << " Escriba 1, 2, 3, 4 o 0.\n";
        }
    }
}

void Menu::opcionArchivos() {
    std::string opcion;
    while (true) {
        mostrarMenuArchivos();
        if (!pedir("   Opcion: ", opcion)) return;

        if (opcion == "0" || opcion.empty()) return;

        if (opcion == "1") {
            conManejoDeErrores("listar los archivos", &Menu::archivosListar);
        } else if (opcion == "2") {
            conManejoDeErrores("crear el archivo", &Menu::archivosCrear);
        } else if (opcion == "3") {
            conManejoDeErrores("consultar los metadatos", &Menu::archivosMetadatos);
        } else if (opcion == "4") {
            conManejoDeErrores("eliminar el archivo", &Menu::archivosEliminar);
        } else {
            salida_ << "\n     '" << opcion << "' no es una opcion valida."
                    << " Escriba 1, 2, 3, 4 o 0.\n";
        }
    }
}

void Menu::archivosListar() {
    imprimirArchivos(salida_, gestor_.listar(), gestor_.directorio());
}

void Menu::archivosCrear() {
    std::string nombre;
    if (!pedir("     Nombre del archivo: ", nombre)) return;

    std::string contenido;
    if (!pedir("     Contenido (puede dejarse vacio): ", contenido)) return;

    gestor_.crear(nombre, contenido);
    salida_ << "\n   Creado '" << nombre << "' (" << contenido.size() << " bytes).\n";
}

void Menu::archivosMetadatos() {
    std::string nombre;
    if (!pedir("     Nombre del archivo: ", nombre)) return;
    imprimirMetadatos(salida_, gestor_.metadatos(nombre));
}

void Menu::archivosEliminar() {
    std::string nombre;
    if (!pedir("     Nombre del archivo a eliminar: ", nombre)) return;

    // Confirmacion antes de una operacion destructiva. El enunciado no la
    // pide, pero borrar sin preguntar es justo el tipo de detalle que se echa
    // en falta cuando alguien usa la herramienta de verdad.
    std::string confirmacion;
    if (!pedir("     Confirmar eliminacion de '" + nombre + "' [s/N]: ", confirmacion)) return;
    if (confirmacion != "s" && confirmacion != "S") {
        salida_ << "\n   Cancelado: no se elimino nada.\n";
        return;
    }

    gestor_.eliminar(nombre);
    salida_ << "\n   Eliminado '" << nombre << "'.\n";
}

void Menu::opcionProcesos() {
    // Se consulta UNA sola vez y se ordena UNA sola vez: si cada pagina
    // volviera a preguntar al sistema, la lista cambiaria entre paginas y el
    // mismo proceso podria verse dos veces o ninguna.
    std::vector<ProcesoInfo> procesos = listarProcesos();
    ordenarPorMemoria(procesos);

    std::size_t pagina = 0;
    while (true) {
        const std::size_t totalPaginas =
            imprimirPaginaProcesos(salida_, procesos, pagina, PROCESOS_POR_PAGINA);
        const bool hayMas = pagina + 1 < totalPaginas;

        std::string tecla;
        const std::string aviso = hayMas
            ? "\n   [Enter] pagina siguiente   [a] anterior   [0] volver: "
            : "\n   [a] pagina anterior   [Enter o 0] volver: ";
        if (!pedir(aviso, tecla)) return;   // entrada agotada: se sale limpio

        if (tecla == "0") return;
        if (tecla == "a" || tecla == "A") {
            if (pagina > 0) --pagina;
            continue;
        }
        if (!hayMas) return;   // en la ultima pagina, cualquier otra tecla vuelve
        ++pagina;
    }
}

void Menu::opcionMemoria() {
    imprimirMemoria(salida_, consultarMemoria());
}

// RE-3 desde el menu: es el mismo calculo que `--autoconsumo`, porque los dos
// llaman a la misma funcion. Si midieran por separado podrian dar cifras
// distintas y no habria forma de saber cual creer.
void Menu::opcionAutoconsumo() {
    imprimirAutoconsumoActual(salida_);
}

} // namespace rec
