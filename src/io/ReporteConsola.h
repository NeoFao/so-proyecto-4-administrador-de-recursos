#ifndef IO_REPORTECONSOLA_H
#define IO_REPORTECONSOLA_H

#include <ostream>
#include <string>
#include <vector>

#include "core/Medicion.h"
#include "plataforma/Archivos.h"
#include "plataforma/Sistema.h"

// Presentacion en consola de las tres vistas del enunciado y de los
// resultados experimentales.
//
// Todas las funciones reciben el ostream como parametro en lugar de escribir
// en std::cout directamente. Asi la misma vista puede volcarse a un fichero
// sin duplicar codigo, que es lo que hace el script de experimentos para
// generar la evidencia de results/.
namespace rec {

void imprimirEncabezado(std::ostream& salida, const std::string& directorioTrabajo);

// RF-1: listado de archivos con sus metadatos.
void imprimirArchivos(std::ostream& salida, const std::vector<MetadatosArchivo>& archivos,
                      const std::string& directorio);

// RF-1: metadatos de un archivo concreto, en detalle.
void imprimirMetadatos(std::ostream& salida, const MetadatosArchivo& m);

// RF-2: procesos activos. Se ordenan por memoria descendente y se muestran los
// `limite` primeros, porque una lista de 358 procesos sin ordenar no informa
// de nada; los que consumen mas son los que interesan al monitorear.
//
// Para verlos TODOS sin recortar, pase `sinLimite()`.
void imprimirProcesos(std::ostream& salida, std::vector<ProcesoInfo> procesos,
                      std::size_t limite);

// "Sin recorte": el mayor tamano posible, para pedir la lista completa.
std::size_t sinLimite();

// Ordena por memoria descendente. Se expone porque el menu pagina la lista y
// necesita ordenar UNA vez: si reordenara en cada pagina tendria que volver a
// consultar los procesos, y la lista cambiaria entre paginas.
void ordenarPorMemoria(std::vector<ProcesoInfo>& procesos);

// RF-2 paginado: imprime UNA pagina de la lista ya ordenada (pagina 0 es la
// primera) y, debajo, en que pagina esta y los totales. No pide nada por
// teclado: de la navegacion se encarga el menu.
//
// Devuelve el numero total de paginas, que es lo que el menu necesita para
// saber si hay siguiente.
std::size_t imprimirPaginaProcesos(std::ostream& salida,
                                   const std::vector<ProcesoInfo>& ordenados,
                                   std::size_t pagina, std::size_t porPagina);

// RF-3: memoria total, usada y disponible, con barra de ocupacion.
void imprimirMemoria(std::ostream& salida, const InfoMemoria& memoria);

// RE-1: tabla de tiempos por operacion y cantidad de archivos.
void imprimirMediciones(std::ostream& salida, const std::vector<Medicion>& mediciones);

// RE-3: consumo de la propia herramienta.
void imprimirAutoconsumo(std::ostream& salida, const ProcesoInfo& propio,
                         const InfoMemoria& memoria);

// RE-3, buscando los datos por su cuenta: localiza el proceso propio en la
// lista del sistema --por su PID-- y consulta la memoria. Lo usan el menu y el
// modo de argumentos, para que los dos midan exactamente igual.
void imprimirAutoconsumoActual(std::ostream& salida);

// Traduce los nueve caracteres de permisos a palabras, para quien no conoce la
// notacion de Unix: "rw-r--r--" -> "dueno: leer y escribir; grupo: leer; ...".
// Se muestra junto a los permisos para que la herramienta se entienda sin
// tener que saber que significa cada letra.
std::string permisosEnPalabras(const std::string& permisos);

} // namespace rec
#endif
