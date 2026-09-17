#include "plataforma/Archivos.h"

#include <algorithm>
#include <cerrno>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <system_error>

namespace fs = std::filesystem;

namespace rec {
namespace {

// Verdadero si el codigo de error es "acceso denegado".
//
// Se compara contra std::errc y no contra el numero crudo: en Windows el
// sistema devuelve ERROR_ACCESS_DENIED (5) y en Linux EACCES (13) o EPERM
// (1). La categoria generica de <system_error> traduce los tres al mismo
// valor portable, asi que la comparacion vale en las dos plataformas.
bool esPermisoDenegado(const std::error_code& ec) {
    return ec == std::errc::permission_denied ||
           ec == std::errc::operation_not_permitted;
}

// Mensaje de error del sistema de ficheros, con el caso de permisos
// insuficientes dicho con todas las letras.
//
// El requisito no funcional 1 nombra tres casos: archivo inexistente,
// PERMISOS INSUFICIENTES y comando no disponible. Sin esta funcion el segundo
// llegaba al usuario como el texto crudo del sistema ("Access is denied"),
// pegado a una frase que no tenia nada que ver con permisos.
std::string explicar(const std::error_code& ec) {
    if (esPermisoDenegado(ec)) {
        return "permisos insuficientes (" + ec.message() + ")";
    }
    return ec.message();
}

// Traduce los permisos de <filesystem> a la notacion de nueve letras de Unix.
//
// NOTA DE HONESTIDAD, y conviene decirla en la defensa. Los nueve bits NO
// significan lo mismo en los dos sistemas, y se comprobo creando el mismo
// archivo en ambos:
//
//   Linux    ->  rw-r--r--   son los bits reales del inodo, respetando umask
//   Windows  ->  rwxrwxrwx   la biblioteca estandar los reporta todos activos
//
// La razon es que Windows no usa el modelo de Unix: usa listas de control de
// acceso (ACL), que son mucho mas ricas y no caben en nueve bits. <filesystem>
// no intenta traducirlas y devuelve el permiso mas amplio, salvo que el
// archivo tenga el atributo de solo lectura.
//
// O sea: en Linux el dato es exacto; en Windows es una aproximacion del
// estandar de C++, no un defecto de esta herramienta. Se declara en el
// documento y en la propia salida del programa.
std::string permisosATexto(fs::perms p) {
    auto bit = [p](fs::perms mascara, char letra) {
        return (p & mascara) != fs::perms::none ? letra : '-';
    };
    std::string s;
    s += bit(fs::perms::owner_read,  'r');
    s += bit(fs::perms::owner_write, 'w');
    s += bit(fs::perms::owner_exec,  'x');
    s += bit(fs::perms::group_read,  'r');
    s += bit(fs::perms::group_write, 'w');
    s += bit(fs::perms::group_exec,  'x');
    s += bit(fs::perms::others_read, 'r');
    s += bit(fs::perms::others_write,'w');
    s += bit(fs::perms::others_exec, 'x');
    return s;
}

// Convierte la fecha de modificacion a texto legible.
//
// POR QUE ES MAS ENREDADO DE LO QUE PARECE: file_time_type usa un reloj propio
// del sistema de ficheros, que NO es system_clock. C++20 trae clock_cast para
// convertirlo; en C++17 no existe, asi que se estima la diferencia entre los
// dos relojes tomando "ahora" en ambos y aplicando ese desfase. Es la solucion
// habitual en C++17 y tiene una precision de milisegundos, de sobra para una
// fecha de modificacion.
std::string fechaATexto(fs::file_time_type tiempo) {
    using namespace std::chrono;

    // La resta da una duracion, y sumarla a system_clock::now() la lleva al
    // reloj del sistema. Es el idiom habitual en C++17; en C++20 existe
    // clock_cast y esto sobra.
    const auto puntoSistema = time_point_cast<system_clock::duration>(
        tiempo - fs::file_time_type::clock::now() + system_clock::now());

    const std::time_t t = system_clock::to_time_t(puntoSistema);

    std::tm tm{};
#ifdef _WIN32
    // localtime_s es la version de MSVC con comprobacion de limites; el
    // localtime clasico esta marcado como inseguro y con /W4 avisa, y este
    // proyecto exige cero advertencias.
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    char buffer[32];
    if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm) == 0) {
        return "(fecha no disponible)";
    }
    return buffer;
}

} // namespace

