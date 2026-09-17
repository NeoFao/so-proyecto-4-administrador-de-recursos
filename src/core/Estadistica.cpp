#include "core/Estadistica.h"

#include <cmath>

namespace rec {

double media(const std::vector<double>& valores) {
    if (valores.empty()) return 0.0;

    double suma = 0.0;
    for (double v : valores) suma += v;
    return suma / static_cast<double>(valores.size());
}

double desviacionEstandar(const std::vector<double>& valores) {
    if (valores.size() < 2) return 0.0;

    const double m = media(valores);

    double sumaCuadrados = 0.0;
    for (double v : valores) {
        const double d = v - m;
        sumaCuadrados += d * d;
    }
    // Divisor n-1 (estimador muestral insesgado): las repeticiones son una
    // muestra del comportamiento posible, no la poblacion completa.
    return std::sqrt(sumaCuadrados / static_cast<double>(valores.size() - 1));
}

} // namespace rec
