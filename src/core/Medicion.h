#ifndef CORE_MEDICION_H
#define CORE_MEDICION_H

#include <string>
#include <vector>

#include "core/Estadistica.h"

// Resultado de cronometrar una operacion, agregado sobre varias repeticiones.
//
// Vive en src/core/ porque es un tipo puro: no mide nada por si mismo ni toca
// el sistema. Solo guarda lo medido y sabe resumirlo.
//
// POR QUE SE REPORTA MEDIA Y DESVIACION Y NO UN SOLO NUMERO
//
// Leccion del Proyecto 3: crear diez archivos tarda microsegundos, y medir eso
// una sola vez da ruido del reloj, no un dato. Con repeticiones, la desviacion
// dice ademas SI LA MEDICION ES FIABLE: cuando sigma se acerca a la media, el
// numero no sostiene ninguna comparacion, y el reporte lo advierte en vez de
// publicarlo a secas.
namespace rec {

struct Medicion {
    std::string operacion;      // "crear", "listar", "metadatos", "eliminar"
    std::size_t cantidad = 0;   // cuantos archivos habia (10, 100, 1000)
    int repeticiones = 0;
    std::vector<double> muestras;   // milisegundos de cada repeticion

    double media() const { return rec::media(muestras); }
    double desviacion() const { return rec::desviacionEstandar(muestras); }

    // Milisegundos por archivo. Es la metrica que permite comparar entre
    // cantidades distintas: 100 ms para 1000 archivos es mucho mejor que
    // 100 ms para 10, y el total por si solo no lo deja ver.
    double msPorArchivo() const {
        if (cantidad == 0) return 0.0;
        return media() / static_cast<double>(cantidad);
    }

    // Verdadero cuando la dispersion es tan grande que la media no sostiene
    // una comparacion. El umbral de la mitad es el mismo que se uso en el
    // Proyecto 3.
    bool medicionDudosa() const {
        return media() > 0.0 && desviacion() > media() * 0.5;
    }
};

} // namespace rec
#endif
