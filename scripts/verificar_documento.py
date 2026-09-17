#!/usr/bin/env python3
"""Verifica la coherencia interna del documento IEEE.

Administrador Simplificado de Recursos (Proyecto 4) -- Fabrizio Espinoza Arce

===========================================================================
POR QUE ESTE SCRIPT
===========================================================================

En el Proyecto 2 el documento se entrego con tres referencias declaradas en la
seccion 10 que nunca se citaban en el cuerpo, y con cinco tablas sin numerar.
Ninguna de las dos cosas se detecta leyendo el documento a ojo: son
exactamente el tipo de error que una maquina encuentra en un segundo y una
persona no encuentra nunca.

COMPRUEBA SEIS COSAS:

  1. Correspondencia bidireccional de citas. Toda referencia [n] declarada en
     la seccion 10 debe citarse en el cuerpo, y toda cita del cuerpo debe
     existir en la seccion 10.
  2. Toda tabla numerada debe referenciarse desde el texto, y toda referencia
     a una tabla debe corresponder a una tabla existente.
  3. Lo mismo para las figuras.
  4. Los bloques de codigo no pasan de 50 caracteres por linea (a dos columnas
     no caben mas).
  5. Las 10 secciones de la plantilla estan y en orden.
  6. Cada referencia lleva su ano entre parentesis, como exige el formato de
     la plantilla institucional: "[1] Apellido, Inicial. (Ano). Titulo.
     Editorial." En el Proyecto 4 se entrego una version sin anos y este
     script no lo detecto, porque no lo comprobaba.

Devuelve 0 si todo esta bien, 1 si hay algun problema, para poder encadenarlo
en el script de experimentos.

Nota de estilo: las rutas van con prefijo r"..." -- sin el, secuencias como
\\v o \\n dentro de un nombre de carpeta se interpretarian como escapes.
"""

import pathlib
import re
import sys

LIMITE_ANCHO_CODIGO = 50


def cargar(ruta):
    return pathlib.Path(ruta).read_text(encoding="utf-8")


def separar_cuerpo_y_referencias(texto):
    """Divide el documento en cuerpo y seccion 10."""
    marca = re.search(r"^##\s*10\.\s*Referencias", texto, re.MULTILINE)
    if not marca:
        return texto, ""
    return texto[: marca.start()], texto[marca.start():]


def quitar_bloques_de_codigo(texto):
    """Elimina los bloques ``` para no confundir su contenido con citas."""
    return re.sub(r"```.*?```", "", texto, flags=re.DOTALL)


def comprobar_citas(cuerpo, referencias, problemas):
    declaradas = set(int(n) for n in re.findall(r"^\[(\d+)\]", referencias, re.MULTILINE))
    citadas = set(int(n) for n in re.findall(r"\[(\d+)\]", quitar_bloques_de_codigo(cuerpo)))

    print("  Referencias declaradas en la seccion 10: %s" % sorted(declaradas))
    print("  Referencias citadas en el cuerpo       : %s" % sorted(citadas))

    huerfanas = declaradas - citadas
    fantasmas = citadas - declaradas

    if huerfanas:
        problemas.append(
            "referencias declaradas pero NUNCA citadas en el cuerpo: %s" % sorted(huerfanas))
    if fantasmas:
        problemas.append(
            "citas en el cuerpo que no existen en la seccion 10: %s" % sorted(fantasmas))
    if len(declaradas) < 3:
        problemas.append(
            "la plantilla exige un minimo de 3 referencias y hay %d" % len(declaradas))


def comprobar_numerados(cuerpo, problemas, clase):
    """Comprueba tablas o figuras: definidas frente a referenciadas."""
    # Definicion: una linea que empieza por **Tabla N.** o **Figura N.**
    definidas = set(int(n) for n in re.findall(
        r"^\*\*%s\s+(\d+)\.\*\*" % clase, cuerpo, re.MULTILINE))
    # Referencia desde el texto: "Tabla N" o "(Tabla N)" en cualquier posicion
    # que NO sea la propia definicion.
    sin_definiciones = re.sub(r"^\*\*%s\s+\d+\.\*\*.*$" % clase, "", cuerpo,
                              flags=re.MULTILINE)
    referenciadas = set(int(n) for n in re.findall(
        r"%s\s+(\d+)" % clase, sin_definiciones))

    print("  %ss definidas    : %s" % (clase, sorted(definidas)))
    print("  %ss referenciadas: %s" % (clase, sorted(referenciadas)))

    sin_mencion = definidas - referenciadas
    inexistentes = referenciadas - definidas

    if sin_mencion:
        problemas.append(
            "%ss numeradas pero nunca referenciadas desde el texto: %s"
            % (clase, sorted(sin_mencion)))
    if inexistentes:
        problemas.append(
            "el texto menciona %ss que no existen: %s" % (clase, sorted(inexistentes)))

    # La numeracion debe ser consecutiva desde 1: un hueco delata una tabla
    # borrada sin renumerar el resto.
    if definidas and sorted(definidas) != list(range(1, len(definidas) + 1)):
        problemas.append(
            "la numeracion de %ss no es consecutiva desde 1: %s" % (clase, sorted(definidas)))


