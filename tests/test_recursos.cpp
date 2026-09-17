// ============================================================================
// test_recursos.cpp -- Suite de pruebas del Administrador de Recursos
// Proyecto 4, TIIT2007 Sistemas Operativos, Universidad Invenio
// Autor: Fabrizio Espinoza Arce
//
// Arnes propio, sin framework externo. Misma decision que en los Proyectos 1,
// 2 y 3: en la maquina de desarrollo no hay gestor de paquetes de C++, asi que
// cero dependencias significa que la suite compila en cualquier maquina con un
// compilador de C++17 y nada mas.
//
// ESTE FICHERO SE EJECUTA EN LAS DOS PLATAFORMAS, sin cambios. Es el mismo
// codigo el que corre bajo MSVC en Windows y bajo g++ en Linux, y es una de
// las evidencias de compatibilidad: si la capa de plataforma estuviera mal
// abstraida, la suite no compilaria en el otro sistema.
//
// EL DESAFIO METODOLOGICO DE ESTE PROYECTO
//
// A diferencia del Proyecto 3 --donde la simulacion era determinista y se
// podia comparar contra valores exactos-- aqui casi todo lo que se mide
// depende del estado real de la maquina: cuanta memoria hay libre en este
// instante, cuantos procesos corren ahora. No se puede escribir
// `afirmar(procesos == 358)`.
//
// La suite se apoya entonces en tres clases de asercion que SI son estables:
//
//   1. Comportamiento de la gestion de archivos, que es determinista y esta
//      bajo control: se crea un directorio temporal propio y se opera ahi.
//   2. Contratos de error: las precondiciones se hacen cumplir siempre.
//   3. INVARIANTES sobre los datos del sistema: no se afirma cuanta memoria
//      hay, sino que usada + disponible == total y que el porcentaje cae en
//      [0, 100]. Eso es cierto en cualquier maquina y en cualquier momento.
// ============================================================================

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "core/Cronometro.h"
#include "core/Estadistica.h"
#include "io/FormatoTabla.h"
#include "plataforma/Archivos.h"
#include "plataforma/Sistema.h"

namespace fs = std::filesystem;

namespace {

// --- Arnes minimo -----------------------------------------------------------

int totalPruebas = 0;
int pruebasSuperadas = 0;
std::vector<std::string> fallos;

void afirmar(bool condicion, const std::string& descripcion) {
    if (!condicion) fallos.push_back(descripcion);
}

void afirmarIgual(long long obtenido, long long esperado, const std::string& descripcion) {
    afirmar(obtenido == esperado,
            descripcion + " (se obtuvo " + std::to_string(obtenido) + ", se esperaba " +
                std::to_string(esperado) + ")");
}

// Nunca se comparan dos `double` con ==: el resultado de una division rara vez
// es exacto en binario y la prueba fallaria por motivos ajenos a lo que se
// pretende comprobar.
void afirmarCasiIgual(double obtenido, double esperado, const std::string& descripcion) {
    afirmar(std::fabs(obtenido - esperado) < 1e-9,
            descripcion + " (se obtuvo " + std::to_string(obtenido) + ", se esperaba " +
                std::to_string(esperado) + ")");
}

void ejecutar(const std::string& nombre, void (*prueba)()) {
    ++totalPruebas;
    const std::size_t fallosAntes = fallos.size();
    try {
        prueba();
    } catch (const std::exception& e) {
        fallos.push_back(nombre + " lanzo una excepcion inesperada: " + e.what());
    } catch (...) {
        fallos.push_back(nombre + " lanzo una excepcion desconocida");
    }

    if (fallos.size() == fallosAntes) {
        ++pruebasSuperadas;
        std::cout << "[OK] " << nombre << "\n";
    } else {
        std::cout << "[FALLA] " << nombre << "\n";
        for (std::size_t i = fallosAntes; i < fallos.size(); ++i) {
            std::cout << "       -> " << fallos[i] << "\n";
        }
    }
}

// --- Utilidad: directorio de trabajo temporal y aislado ----------------------
//
// Cada prueba que toca el disco crea el suyo y lo borra al terminar, pase lo
// que pase. Se usa un destructor (RAII) y no una llamada explicita al final:
// si la prueba lanza una excepcion a mitad, el destructor se ejecuta igual y
// no queda basura en el disco. Es el idiom de C++ que en C# se escribiria con
// `using` e `IDisposable`.
class DirectorioTemporal {
public:
    DirectorioTemporal() {
        static int contador = 0;
        ruta_ = (fs::temp_directory_path() /
                 ("p4_prueba_" + std::to_string(++contador) + "_" +
                  std::to_string(reinterpret_cast<std::uintptr_t>(this))))
                    .string();
        std::error_code ec;
        fs::remove_all(ruta_, ec);
        fs::create_directories(ruta_, ec);
    }

