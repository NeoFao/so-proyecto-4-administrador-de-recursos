#!/usr/bin/env python3
"""Normaliza a CRLF los finales de linea de todos los .bat del proyecto.

POR QUE EXISTE ESTE SCRIPT
==========================

cmd.exe NO resuelve `call :etiqueta` en un fichero .bat cuyos finales de linea
sean LF (estilo Unix). El sintoma es:

    El sistema no encuentra la etiqueta por lotes especificada: localizar_vs

...aunque la etiqueta este escrita correctamente unas lineas mas abajo.

Lo que hace este fallo dificil de diagnosticar es que `goto :etiqueta` SI
funciona con finales LF. Solo falla `call`. Es decir: un script que salta con
`goto` parece perfectamente sano, y solo se rompe el que usa subrutinas --que
en este proyecto es `verificar_compatibilidad.bat`, el que verifica las seis
configuraciones--.

Se descubrio en la Fase 0 del Proyecto 3, y se arrastra aqui como prevencion y esta documentado como "trampa 8" en
la seccion 1 de docs/PLAN_Proyecto3_Memoria.md.

COMO COMPROBARLO A MANO
=======================

No sirve `grep -c $'\\r'` desde Git Bash: en la practica dio un falso positivo
sobre un fichero que era LF puro. Tampoco sirve `sed ... | od -c`, porque sed
elimina el \\r al imprimir. Lo fiable es contar los bytes:

    d = open(ruta, "rb").read()
    print(d.count(b"\\r\\n"), d.count(b"\\n") - d.count(b"\\r\\n"))

USO
===

    python scripts/normalizar_bat_crlf.py

Devuelve 0 siempre que la normalizacion termine bien (haya cambiado ficheros o
no), para poder encadenarlo antes de la verificacion de compatibilidad.

Nota de estilo: las rutas van con prefijo r"..." -- sin el, secuencias como
\\v o \\n dentro de un nombre de carpeta se interpretarian como escapes. Fue un
defecto real del Proyecto 2, donde "\\verificar" se convirtio en una tabulacion
vertical y corrompio un documento.
"""

import pathlib
import sys


def raiz_del_proyecto() -> pathlib.Path:
    """Carpeta del proyecto = la que contiene a scripts/.

    Se deduce del propio fichero en vez de recibirla como argumento para que
    el script funcione igual invocado desde la raiz o desde scripts/.
    """
    return pathlib.Path(__file__).resolve().parent.parent


def normalizar(bat: pathlib.Path) -> bool:
    """Deja `bat` con finales CRLF. Devuelve True si hubo que cambiarlo.

    Se hace en dos pasos --primero todo a LF, luego todo a CRLF-- para que un
    fichero con finales MEZCLADOS quede coherente. Convertir LF a CRLF de una
    sola pasada sobre un fichero mixto produciria \\r\\r\\n donde ya habia CRLF.
    """
    datos = bat.read_bytes()
    normalizado = datos.replace(b"\r\n", b"\n").replace(b"\n", b"\r\n")
    if normalizado == datos:
        return False
    bat.write_bytes(normalizado)
    return True


def main() -> int:
    raiz = raiz_del_proyecto()
    ficheros = sorted(raiz.rglob("*.bat"))

    if not ficheros:
        print("No se encontro ningun .bat bajo", raiz)
        return 0

    convertidos = 0
    for bat in ficheros:
        cambiado = normalizar(bat)
        convertidos += int(cambiado)
        estado = "CONVERTIDO a CRLF" if cambiado else "ya estaba CRLF"
        print("  %-44s %s" % (bat.relative_to(raiz), estado))

    print()
    print("ficheros revisados: %d, convertidos: %d" % (len(ficheros), convertidos))
    return 0


if __name__ == "__main__":
    sys.exit(main())