def comprobar_ancho_del_codigo(texto, problemas):
    largas = []
    for bloque in re.findall(r"```[a-zA-Z]*\n(.*?)```", texto, flags=re.DOTALL):
        for linea in bloque.split("\n"):
            if len(linea) > LIMITE_ANCHO_CODIGO:
                largas.append((len(linea), linea))

    if largas:
        problemas.append(
            "%d linea(s) de codigo superan %d caracteres y se desbordaran de la columna"
            % (len(largas), LIMITE_ANCHO_CODIGO))
        for ancho, linea in largas[:5]:
            print("      %d car.: %s" % (ancho, linea))
    else:
        print("  Bloques de codigo: ninguna linea supera %d caracteres" % LIMITE_ANCHO_CODIGO)


def comprobar_anos(referencias, problemas):
    """Cada referencia [n] debe llevar el ano entre parentesis: (AAAA)."""
    entradas = re.findall(r"^\[(\d+)\](.*?)(?=^\[\d+\]|\Z)", referencias,
                          flags=re.MULTILINE | re.DOTALL)
    sin_ano = [int(n) for n, texto in entradas if not re.search(r"\((19|20)\d\d\)", texto)]
    print("  Referencias sin ano entre parentesis: %s" % (sin_ano or "ninguna"))
    if sin_ano:
        problemas.append(
            "referencias sin el ano que exige la plantilla, (AAAA): %s" % sin_ano)


def comprobar_secciones(cuerpo, referencias, problemas):
    """Las 10 secciones de la plantilla institucional deben estar y en orden."""
    completo = cuerpo + referencias
    encontradas = [int(n) for n in re.findall(r"^##\s*(\d+)\.", completo, re.MULTILINE)]
    print("  Secciones encontradas: %s" % encontradas)
    if encontradas != list(range(1, 11)):
        problemas.append(
            "la plantilla exige las secciones 1 a 10 en orden, y se encontro %s" % encontradas)


def main():
    if len(sys.argv) < 2:
        raiz = pathlib.Path(__file__).resolve().parent.parent
        ruta = raiz / "docs" / "documento_ieee_recursos.md"
    else:
        ruta = pathlib.Path(sys.argv[1])

    if not ruta.exists():
        print("ERROR: no existe %s" % ruta)
        return 1

    texto = cargar(ruta)
    cuerpo, referencias = separar_cuerpo_y_referencias(texto)
    problemas = []

    print("Verificacion de %s" % ruta.name)
    print("=" * 66)
    print()
    print("Secciones de la plantilla")
    comprobar_secciones(cuerpo, referencias, problemas)
    print()
    print("Correspondencia bidireccional de citas")
    comprobar_citas(cuerpo, referencias, problemas)
    print()
    print("Formato de las referencias")
    comprobar_anos(referencias, problemas)
    print()
    print("Tablas")
    comprobar_numerados(cuerpo, problemas, "Tabla")
    print()
    print("Figuras")
    comprobar_numerados(cuerpo, problemas, "Figura")
    print()
    print("Ancho de los bloques de codigo")
    comprobar_ancho_del_codigo(texto, problemas)

    print()
    print("=" * 66)
    if problemas:
        print("SE ENCONTRARON %d PROBLEMA(S):" % len(problemas))
        for p in problemas:
            print("  - %s" % p)
        return 1

    print("RESULTADO: el documento es coherente.")
    print("  Citas bidireccionales, tablas y figuras numeradas y referenciadas,")
    print("  bloques de codigo dentro del ancho de columna.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
