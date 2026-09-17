#ifndef IO_REPORTECONSOLA_H
#define IO_REPORTECONSOLA_H

#include <ostream>
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
void imprimirProcesos(std::ostream& salida, std::vector<ProcesoInfo> procesos,
                      std::size_t limite);

// RF-3: memoria total, usada y disponible, con barra de ocupacion.
void imprimirMemoria(std::ostream& salida, const InfoMemoria& memoria);

// RE-1: tabla de tiempos por operacion y cantidad de archivos.
void imprimirMediciones(std::ostream& salida, const std::vector<Medicion>& mediciones);

// RE-3: consumo de la propia herramienta.
void imprimirAutoconsumo(std::ostream& salida, const ProcesoInfo& propio,
                         const InfoMemoria& memoria);

} // namespace rec
#endif
