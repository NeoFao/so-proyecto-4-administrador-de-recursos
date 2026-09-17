#ifndef CORE_ESTADISTICA_H
#define CORE_ESTADISTICA_H

#include <vector>

// Agregacion estadistica de una serie de mediciones.
//
// CODIGO REUTILIZADO DE LOS PROYECTOS 2 Y 3 (src/core/Estadistica.{h,cpp}). Alli las dos
// funciones vivian dentro de un modulo acoplado al dominio de la concurrencia
// (`ResumenVersion` conocia productores, consumidores y anomalias de buffer).
// Aqui se extrae SOLO la parte que es matematica pura y no sabe nada del
// dominio, que es justamente la parte reutilizable. Se declara en la seccion 5
// del documento IEEE.
//
// POR QUE SE MIDE ASI EN ESTE PROYECTO: el tiempo que tarda una operacion de
// archivo NO es determinista. Depende del planificador del sistema, de la
// cache del sistema de ficheros, del antivirus y de la frecuencia del
// procesador en ese instante. Por eso cada tiempo se repite varias veces y se
// reporta como media +- desviacion, nunca como una medicion suelta.
namespace rec {

// Media aritmetica. Con el vector vacio devuelve 0.0: no hay media que
// calcular, y devolver 0 es preferible a dividir entre cero.
double media(const std::vector<double>& valores);

// Desviacion estandar MUESTRAL (divisor n-1). Con n < 2 devuelve 0.0: una
// sola observacion no tiene dispersion que estimar, y devolver 0 es mas
// honesto que una division por cero disfrazada.
//
// Se usa el divisor n-1 y no n porque las repeticiones son una MUESTRA del
// comportamiento posible de la maquina, no la poblacion completa de todas las
// ejecuciones que podrian darse.
//
// Regla de interpretacion heredada del Proyecto 1: si la desviacion resulta
// mayor que la diferencia entre dos algoritmos, la conclusion honesta es que
// los datos NO sostienen esa diferencia. No se redondea hasta que parezca que
// si.
double desviacionEstandar(const std::vector<double>& valores);

} // namespace rec
#endif