GestorArchivos::GestorArchivos(const std::string& directorioTrabajo) {
    const fs::path raiz = fs::absolute(directorioTrabajo);

    std::error_code ec;
    if (fs::exists(raiz, ec) && !fs::is_directory(raiz, ec)) {
        throw std::runtime_error(
            "'" + raiz.string() + "' existe pero no es un directorio; no puede"
            " usarse como directorio de trabajo");
    }

    fs::create_directories(raiz, ec);
    if (ec) {
        throw std::runtime_error(
            "no se pudo crear el directorio de trabajo '" + raiz.string() +
            "': " + ec.message());
    }

    // Se guarda ya normalizado y en forma canonica, para que la comprobacion
    // de frontera compare siempre contra la misma cadena.
    directorioTexto_ = fs::weakly_canonical(raiz, ec).string();
    if (ec || directorioTexto_.empty()) directorioTexto_ = raiz.string();
}

std::string GestorArchivos::resolverSeguro(const std::string& nombre) const {
    if (nombre.empty()) {
        throw std::invalid_argument("el nombre de archivo no puede estar vacio");
    }

    std::error_code ec;
    const fs::path raiz = fs::path(directorioTexto_);
    const fs::path destino = fs::weakly_canonical(raiz / nombre, ec);
    if (ec) {
        if (esPermisoDenegado(ec)) {
            throw std::runtime_error(
                "no se pudo acceder a '" + nombre + "': " + explicar(ec) +
                ". Compruebe los permisos de " + directorioTexto_);
        }
        throw std::invalid_argument(
            "no se pudo interpretar el nombre '" + nombre + "': " + ec.message());
    }

    // LA COMPROBACION DE FRONTERA.
    //
    // Se compara elemento a elemento y no con "empieza por": comparar cadenas
    // dejaria pasar un directorio hermano cuyo nombre empiece igual, como
    // "trabajo_otro" frente a "trabajo". Recorrer los componentes es exacto.
    auto itRaiz = raiz.begin();
    auto itDest = destino.begin();
    for (; itRaiz != raiz.end(); ++itRaiz, ++itDest) {
        if (itDest == destino.end() || *itRaiz != *itDest) {
            throw std::invalid_argument(
                "'" + nombre + "' apunta fuera del directorio de trabajo (" +
                directorioTexto_ + "). La herramienta solo opera dentro de esa"
                " carpeta");
        }
    }

    // El nombre debe designar algo DENTRO de la raiz, no la raiz misma.
    //
    // DEFECTO REAL (DEF-006): nombres como "." o "sub/.." pasaban la
    // comprobacion anterior, porque la raiz es prefijo de si misma. Con el
    // directorio vacio, "eliminar ." borraba el propio directorio de trabajo y
    // la herramienta se quedaba sin area sobre la que operar. Se exige que
    // quede al menos un componente no vacio despues de los de la raiz.
    // (weakly_canonical puede dejar un componente vacio final si la ruta
    // acaba en separador; ese no cuenta.)
    bool hayComponentePropio = false;
    for (; itDest != destino.end(); ++itDest) {
        if (!itDest->empty() && *itDest != ".") {
            hayComponentePropio = true;
            break;
        }
    }
    if (!hayComponentePropio) {
        throw std::invalid_argument(
            "'" + nombre + "' designa el propio directorio de trabajo, no un"
            " archivo dentro de el");
    }
    return destino.string();
}

