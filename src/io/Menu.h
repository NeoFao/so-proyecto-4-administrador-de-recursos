#ifndef IO_MENU_H
#define IO_MENU_H

#include <istream>
#include <ostream>
#include <string>

#include "plataforma/Archivos.h"

// Interfaz de consola con menu de navegacion (requisito funcional 4).
//
// ============================================================================
// POR QUE RECIBE LOS FLUJOS EN LUGAR DE USAR cin Y cout
// ============================================================================
//
// El menu no llama a std::cin ni a std::cout: se le pasan por el constructor.
// Eso tiene tres consecuencias practicas:
//
//   1. El menu se puede conducir desde un fichero o desde una tuberia, que es
//      como EJECUTAR.bat demuestra las tres funciones sin que nadie teclee.
//   2. La salida se puede volcar a un fichero para generar evidencia.
//   3. Es probable sin capturar la salida estandar del proceso.
//
// Es el mismo patron que en el Proyecto 3 hacia que la CLI fuera probable:
// separar "decidir que hacer" de "donde escribirlo".
//
// ============================================================================
// MANEJO DE ENTRADA INVALIDA
// ============================================================================
//
// El requisito no funcional 1 pide robustez. En un menu eso significa que
// escribir "abc" donde se espera un numero NO puede colgar el programa ni
// dejarlo en un bucle infinito. Se lee siempre la linea completa y se
// interpreta despues: asi el flujo nunca queda en estado de error con
// caracteres pendientes, que es el fallo clasico de leer con `cin >> entero`.
namespace rec {

class Menu {
public:
    Menu(std::istream& entrada, std::ostream& salida, GestorArchivos& gestor);

    // Bucle principal. Termina cuando el usuario elige salir o cuando la
    // entrada se agota (fin de fichero), lo que permite conducirlo desde un
    // script sin que se quede esperando para siempre.
    void ejecutar();

private:
    std::istream& entrada_;
    std::ostream& salida_;
    GestorArchivos& gestor_;

    void mostrarMenuPrincipal();
    void mostrarMenuArchivos();

    void opcionArchivos();
    void opcionProcesos();
    void opcionMemoria();

    void archivosListar();
    void archivosCrear();
    void archivosMetadatos();
    void archivosEliminar();

    // Lee una linea completa. Devuelve false si la entrada se agoto.
    bool leerLinea(std::string& destino);

    // Pide un dato al usuario mostrando `mensaje`.
    bool pedir(const std::string& mensaje, std::string& destino);

    // Ejecuta `accion` y convierte cualquier excepcion en un mensaje claro.
    //
    // Centralizar el manejo aqui es lo que hace que NINGUNA operacion del menu
    // pueda tumbar el programa: el usuario ve que fallo y vuelve al menu.
    void conManejoDeErrores(const std::string& queSeIntentaba, void (Menu::*accion)());
};

} // namespace rec
#endif