    ~DirectorioTemporal() {
        std::error_code ec;
        fs::remove_all(ruta_, ec);
    }

    // Sin copia: dos objetos apuntando al mismo directorio lo borrarian dos
    // veces, y el segundo destructor actuaria sobre una ruta ya eliminada.
    DirectorioTemporal(const DirectorioTemporal&) = delete;
    DirectorioTemporal& operator=(const DirectorioTemporal&) = delete;

    const std::string& ruta() const { return ruta_; }

private:
    std::string ruta_;
};

// ============================================================================
// GRUPO 1 -- GESTION DE ARCHIVOS (RF-1)
// ============================================================================

void prueba_crearYListar() {
    DirectorioTemporal dir;
    rec::GestorArchivos gestor(dir.ruta());

    afirmarIgual(static_cast<long long>(gestor.listar().size()), 0,
                 "un directorio recien creado esta vacio");

    gestor.crear("uno.txt", "contenido");
    gestor.crear("dos.txt", "");

    const std::vector<rec::MetadatosArchivo> entradas = gestor.listar();
    afirmarIgual(static_cast<long long>(entradas.size()), 2, "se crearon dos archivos");

    // El listado va ordenado alfabeticamente: sin ese orden, dos ejecuciones
    // seguidas podrian mostrar listas distintas y la evidencia experimental no
    // seria reproducible.
    afirmar(entradas[0].nombre == "dos.txt" && entradas[1].nombre == "uno.txt",
            "el listado debe venir ordenado alfabeticamente");
}

void prueba_metadatos() {
    DirectorioTemporal dir;
    rec::GestorArchivos gestor(dir.ruta());

    const std::string contenido = "12345678";   // 8 bytes exactos
    gestor.crear("medido.txt", contenido);

    const rec::MetadatosArchivo m = gestor.metadatos("medido.txt");
    afirmarIgual(static_cast<long long>(m.tamanoBytes), 8,
                 "el tamano debe ser el numero de bytes escritos");
    afirmar(!m.esDirectorio, "un archivo no es un directorio");
    afirmarIgual(static_cast<long long>(m.permisos.size()), 9,
                 "los permisos se muestran en notacion de nueve caracteres");
    // No se comprueba el valor de la fecha --depende del reloj-- sino su
    // formato: 19 caracteres "AAAA-MM-DD HH:MM:SS".
    afirmarIgual(static_cast<long long>(m.fechaModificacion.size()), 19,
                 "la fecha se formatea como AAAA-MM-DD HH:MM:SS");
}

void prueba_eliminar() {
    DirectorioTemporal dir;
    rec::GestorArchivos gestor(dir.ruta());

    gestor.crear("temporal.txt", "x");
    afirmarIgual(static_cast<long long>(gestor.cuantos()), 1, "existe un archivo");

    gestor.eliminar("temporal.txt");
    afirmarIgual(static_cast<long long>(gestor.cuantos()), 0, "el archivo se elimino");
}

void prueba_directorioDeTrabajoSeCrea() {
    DirectorioTemporal dir;
    const std::string anidado = dir.ruta() + "/sub/carpeta";

    rec::GestorArchivos gestor(anidado);
    afirmar(fs::exists(anidado), "el constructor crea el directorio de trabajo si no existe");
    gestor.crear("dentro.txt", "ok");
    afirmarIgual(static_cast<long long>(gestor.cuantos()), 1, "se puede operar en el creado");
}

// ============================================================================
// GRUPO 2 -- LA FRONTERA DEL DIRECTORIO CONTROLADO
//
// El enunciado pide operar "dentro de un directorio de trabajo controlado".
// Estas pruebas comprueban que esa frontera se hace cumplir de verdad, y no es
// solo una carpeta por omision.
// ============================================================================

void exigirRechazo(rec::GestorArchivos& gestor, const std::string& nombre,
                   const std::string& porQue) {
    bool lanzo = false;
    try {
        gestor.crear(nombre, "x");
    } catch (const std::invalid_argument&) {
        lanzo = true;
    }
    afirmar(lanzo, "deberia rechazarse '" + nombre + "': " + porQue);
}

void prueba_rechazaSalirConDosPuntos() {
    DirectorioTemporal dir;
    rec::GestorArchivos gestor(dir.ruta() + "/trabajo");

    exigirRechazo(gestor, "../fuera.txt", "sale un nivel del directorio controlado");
    exigirRechazo(gestor, "../../mas_fuera.txt", "sale dos niveles");
    exigirRechazo(gestor, "sub/../../fuera.txt", "sale despues de entrar y salir");
}

void prueba_rechazaRutaAbsoluta() {
    DirectorioTemporal dir;
    rec::GestorArchivos gestor(dir.ruta() + "/trabajo");

#ifdef _WIN32
    exigirRechazo(gestor, "C:\\Windows\\Temp\\intruso.txt", "es una ruta absoluta de Windows");
#else
    exigirRechazo(gestor, "/tmp/intruso.txt", "es una ruta absoluta de Unix");
#endif
}

void prueba_rechazaNombreVacio() {
    DirectorioTemporal dir;
    rec::GestorArchivos gestor(dir.ruta());
    exigirRechazo(gestor, "", "un nombre vacio no identifica ningun archivo");
}

void prueba_aceptaSubdirectorioLegitimo() {
    // El contraste: la frontera bloquea salir, pero NO bloquea moverse dentro.
    // Sin esta prueba, una comprobacion demasiado estricta pasaria inadvertida.
    DirectorioTemporal dir;
    const std::string trabajo = dir.ruta() + "/trabajo";
    fs::create_directories(trabajo + "/sub");

    rec::GestorArchivos gestor(trabajo);
    gestor.crear("sub/valido.txt", "ok");
    afirmar(fs::exists(trabajo + "/sub/valido.txt"),
            "un subdirectorio dentro del area controlada si debe permitirse");
}

void prueba_fronteraNoSeEnganaConNombreParecido() {
    // Si la comprobacion se hiciera comparando prefijos de cadena, un
    // directorio hermano llamado "trabajo_otro" pasaria el filtro de
    // "trabajo". Se comprueba componente a componente, y esta prueba lo fija.
    DirectorioTemporal dir;
    const std::string trabajo = dir.ruta() + "/trabajo";
    fs::create_directories(dir.ruta() + "/trabajo_otro");

    rec::GestorArchivos gestor(trabajo);
    exigirRechazo(gestor, "../trabajo_otro/intruso.txt",
                  "un hermano con nombre parecido sigue estando fuera");
}

void prueba_rechazaElPropioDirectorio() {
    // DEFECTO DEF-006: "eliminar ." borraba el directorio de trabajo entero
    // cuando estaba vacio, porque la raiz pasaba la comprobacion de frontera
    // (es prefijo de si misma). Se fija que ningun nombre que designe la raiz
    // se acepte, y que el directorio siga existiendo despues del intento.
    DirectorioTemporal dir;
    const std::string trabajo = dir.ruta() + "/trabajo";
    fs::create_directories(trabajo + "/sub");
    rec::GestorArchivos gestor(trabajo);

    for (const std::string nombre : {".", "./", "sub/..", "sub/../."}) {
        bool lanzo = false;
        try {
            gestor.eliminar(nombre);
        } catch (const std::invalid_argument&) {
            lanzo = true;
        }
        afirmar(lanzo, "eliminar '" + nombre + "' debe rechazarse: designa la raiz");
    }
    // El caso peligroso de verdad: con el directorio VACIO, fs::remove si
    // habria podido borrarlo.
    fs::remove(trabajo + "/sub");
    bool lanzo = false;
    try {
        gestor.eliminar(".");
    } catch (const std::invalid_argument&) {
        lanzo = true;
    }
    afirmar(lanzo, "eliminar '.' con el directorio vacio debe rechazarse");
    afirmar(fs::is_directory(trabajo),
            "el directorio de trabajo debe seguir existiendo tras el intento");

    exigirRechazo(gestor, ".", "crear sobre la raiz no tiene sentido");
}

// ============================================================================
// GRUPO 3 -- CONTRATOS DE ERROR (RNF-1)
//
// El enunciado pide manejo robusto de tres casos concretos: archivo
// inexistente, permisos insuficientes y comando no disponible. Cada uno tiene
// su prueba.
// ============================================================================

void prueba_archivoInexistente() {
    DirectorioTemporal dir;
    rec::GestorArchivos gestor(dir.ruta());

    bool lanzoAlEliminar = false;
    std::string mensaje;
    try {
        gestor.eliminar("no_existe.txt");
    } catch (const std::invalid_argument& e) {
        lanzoAlEliminar = true;
        mensaje = e.what();
    }
    afirmar(lanzoAlEliminar, "eliminar un archivo inexistente debe lanzar");
    afirmar(mensaje.find("no_existe.txt") != std::string::npos,
            "el mensaje debe nombrar el archivo, y dice: " + mensaje);

    bool lanzoAlConsultar = false;
    try {
        gestor.metadatos("tampoco_existe.txt");
    } catch (const std::invalid_argument&) {
        lanzoAlConsultar = true;
    }
    afirmar(lanzoAlConsultar, "consultar metadatos de un inexistente debe lanzar");
}

void prueba_archivoDuplicado() {
    DirectorioTemporal dir;
    rec::GestorArchivos gestor(dir.ruta());

    gestor.crear("unico.txt", "primero");

    bool lanzo = false;
    std::string mensaje;
    try {
        gestor.crear("unico.txt", "segundo");
    } catch (const std::invalid_argument& e) {
        lanzo = true;
        mensaje = e.what();
    }
    afirmar(lanzo, "crear un archivo que ya existe debe lanzar en vez de sobrescribir");
    afirmar(mensaje.find("ya existe") != std::string::npos,
            "el mensaje debe decir que ya existe, y dice: " + mensaje);

    // Y lo que importa de verdad: el contenido original no se toco.
    std::ifstream f((fs::path(dir.ruta()) / "unico.txt").string());
    std::string contenido;
    std::getline(f, contenido);
    afirmar(contenido == "primero",
            "el archivo original no debe modificarse al rechazar el duplicado");
}

void prueba_directorioDeTrabajoInvalido() {
    // Pasar un archivo donde se espera un directorio.
    DirectorioTemporal dir;
    const std::string archivo = (fs::path(dir.ruta()) / "soy_un_archivo.txt").string();
    { std::ofstream f(archivo); f << "x"; }

    bool lanzo = false;
    try {
        rec::GestorArchivos gestor(archivo);
    } catch (const std::runtime_error&) {
        lanzo = true;
    }
    afirmar(lanzo, "usar un archivo como directorio de trabajo debe lanzar");
}

// SEGUNDO CASO DEL RNF-1: permisos insuficientes, provocados de verdad.
//
// Se quita el permiso de escritura sobre un directorio con la herramienta
// nativa de cada sistema y se intenta crear un archivo dentro:
//
//   Windows -> icacls, con una entrada DENEGAR escritura para "Todos"
//              (SID S-1-1-0). Se usa el SID y no el nombre del grupo porque
//              el nombre cambia con el idioma de Windows.
//   Linux   -> los bits de permiso del directorio a r-x (0555).
//
// Si la sonda puede escribir igualmente (en Linux, root ignora los bits), la
// prueba lo dice y no afirma nada: no se puede fingir un caso que el sistema
// no produce.
class DirectorioSinEscritura {
public:
    explicit DirectorioSinEscritura(const std::string& ruta) : ruta_(ruta) {
        fs::create_directories(ruta_);
#ifdef _WIN32
        comando("icacls \"" + ruta_ + "\" /deny *S-1-1-0:(W,AD) >nul 2>&1");
#else
        fs::permissions(ruta_, fs::perms::owner_read | fs::perms::owner_exec |
                                   fs::perms::group_read | fs::perms::group_exec |
                                   fs::perms::others_read | fs::perms::others_exec);
#endif
    }

