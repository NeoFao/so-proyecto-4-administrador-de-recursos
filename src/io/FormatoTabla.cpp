#include "io/FormatoTabla.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace rec {

std::string alinearTexto(const std::string& texto, std::size_t ancho, Alineacion alineacion) {
    if (texto.size() >= ancho) return texto;
    const std::size_t relleno = ancho - texto.size();

    switch (alineacion) {
        case Alineacion::Izquierda:
            return texto + std::string(relleno, ' ');
        case Alineacion::Derecha:
            return std::string(relleno, ' ') + texto;
        case Alineacion::Centro:
        default: {
            // Con relleno impar se pone un espacio extra a la derecha.
            const std::size_t izquierda = relleno / 2;
            const std::size_t derecha = relleno - izquierda;
            return std::string(izquierda, ' ') + texto + std::string(derecha, ' ');
        }
    }
}

void FormatoTabla::agregarColumna(const std::string& encabezado, Alineacion alineacion) {
    encabezados_.push_back(encabezado);
    alineaciones_.push_back(alineacion);
}

void FormatoTabla::agregarFila(const std::vector<std::string>& celdas) {
    if (celdas.size() != encabezados_.size()) {
        throw std::invalid_argument(
            "FormatoTabla::agregarFila: se esperaban " +
            std::to_string(encabezados_.size()) + " celdas, se recibieron " +
            std::to_string(celdas.size()) +
            " (error de programacion del llamador, no de datos de usuario)");
    }
    filas_.push_back(celdas);
}

std::size_t FormatoTabla::numeroColumnas() const { return encabezados_.size(); }

std::vector<std::size_t> FormatoTabla::calcularAnchos() const {
    std::vector<std::size_t> anchos(encabezados_.size());
    for (std::size_t c = 0; c < encabezados_.size(); ++c) {
        anchos[c] = encabezados_[c].size();
    }
    for (const auto& fila : filas_) {
        for (std::size_t c = 0; c < fila.size(); ++c) {
            anchos[c] = std::max(anchos[c], fila[c].size());
        }
    }
    return anchos;
}

std::string FormatoTabla::lineaBorde(const std::vector<std::size_t>& anchos) const {
    std::string linea = "+";
    for (std::size_t a : anchos) {
        linea += std::string(a + 2, '-'); // +2 = un espacio de relleno a cada lado
        linea += "+";
    }
    return linea;
}

std::string FormatoTabla::render() const {
    if (encabezados_.empty()) return "";
    const std::vector<std::size_t> anchos = calcularAnchos();
    const std::string borde = lineaBorde(anchos);

    std::ostringstream out;
    out << borde << "\n";

    out << "|";
    for (std::size_t c = 0; c < encabezados_.size(); ++c) {
        out << " " << alinearTexto(encabezados_[c], anchos[c], Alineacion::Centro) << " |";
    }
    out << "\n" << borde << "\n";

    for (const auto& fila : filas_) {
        out << "|";
        for (std::size_t c = 0; c < fila.size(); ++c) {
            out << " " << alinearTexto(fila[c], anchos[c], alineaciones_[c]) << " |";
        }
        out << "\n";
    }
    out << borde << "\n";
    return out.str();
}

std::size_t FormatoTabla::anchoTotal() const {
    if (encabezados_.empty()) return 0;
    const std::vector<std::size_t> anchos = calcularAnchos();
    std::size_t total = 1;
    for (std::size_t a : anchos) total += a + 3;
    return total;
}

} // namespace rec
