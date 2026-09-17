#ifndef CORE_CRONOMETRO_H
#define CORE_CRONOMETRO_H

#include <chrono>

// Medicion de tiempos para los resultados experimentales.
//
// Vive en src/core/ porque es logica pura: no consulta al sistema operativo ni
// escribe nada. Solo lee el reloj.
//
// POR QUE steady_clock Y NO system_clock: system_clock puede saltar hacia
// atras si el sistema ajusta la hora --por NTP, por cambio de horario-- y una
// duracion negativa arruinaria la medicion sin que nada avisara. steady_clock
// esta garantizado como monotono: nunca retrocede. Es la eleccion correcta
// para medir intervalos, y la misma que se uso en el Proyecto 3.
namespace rec {

class Cronometro {
public:
    Cronometro() : inicio_(std::chrono::steady_clock::now()) {}

    void reiniciar() { inicio_ = std::chrono::steady_clock::now(); }

    double milisegundos() const {
        const auto ahora = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::milli>(ahora - inicio_).count();
    }

private:
    std::chrono::steady_clock::time_point inicio_;
};

} // namespace rec
#endif
