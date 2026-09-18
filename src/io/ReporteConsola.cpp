#include "io/ReporteConsola.h"

#include <algorithm>
#include <iomanip>
#include <locale>
#include <sstream>

#include "io/FormatoTabla.h"

namespace rec {
namespace {

// Numero con punto decimal y precision fija.
//
// Se fuerza la configuracion regional "C": si se usara la del sistema, en una
// maquina en espanol el separador decimal seria una COMA. En pantalla no
// importaria, pero el mismo helper alimenta el CSV, y ahi una coma parte el
// campo. Es el defecto DEF-001 del Proyecto 3, evitado de raiz.
std::string decimal(double valor, int precision) {
    std::ostringstream out;
    out.imbue(std::locale::classic());
    out << std::fixed << std::setprecision(precision) << valor;
    return out.str();
}

// Tamano legible: 1536 bytes -> "1,5 KB".
std::string tamanoLegible(unsigned long long bytes) {
    const char* unidades[] = {"B", "KB", "MB", "GB", "TB"};
    double valor = static_cast<double>(bytes);
    int u = 0;
    while (valor >= 1024.0 && u < 4) {
        valor /= 1024.0;
        ++u;
    }
    return decimal(valor, u == 0 ? 0 : 1) + " " + unidades[u];
}

// Barra de ocupacion en ASCII: [#########.........] 47.3 %
std::string barra(double porcentaje, int ancho) {
    const int llenos = static_cast<int>((porcentaje / 100.0) * ancho + 0.5);
    std::string s = "[";
    for (int i = 0; i < ancho; ++i) s += (i < llenos ? '#' : '.');
    s += "]";
    return s;
}

} // namespace

// "rw-" -> "leer y escribir"; "---" -> "ningun permiso".
std::string permisosDeUnGrupo(char r, char w, char x) {
    std::vector<std::string> tiene;
    if (r == 'r') tiene.push_back("leer");
    if (w == 'w') tiene.push_back("escribir");
    if (x == 'x') tiene.push_back("ejecutar");

    if (tiene.empty()) return "ningun permiso";
    if (tiene.size() == 1) return tiene[0];

    std::string texto = tiene[0];
    for (std::size_t i = 1; i + 1 < tiene.size(); ++i) texto += ", " + tiene[i];
    return texto + " y " + tiene.back();
}

std::string permisosEnPalabras(const std::string& permisos) {
    if (permisos.size() != 9) return "formato de permisos no reconocido";

    // Los tres grupos son iguales muy a menudo --en Windows casi siempre--, y
    // repetir la misma frase tres veces estorba mas de lo que informa.
    const std::string dueno = permisosDeUnGrupo(permisos[0], permisos[1], permisos[2]);
    const std::string grupo = permisosDeUnGrupo(permisos[3], permisos[4], permisos[5]);
    const std::string otros = permisosDeUnGrupo(permisos[6], permisos[7], permisos[8]);

    if (dueno == grupo && grupo == otros) return "todos pueden " + dueno;
    return "dueno: " + dueno + "; grupo: " + grupo + "; otros: " + otros;
}

void imprimirEncabezado(std::ostream& salida, const std::string& directorioTrabajo) {
    salida << "==================================================================\n";
    salida << " Administrador Simplificado de Recursos -- Proyecto 4\n";
    salida << " Universidad Invenio, TIIT2007 Sistemas Operativos\n";
    salida << " Fabrizio Espinoza Arce\n";
    salida << "==================================================================\n";
    salida << " Plataforma          : " << nombrePlataforma() << "\n";
    salida << " Comando de procesos : " << comandoDeProcesos() << "\n";
    salida << " Directorio de trabajo: " << directorioTrabajo << "\n";
}

void imprimirArchivos(std::ostream& salida, const std::vector<MetadatosArchivo>& archivos,
                      const std::string& directorio) {
    salida << "\n-- Archivos en el directorio de trabajo (RF-1) -------------------\n";
    salida << "   " << directorio << "\n\n";

    if (archivos.empty()) {
        salida << "   (el directorio esta vacio)\n";
        return;
    }

    FormatoTabla tabla;
    tabla.agregarColumna("Nombre", Alineacion::Izquierda);
    tabla.agregarColumna("Tamano", Alineacion::Derecha);
    tabla.agregarColumna("Modificado", Alineacion::Izquierda);
    tabla.agregarColumna("Permisos", Alineacion::Izquierda);

    unsigned long long total = 0;
    for (const MetadatosArchivo& m : archivos) {
        total += m.tamanoBytes;
        tabla.agregarFila({m.esDirectorio ? "[dir] " + m.nombre : m.nombre,
                           m.esDirectorio ? "-" : tamanoLegible(m.tamanoBytes),
                           m.fechaModificacion,
                           m.permisos});
    }
    salida << tabla.render();
    salida << "\n   " << archivos.size() << " entradas, " << tamanoLegible(total) << " en total\n";
    // Leyenda: la notacion de nueve letras no se entiende sola si uno no viene
    // de Unix, y esta herramienta la muestra en cada fila.
    salida << "   Permisos: r leer, w escribir, x ejecutar; en tres grupos,\n";
    salida << "   dueno / grupo / otros. Un guion es un permiso que no esta.\n";
}

void imprimirMetadatos(std::ostream& salida, const MetadatosArchivo& m) {
    salida << "\n-- Metadatos de '" << m.nombre << "' ---------------------------\n\n";
    salida << "   Tipo       : " << (m.esDirectorio ? "directorio" : "archivo") << "\n";
    salida << "   Tamano     : " << m.tamanoBytes << " bytes (" << tamanoLegible(m.tamanoBytes) << ")\n";
    salida << "   Modificado : " << m.fechaModificacion << "\n";
    salida << "   Permisos   : " << m.permisos << "\n";
    // "Segun estas letras" y no a secas: en Windows los nueve caracteres son
    // una aproximacion del estandar de C++, no los permisos reales, y la nota
    // de abajo lo explica. Decir "todos pueden escribir" sin ese matiz seria
    // enganoso para quien no conoce el detalle.
    salida << "                (segun estas letras, " << permisosEnPalabras(m.permisos) << ")\n";
    salida << "\n";
    salida << "   Nota sobre los permisos: en Linux los nueve bits son los\n";
    salida << "   reales del inodo, y un archivo nuevo sale como rw-r--r--. En\n";
    salida << "   Windows no existe ese modelo --se usan listas de control de\n";
    salida << "   acceso-- y la biblioteca estandar de C++ los reporta todos\n";
    salida << "   activos (rwxrwxrwx), salvo que el archivo sea de solo lectura.\n";
}

std::size_t sinLimite() { return static_cast<std::size_t>(-1); }

void ordenarPorMemoria(std::vector<ProcesoInfo>& procesos) {
    // Orden descendente por memoria: en una lista de varios cientos, los que
    // consumen mas son los que interesan a quien monitorea.
    std::sort(procesos.begin(), procesos.end(),
              [](const ProcesoInfo& a, const ProcesoInfo& b) {
                  return a.memoriaKb > b.memoriaKb;
              });
}

namespace {

// Tabla de PID, nombre y memoria para un tramo de la lista ya ordenada.
FormatoTabla tablaDeProcesos(const std::vector<ProcesoInfo>& ordenados,
                             std::size_t desde, std::size_t hasta) {
    FormatoTabla tabla;
    tabla.agregarColumna("PID", Alineacion::Derecha);
    tabla.agregarColumna("Proceso", Alineacion::Izquierda);
    tabla.agregarColumna("Memoria", Alineacion::Derecha);
    for (std::size_t i = desde; i < hasta && i < ordenados.size(); ++i) {
        tabla.agregarFila({std::to_string(ordenados[i].pid),
                           ordenados[i].nombre,
                           tamanoLegible(ordenados[i].memoriaKb * 1024ULL)});
    }
    return tabla;
}

unsigned long long memoriaSumada(const std::vector<ProcesoInfo>& procesos) {
    unsigned long long total = 0;
    for (const ProcesoInfo& p : procesos) total += p.memoriaKb;
    return total;
}

// La suma de las memorias por proceso NO es la memoria usada del sistema: el
// working set de cada proceso incluye las paginas compartidas --las
// bibliotecas del sistema-- y por eso se cuentan tantas veces como procesos
// las usen. Se dice aqui para que nadie lea esa cifra como si fuera la del
// sistema, que es la de la opcion 3 del menu.
void imprimirTotales(std::ostream& salida, const std::vector<ProcesoInfo>& procesos) {
    salida << "\n   " << procesos.size() << " procesos activos en total\n";
    salida << "   Memoria sumada de todos: "
           << tamanoLegible(memoriaSumada(procesos) * 1024ULL)
           << " (cuenta varias veces las paginas compartidas)\n";
}

} // namespace

std::size_t imprimirPaginaProcesos(std::ostream& salida,
                                   const std::vector<ProcesoInfo>& ordenados,
                                   std::size_t pagina, std::size_t porPagina) {
    if (porPagina == 0) porPagina = 1;
    const std::size_t totalPaginas =
        ordenados.empty() ? 1 : (ordenados.size() + porPagina - 1) / porPagina;
    if (pagina >= totalPaginas) pagina = totalPaginas - 1;

    const std::size_t desde = pagina * porPagina;
    const std::size_t hasta = desde + porPagina;

    salida << "\n-- Procesos activos (RF-2) -- pagina " << (pagina + 1) << " de "
           << totalPaginas << " ------------------\n\n";
    salida << tablaDeProcesos(ordenados, desde, hasta).render();
    salida << "\n   Puestos " << (ordenados.empty() ? 0 : desde + 1) << " a "
           << std::min(hasta, ordenados.size()) << ", ordenados por memoria\n";
    imprimirTotales(salida, ordenados);
    return totalPaginas;
}

void imprimirProcesos(std::ostream& salida, std::vector<ProcesoInfo> procesos,
                      std::size_t limite) {
    salida << "\n-- Procesos activos (RF-2) ---------------------------------------\n\n";

    const std::size_t totalProcesos = procesos.size();

    ordenarPorMemoria(procesos);
    salida << tablaDeProcesos(procesos, 0, limite).render();

    if (totalProcesos > limite) {
        salida << "\n   Se muestran los " << limite << " de mayor consumo\n";
    }
    imprimirTotales(salida, procesos);
}

void imprimirMemoria(std::ostream& salida, const InfoMemoria& memoria) {
    salida << "\n-- Memoria del sistema (RF-3) ------------------------------------\n\n";

    FormatoTabla tabla;
    tabla.agregarColumna("Metrica", Alineacion::Izquierda);
    tabla.agregarColumna("Valor", Alineacion::Derecha);
    tabla.agregarColumna("Porcentaje", Alineacion::Derecha);

    tabla.agregarFila({"Total", tamanoLegible(memoria.totalKb * 1024ULL), "100.0 %"});
    tabla.agregarFila({"Usada", tamanoLegible(memoria.usadaKb() * 1024ULL),
                       decimal(memoria.porcentajeUsado(), 1) + " %"});
    tabla.agregarFila({"Disponible", tamanoLegible(memoria.disponibleKb * 1024ULL),
                       decimal(100.0 - memoria.porcentajeUsado(), 1) + " %"});
    salida << tabla.render();

    salida << "\n   Ocupacion: " << barra(memoria.porcentajeUsado(), 40) << " "
           << decimal(memoria.porcentajeUsado(), 1) << " %\n";
}

void imprimirMediciones(std::ostream& salida, const std::vector<Medicion>& mediciones) {
    salida << "\n-- Rendimiento de las operaciones de archivo (RE-1) --------------\n\n";

    FormatoTabla tabla;
    tabla.agregarColumna("Operacion", Alineacion::Izquierda);
    tabla.agregarColumna("Archivos", Alineacion::Derecha);
    tabla.agregarColumna("ms (media +- s)", Alineacion::Derecha);
    tabla.agregarColumna("ms/archivo", Alineacion::Derecha);

    for (const Medicion& m : mediciones) {
        tabla.agregarFila({m.operacion,
                           std::to_string(m.cantidad),
                           decimal(m.media(), 3) + " +- " + decimal(m.desviacion(), 3),
                           decimal(m.msPorArchivo(), 4)});
    }
    salida << tabla.render();

    // Aviso de fiabilidad. Si la dispersion es del orden de la media, la
    // medicion no sostiene ninguna comparacion, y decirlo es mas honesto que
    // publicar el numero a secas.
    const bool hayDudosa = std::any_of(mediciones.begin(), mediciones.end(),
                                       [](const Medicion& m) { return m.medicionDudosa(); });
    if (hayDudosa) {
        salida << "\n   AVISO: en alguna fila la desviacion supera la mitad de la media.\n";
        salida << "   A esa escala el tiempo esta dominado por el ruido del sistema y\n";
        salida << "   no por la operacion. Las filas de mas archivos son las fiables.\n";
    }
}

void imprimirAutoconsumo(std::ostream& salida, const ProcesoInfo& propio,
                         const InfoMemoria& memoria) {
    salida << "\n-- Consumo de la propia herramienta (RE-3) -----------------------\n\n";

    const double porcentaje =
        memoria.totalKb == 0 ? 0.0
                             : 100.0 * static_cast<double>(propio.memoriaKb) /
                                   static_cast<double>(memoria.totalKb);

    FormatoTabla tabla;
    tabla.agregarColumna("Metrica", Alineacion::Izquierda);
    tabla.agregarColumna("Valor", Alineacion::Derecha);

    tabla.agregarFila({"PID de la herramienta", std::to_string(propio.pid)});
    tabla.agregarFila({"Memoria residente", tamanoLegible(propio.memoriaKb * 1024ULL)});
    tabla.agregarFila({"Sobre la memoria del sistema", decimal(porcentaje, 3) + " %"});
    salida << tabla.render();

    salida << "\n   Se mide con el mismo mecanismo que el resto de procesos,\n";
    salida << "   filtrando por el PID propio: no hace falta codigo adicional.\n";
}

void imprimirAutoconsumoActual(std::ostream& salida) {
    const unsigned long mio = pidPropio();

    ProcesoInfo propio;
    propio.pid = mio;
    propio.nombre = "(esta herramienta)";

    for (const ProcesoInfo& p : listarProcesos()) {
        if (p.pid == mio) {
            propio = p;
            break;
        }
    }
    imprimirAutoconsumo(salida, propio, consultarMemoria());
}

} // namespace rec
