#ifndef IO_FORMATOTABLA_H
#define IO_FORMATOTABLA_H

#include <cstddef>
#include <string>
#include <vector>

// Helper generico de tablas ASCII con bordes "+---+" y "|".
//
// CODIGO REUTILIZADO DE LOS PROYECTOS 1, 2 Y 3 (src/io/FormatoTabla.{h,cpp}), sin
// cambios funcionales; solo se adapto el namespace (`sched` -> `conc` -> `mem` -> `rec`).
// La reutilizacion esta permitida por la seccion "Consideraciones Transversales"
// del enunciado y se declara ademas en la seccion 5 del documento IEEE.
//
// Que sobreviva sin tocarlo a tres proyectos con dominios distintos es la
// prueba de que la separacion nucleo/E-S funciona: esta clase no sabe nada de
// procesos, ni de hilos, ni de marcos de memoria. Solo sabe alinear texto.
namespace rec {

// Alineacion del contenido de una columna dentro de su ancho calculado.
enum class Alineacion {
    Izquierda,
    Derecha,
    Centro
};

// Rellena `texto` con espacios hasta `ancho` segun `alineacion`. Si el texto
// ya es mas ancho que `ancho`, se devuelve tal cual (no trunca: truncar
// datos silenciosamente seria peor que una tabla un poco mas ancha).
std::string alinearTexto(const std::string& texto, std::size_t ancho, Alineacion alineacion);

// Tabla ASCII de ancho de columna automatico. Uso tipico:
//   FormatoTabla tabla;
//   tabla.agregarColumna("Algoritmo", Alineacion::Izquierda);
//   tabla.agregarColumna("Fallos", Alineacion::Derecha);
//   tabla.agregarFila({"FIFO", "15"});
//   salida << tabla.render();
class FormatoTabla {
public:
    void agregarColumna(const std::string& encabezado, Alineacion alineacion = Alineacion::Derecha);

    // Precondicion: celdas.size() == numero de columnas agregadas hasta
    // ahora. Un tamano distinto es un error de programacion del llamador (no
    // un error de datos del usuario), por eso se senala con excepcion.
    void agregarFila(const std::vector<std::string>& celdas);

    // Texto completo de la tabla (encabezado + filas + bordes), terminado en
    // '\n' en cada linea, listo para volcar a un ostream.
    std::string render() const;

    // Ancho total en caracteres de cualquier linea de render().
    std::size_t anchoTotal() const;

    std::size_t numeroColumnas() const;

private:
    std::vector<std::string> encabezados_;
    std::vector<Alineacion> alineaciones_;
    std::vector<std::vector<std::string>> filas_;

    std::vector<std::size_t> calcularAnchos() const;
    std::string lineaBorde(const std::vector<std::size_t>& anchos) const;
};

} // namespace rec
#endif
