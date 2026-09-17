#ifndef PLATAFORMA_ARCHIVOS_H
#define PLATAFORMA_ARCHIVOS_H

#include <string>
#include <vector>

// Gestion basica de archivos dentro de un directorio de trabajo controlado
// (requisito funcional 1).
//
// ============================================================================
// POR QUE ESTE MODULO ESTA EN plataforma/ AUNQUE NO TENGA #ifdef
// ============================================================================
//
// src/plataforma/ agrupa todo lo que toca el sistema operativo. Dentro hay dos
// clases de fichero, y conviene no confundirlas al explicar el proyecto:
//
//   Archivos.cpp                UNA sola implementacion, portable, escrita
//                               sobre <filesystem> de C++17. El estandar ya
//                               resuelve las diferencias entre sistemas.
//
//   SistemaWindows/Linux.cpp    DOS implementaciones, una por sistema. No hay
//                               forma estandar de listar procesos ni de
//                               consultar la memoria del anfitrion.
//
// Es decir: la gestion de archivos es portable porque el estandar de C++ la
// cubre; el monitoreo no lo es porque el estandar no lo cubre. Esa es la linea
// que separa lo que hubo que duplicar de lo que no.
//
// ============================================================================
// EL DIRECTORIO DE TRABAJO CONTROLADO
// ============================================================================
//
// El enunciado pide operar "dentro de un directorio de trabajo controlado".
// Eso no es solo una carpeta por omision: es una FRONTERA que hay que hacer
// cumplir. Sin comprobacion, un nombre como "..\..\Windows\System32\algo"
// dejaria que la herramienta escribiera o borrara fuera de su area.
//
// `resolverSeguro` normaliza la ruta y comprueba que siga cayendo dentro de la
// raiz. Es una defensa contra el recorrido de directorios (path traversal), y
// es barata: unas pocas lineas.
namespace rec {

struct MetadatosArchivo {
    std::string nombre;
    unsigned long long tamanoBytes = 0;
    std::string fechaModificacion;   // "AAAA-MM-DD HH:MM:SS"
    std::string permisos;            // estilo Unix: "rw-r--r--"
    bool esDirectorio = false;
};

class GestorArchivos {
public:
    // Crea el directorio de trabajo si no existe.
    // LANZA std::runtime_error si la ruta existe y no es un directorio.
    explicit GestorArchivos(const std::string& directorioTrabajo);

    // LANZA std::invalid_argument si el nombre intenta salir del directorio
    // controlado, o si ya existe un archivo con ese nombre.
    void crear(const std::string& nombre, const std::string& contenido = "");

    // LANZA std::invalid_argument si el archivo no existe o si el nombre sale
    // del directorio controlado.
    void eliminar(const std::string& nombre);

    std::vector<MetadatosArchivo> listar() const;

    // LANZA std::invalid_argument si el archivo no existe.
    MetadatosArchivo metadatos(const std::string& nombre) const;

    // Numero de entradas del directorio de trabajo.
    std::size_t cuantos() const;

    const std::string& directorio() const { return directorioTexto_; }

private:
    std::string directorioTexto_;

    // Devuelve la ruta absoluta y normalizada de `nombre` dentro de la raiz.
    // LANZA std::invalid_argument si el resultado se sale de la raiz.
    std::string resolverSeguro(const std::string& nombre) const;
};

} // namespace rec
#endif