void GestorArchivos::crear(const std::string& nombre, const std::string& contenido) {
    const std::string ruta = resolverSeguro(nombre);

    std::error_code ec;
    if (fs::exists(ruta, ec)) {
        throw std::invalid_argument(
            "ya existe '" + nombre + "' en el directorio de trabajo; elija otro"
            " nombre o eliminelo primero");
    }

    errno = 0;
    std::ofstream salida(ruta, std::ios::binary);
    if (!salida) {
        // ofstream no devuelve un error_code, pero la biblioteca de C que usa
        // por debajo deja la causa en errno tanto en MSVC como en glibc.
        const int causa = errno;
        if (causa == EACCES || causa == EPERM) {
            throw std::runtime_error(
                "no se pudo crear '" + nombre + "': permisos insuficientes para"
                " escribir en " + directorioTexto_);
        }
        throw std::runtime_error(
            "no se pudo crear '" + nombre + "' en " + directorioTexto_ +
            (causa != 0 ? ": " + std::generic_category().message(causa)
                        : std::string(". Puede ser falta de permisos de escritura")));
    }
    salida << contenido;
    if (!salida) {
        throw std::runtime_error("fallo la escritura de '" + nombre + "'");
    }
}

void GestorArchivos::eliminar(const std::string& nombre) {
    const std::string ruta = resolverSeguro(nombre);

    std::error_code ec;
    if (!fs::exists(ruta, ec)) {
        throw std::invalid_argument(
            "no existe '" + nombre + "' en el directorio de trabajo");
    }

    // remove y no remove_all: borrar un directorio con contenido de forma
    // recursiva es una operacion destructiva que el enunciado no pide, y
    // hacerla en silencio seria peligroso.
    if (!fs::remove(ruta, ec) || ec) {
        throw std::runtime_error(
            "no se pudo eliminar '" + nombre + "': " +
            (ec ? explicar(ec) : std::string("puede estar en uso o ser un"
                                             " directorio con contenido")));
    }
}

std::vector<MetadatosArchivo> GestorArchivos::listar() const {
    std::vector<MetadatosArchivo> entradas;

    std::error_code ec;
    for (const fs::directory_entry& e : fs::directory_iterator(directorioTexto_, ec)) {
        MetadatosArchivo m;
        m.nombre = e.path().filename().string();
        m.esDirectorio = e.is_directory(ec);
        m.tamanoBytes = m.esDirectorio ? 0 : static_cast<unsigned long long>(e.file_size(ec));
        m.fechaModificacion = fechaATexto(e.last_write_time(ec));
        m.permisos = permisosATexto(e.status(ec).permissions());
        entradas.push_back(m);
    }
    if (ec) {
        throw std::runtime_error(
            "no se pudo listar '" + directorioTexto_ + "': " + explicar(ec));
    }

    // Orden alfabetico estable: sin esto el orden lo decide el sistema de
    // ficheros y dos ejecuciones seguidas pueden mostrar listas distintas, lo
    // que haria la evidencia experimental no reproducible.
    std::sort(entradas.begin(), entradas.end(),
              [](const MetadatosArchivo& a, const MetadatosArchivo& b) {
                  return a.nombre < b.nombre;
              });
    return entradas;
}

MetadatosArchivo GestorArchivos::metadatos(const std::string& nombre) const {
    const std::string ruta = resolverSeguro(nombre);

    std::error_code ec;
    if (!fs::exists(ruta, ec)) {
        throw std::invalid_argument(
            "no existe '" + nombre + "' en el directorio de trabajo");
    }

    const fs::directory_entry e{ruta, ec};
    MetadatosArchivo m;
    m.nombre = nombre;
    m.esDirectorio = e.is_directory(ec);
    m.tamanoBytes = m.esDirectorio ? 0 : static_cast<unsigned long long>(e.file_size(ec));
    m.fechaModificacion = fechaATexto(e.last_write_time(ec));
    m.permisos = permisosATexto(e.status(ec).permissions());
    return m;
}

std::size_t GestorArchivos::cuantos() const {
    std::error_code ec;
    std::size_t total = 0;
    for (const auto& e : fs::directory_iterator(directorioTexto_, ec)) {
        (void)e;
        ++total;
    }
    return total;
}

} // namespace rec