    // Se restaura SIEMPRE en el destructor: si no, DirectorioTemporal no
    // podria borrar la carpeta y quedaria basura en el directorio temporal.
    ~DirectorioSinEscritura() {
#ifdef _WIN32
        comando("icacls \"" + ruta_ + "\" /remove:d *S-1-1-0 >nul 2>&1");
#else
        std::error_code ec;
        fs::permissions(ruta_, fs::perms::owner_all, ec);
#endif
    }

    DirectorioSinEscritura(const DirectorioSinEscritura&) = delete;
    DirectorioSinEscritura& operator=(const DirectorioSinEscritura&) = delete;

    // Verdadero si, pese a todo, se puede escribir en el directorio.
    bool sigueSiendoEscribible() const {
        const std::string sonda = ruta_ + "/sonda.tmp";
        bool pudo = false;
        {
            std::ofstream f(sonda);
            pudo = static_cast<bool>(f);
        }
        std::error_code ec;
        fs::remove(sonda, ec);
        return pudo;
    }

private:
    std::string ruta_;

    static void comando(const std::string& linea) {
        const int codigo = std::system(linea.c_str());
        (void)codigo;   // si icacls fallara, la sonda lo detecta despues
    }
};

void prueba_permisosInsuficientes() {
    DirectorioTemporal dir;
    const std::string protegido = dir.ruta() + "/protegido";
    DirectorioSinEscritura candado(protegido);

    if (candado.sigueSiendoEscribible()) {
        std::cout << "       (omitida: este usuario puede escribir aunque se quiten"
                     " los permisos, p. ej. root)\n";
        return;
    }

    rec::GestorArchivos gestor(protegido);

    bool lanzo = false;
    std::string mensaje;
    try {
        gestor.crear("no_deberia_existir.txt", "x");
    } catch (const std::runtime_error& e) {
        lanzo = true;
        mensaje = e.what();
    }
    afirmar(lanzo, "crear sin permiso de escritura debe lanzar runtime_error");
    afirmar(mensaje.find("permisos insuficientes") != std::string::npos,
            "el mensaje debe decir 'permisos insuficientes', y dice: " + mensaje);
    afirmar(!fs::exists(protegido + "/no_deberia_existir.txt"),
            "no debe quedar ningun archivo creado");
}

// ============================================================================
// GRUPO 4 -- MEMORIA DEL SISTEMA (RF-3)
//
// No se afirma CUANTA memoria hay --depende de la maquina y del momento-- sino
// las relaciones que deben cumplirse siempre.
// ============================================================================

void prueba_memoriaCoherente() {
    const rec::InfoMemoria m = rec::consultarMemoria();

    afirmar(m.totalKb > 0, "la memoria total debe ser mayor que cero");
    afirmar(m.disponibleKb <= m.totalKb,
            "la disponible no puede superar a la total");
    afirmarIgual(static_cast<long long>(m.usadaKb() + m.disponibleKb),
                 static_cast<long long>(m.totalKb),
                 "usada + disponible debe dar exactamente la total");

    const double pct = m.porcentajeUsado();
    afirmar(pct >= 0.0 && pct <= 100.0,
            "el porcentaje usado debe caer en [0, 100], y dio " + std::to_string(pct));
}

void prueba_memoriaEsPlausible() {
    const rec::InfoMemoria m = rec::consultarMemoria();
    // Cota deliberadamente laxa: cualquier maquina que pueda compilar este
    // proyecto tiene mas de 128 MB y menos de 100 TB. Una cota estrecha
    // convertiria una caracteristica de esta maquina en un requisito, y la
    // suite fallaria en otra.
    const unsigned long long mb = m.totalKb / 1024;
    afirmar(mb > 128, "la memoria total deberia superar 128 MB, y dio " + std::to_string(mb));
    afirmar(mb < 100ULL * 1024 * 1024, "la memoria total deberia ser menor que 100 TB");
}

void prueba_memoriaEsRepetible() {
    // Dos consultas seguidas no dan el mismo valor --la memoria cambia sola--
    // pero el TOTAL fisico no cambia entre dos llamadas consecutivas.
    const rec::InfoMemoria a = rec::consultarMemoria();
    const rec::InfoMemoria b = rec::consultarMemoria();
    afirmarIgual(static_cast<long long>(a.totalKb), static_cast<long long>(b.totalKb),
                 "la memoria total del sistema no cambia entre dos consultas");
}

// ============================================================================
// GRUPO 5 -- PROCESOS DEL SISTEMA (RF-2)
// ============================================================================

void prueba_listaDeProcesosNoVacia() {
    const std::vector<rec::ProcesoInfo> procesos = rec::listarProcesos();
    afirmar(!procesos.empty(),
            "siempre hay al menos un proceso: el que ejecuta esta prueba");
    // Cota laxa por la misma razon que en memoria: un contenedor minimo puede
    // tener una decena y un escritorio varios cientos.
    afirmar(procesos.size() < 100000, "el numero de procesos deberia ser razonable");
}

void prueba_camposDeProcesoValidos() {
    const std::vector<rec::ProcesoInfo> procesos = rec::listarProcesos();

    int sinNombre = 0;
    for (const rec::ProcesoInfo& p : procesos) {
        if (p.nombre.empty()) ++sinNombre;
    }
    afirmarIgual(sinNombre, 0, "ningun proceso debe quedar sin nombre tras el parseo");

    // Al menos uno con memoria mayor que cero: si TODOS dieran cero, el campo
    // de memoria se estaria parseando mal. Es la comprobacion que habria
    // detectado un error de separador de miles en un Windows en otro idioma.
    const bool algunoConMemoria = std::any_of(
        procesos.begin(), procesos.end(),
        [](const rec::ProcesoInfo& p) { return p.memoriaKb > 0; });
    afirmar(algunoConMemoria,
            "al menos un proceso debe reportar memoria mayor que cero; si no,"
            " el campo se esta parseando mal");
}

void prueba_procesosEsRepetible() {
    // El numero exacto cambia entre llamadas, pero las dos deben responder sin
    // error y devolver algo. Se comprueba la estabilidad del mecanismo, no el
    // valor.
    const std::vector<rec::ProcesoInfo> a = rec::listarProcesos();
    const std::vector<rec::ProcesoInfo> b = rec::listarProcesos();
    afirmar(!a.empty() && !b.empty(),
            "dos consultas consecutivas deben devolver procesos las dos veces");
}

void prueba_comandoNoDisponible() {
    // TERCER CASO DE ERROR DEL RNF-1: "comando no disponible".
    //
    // No se puede provocar desinstalando tasklist o ps del sistema, asi que la
    // capa de plataforma expone listarProcesosCon(), que acepta otro comando.
    // Esa costura existe exactamente para esto: sin ella este caso se podria
    // AFIRMAR pero no PROBAR, y afirmar sin probar es lo que este proyecto
    // evita.
    const std::string inexistente = "comando_que_no_existe_en_ningun_sistema_12345";

    bool lanzo = false;
    std::string mensaje;
    try {
        rec::listarProcesosCon(inexistente);
    } catch (const std::runtime_error& e) {
        lanzo = true;
        mensaje = e.what();
    }

    afirmar(lanzo, "un comando inexistente debe lanzar runtime_error");
    afirmar(mensaje.find(inexistente) != std::string::npos,
            "el mensaje debe decir QUE comando se intento ejecutar, y dice: " + mensaje);
    afirmar(mensaje.find("no disponible") != std::string::npos ||
                mensaje.find("codigo") != std::string::npos,
            "el mensaje debe explicar la causa probable, y dice: " + mensaje);
}

void prueba_parseoDeNombresConEspacios() {
    // La misma costura de 5.4 permite probar el PARSEO con una salida
    // controlada, en vez de depender de que la maquina tenga un proceso con
    // un nombre raro corriendo en ese momento.
    //
    // Casos cubiertos: nombre con espacios (Firefox lanza "Web Content") y,
    // en Windows, el separador de miles que cambia con el idioma.
#ifdef _WIN32
    const std::string comando =
        "echo \"Web Content.exe\",\"4242\",\"Console\",\"1\",\"12.345 K\"";
    const std::string nombreEsperado = "Web Content.exe";
#else
    const std::string comando = "printf '   4242 Web Content   12345\\n'";
    const std::string nombreEsperado = "Web Content";
#endif
    const std::vector<rec::ProcesoInfo> procesos = rec::listarProcesosCon(comando);

    afirmarIgual(static_cast<long long>(procesos.size()), 1,
                 "la linea con un nombre con espacios no debe descartarse");
    if (procesos.size() != 1) return;
    afirmarIgual(static_cast<long long>(procesos[0].pid), 4242, "PID");
    afirmar(procesos[0].nombre == nombreEsperado,
            "el nombre debe conservar el espacio, y quedo: '" + procesos[0].nombre + "'");
    afirmarIgual(static_cast<long long>(procesos[0].memoriaKb), 12345,
                 "la memoria se lee completa, sin importar el separador de miles");
}

void prueba_plataformaSeIdentifica() {
    const std::string nombre = rec::nombrePlataforma();
    afirmar(nombre == "Windows" || nombre == "Linux",
            "la plataforma debe identificarse como Windows o Linux, y dijo: " + nombre);

    const std::string comando = rec::comandoDeProcesos();
    afirmar(!comando.empty(), "el comando de procesos debe estar documentado");

#ifdef _WIN32
    afirmar(nombre == "Windows", "compilado en Windows debe reportar Windows");
    afirmar(comando.find("tasklist") != std::string::npos,
            "en Windows el comando debe ser tasklist");
#else
    afirmar(nombre == "Linux", "compilado en Linux debe reportar Linux");
    afirmar(comando.find("ps") != std::string::npos,
            "en Linux el comando debe ser ps");
#endif
}

// ============================================================================
// GRUPO 6 -- UTILIDADES
// ============================================================================

void prueba_cronometroMide() {
    rec::Cronometro c;
    // No se afirma un tiempo concreto --depende de la maquina-- sino que el
    // cronometro avanza y nunca retrocede, que es lo que garantiza
    // steady_clock.
    const double t1 = c.milisegundos();
    for (volatile int i = 0; i < 1000000; ++i) { }
    const double t2 = c.milisegundos();

    afirmar(t1 >= 0.0, "el tiempo medido nunca es negativo");
    afirmar(t2 >= t1, "el cronometro nunca retrocede");

    c.reiniciar();
    afirmar(c.milisegundos() < t2 + 1.0, "reiniciar vuelve a empezar la cuenta");
}

void prueba_estadistica() {
    afirmarCasiIgual(rec::media({1.0, 2.0, 3.0, 4.0}), 2.5, "la media de {1,2,3,4} es 2.5");
    afirmarCasiIgual(rec::media({}), 0.0,
                     "la media del vector vacio es 0, no una division por cero");

    // Con {2,4,4,4,5,5,7,9} la desviacion MUESTRAL (divisor n-1) es
    // sqrt(32/7). La poblacional (divisor n) daria 2.0 exacto: se comprueba la
    // muestral para que un cambio accidental del divisor no pase inadvertido.
    const std::vector<double> v = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    afirmarCasiIgual(rec::desviacionEstandar(v), std::sqrt(32.0 / 7.0),
                     "la desviacion debe ser MUESTRAL (divisor n-1)");
    afirmarCasiIgual(rec::desviacionEstandar({5.0}), 0.0,
                     "una sola observacion no tiene dispersion que estimar");
}

void prueba_formatoTabla() {
    rec::FormatoTabla tabla;
    tabla.agregarColumna("Archivo", rec::Alineacion::Izquierda);
    tabla.agregarColumna("Bytes", rec::Alineacion::Derecha);
    tabla.agregarFila({"datos.txt", "1024"});

    const std::string salida = tabla.render();
    afirmar(salida.find("datos.txt") != std::string::npos, "la tabla contiene la fila");
    afirmar(salida.find('+') != std::string::npos, "la tabla tiene bordes");

    bool lanzo = false;
    try {
        tabla.agregarFila({"faltan celdas"});
    } catch (const std::invalid_argument&) {
        lanzo = true;
    }
    afirmar(lanzo, "una fila con menos celdas que columnas debe lanzar");
}

} // namespace

int main() {
    std::cout << "==================================================================\n";
    std::cout << " Suite de pruebas -- Administrador de Recursos (Proyecto 4)\n";
    std::cout << " Plataforma: " << rec::nombrePlataforma() << "\n";
    std::cout << "==================================================================\n";

    std::cout << "\n-- Grupo 1: gestion de archivos (RF-1) ---------------------------\n";
    ejecutar("1.1 Crear y listar", prueba_crearYListar);
    ejecutar("1.2 Metadatos: tamano, fecha y permisos", prueba_metadatos);
    ejecutar("1.3 Eliminar", prueba_eliminar);
    ejecutar("1.4 El directorio de trabajo se crea si no existe", prueba_directorioDeTrabajoSeCrea);

    std::cout << "\n-- Grupo 2: frontera del directorio controlado -------------------\n";
    ejecutar("2.1 Rechaza salir con ..", prueba_rechazaSalirConDosPuntos);
    ejecutar("2.2 Rechaza ruta absoluta", prueba_rechazaRutaAbsoluta);
    ejecutar("2.3 Rechaza nombre vacio", prueba_rechazaNombreVacio);
    ejecutar("2.4 Acepta subdirectorio legitimo", prueba_aceptaSubdirectorioLegitimo);
    ejecutar("2.5 No se engana con un hermano de nombre parecido",
             prueba_fronteraNoSeEnganaConNombreParecido);
    ejecutar("2.6 Rechaza el propio directorio de trabajo", prueba_rechazaElPropioDirectorio);

    std::cout << "\n-- Grupo 3: contratos de error (RNF-1) ---------------------------\n";
    ejecutar("3.1 Archivo inexistente", prueba_archivoInexistente);
    ejecutar("3.2 Archivo duplicado, sin sobrescribir", prueba_archivoDuplicado);
    ejecutar("3.3 Directorio de trabajo invalido", prueba_directorioDeTrabajoInvalido);
    ejecutar("3.4 Permisos insuficientes: error claro (RNF-1)", prueba_permisosInsuficientes);

    std::cout << "\n-- Grupo 4: memoria del sistema (RF-3) ---------------------------\n";
    ejecutar("4.1 usada + disponible = total", prueba_memoriaCoherente);
    ejecutar("4.2 El valor es plausible", prueba_memoriaEsPlausible);
    ejecutar("4.3 El total no cambia entre consultas", prueba_memoriaEsRepetible);

    std::cout << "\n-- Grupo 5: procesos del sistema (RF-2) --------------------------\n";
    ejecutar("5.1 La lista no viene vacia", prueba_listaDeProcesosNoVacia);
    ejecutar("5.2 Los campos se parsean bien", prueba_camposDeProcesoValidos);
    ejecutar("5.3 Dos consultas seguidas funcionan", prueba_procesosEsRepetible);
    ejecutar("5.4 Comando no disponible: error claro (RNF-1)", prueba_comandoNoDisponible);
    ejecutar("5.5 La plataforma se identifica sola", prueba_plataformaSeIdentifica);
    ejecutar("5.6 Nombres con espacios y separador de miles", prueba_parseoDeNombresConEspacios);

    std::cout << "\n-- Grupo 6: utilidades -------------------------------------------\n";
    ejecutar("6.1 Cronometro monotono", prueba_cronometroMide);
    ejecutar("6.2 Estadistica muestral", prueba_estadistica);
    ejecutar("6.3 FormatoTabla", prueba_formatoTabla);

    std::cout << "\n------------------------------------------------------------------\n";
    std::cout << pruebasSuperadas << "/" << totalPruebas << " pruebas superadas\n";
    if (!fallos.empty()) {
        std::cout << fallos.size() << " asercion(es) fallida(s)\n";
    }
    std::cout << "------------------------------------------------------------------\n";

    return fallos.empty() ? 0 : 1;
}
