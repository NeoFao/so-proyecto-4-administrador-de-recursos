# Administrador Simplificado de Recursos — Proyecto 4

**Fabrizio Espinoza Arce** · TIIT2007 Sistemas Operativos · Universidad Invenio

Herramienta de consola en C++17 que integra **gestión de archivos** y
**monitoreo de procesos y memoria** del sistema anfitrión, con menú de
navegación. Funciona en **Windows y en Linux**, y ambas ramas están verificadas.

---

## Empezar en un comando

Doble clic en `EJECUTAR.bat`, o desde cualquier consola:

```
EJECUTAR.bat
```

Configura Visual Studio por su cuenta, compila, ejecuta las 26 pruebas y recorre
los cuatro requisitos funcionales y los resultados experimentales, uno por uno.
No hace falta abrir la «Developer Command Prompt».

Para usarla de verdad:

```
build\recursos.exe
```

---

## Estado de verificación

| Qué | Windows | Linux | Evidencia |
|---|---|---|---|
| Compilación sin advertencias | Cero | Cero | `results/compatibilidad_linux.txt` |
| Suite de pruebas | **26/26** | **26/26** | `results/pruebas_windows.txt` |
| Las tres funciones del enunciado | Sí | Sí | `results/vistas_menu.txt` |
| Los tres casos de error (inexistente, permisos, comando) | Sí | Sí | `results/errores_manejados.txt` |
| Carga del RE-2 comprobada | Memoria 86 %, CPU 100 % | — | `results/prueba_carga.txt` |

**A diferencia de los Proyectos 2 y 3, aquí la rama de Linux no se declara: se
ejecuta.** El mismo código fuente compila con MSVC y con g++ y pasa la misma
suite en los dos sistemas.

---

## Resultados principales

**Cómo escalan las operaciones** (milisegundos por archivo, Windows):

| Operación | 10 | 100 | 1000 | Escala |
|---|---:|---:|---:|---|
| crear | 0,782 | 0,779 | 0,839 | lineal |
| listar | 0,023 \* | 0,004 \* | 0,0015 | por lote |
| metadatos | 0,150 | 0,142 | 0,143 | lineal |
| eliminar | 0,208 | 0,242 | 0,231 | lineal |

\* Desviación mayor que la mitad de la media: no fiable, no se usa para concluir.

**Tres hallazgos:**

1. **Lo que decide el coste es el número de llamadas al sistema.** Listar 1000
   archivos con sus metadatos tarda 1,5 ms; pedir esos mismos metadatos uno a
   uno, 143 ms: 97 veces más. Con diez mil archivos, el cuello de botella
   serían crear, metadatos y eliminar, que hacen al menos una llamada por
   archivo.
2. **Bajo carga comprobada** —memoria al 86 %, los 12 procesadores lógicos al
   100 %— **la herramienta se ralentiza 2,1 veces de media** (entre 1,6 y 3,8),
   lo esperable en un procesador de 6 núcleos físicos, y ninguna operación
   falló.
3. **Crear un archivo cuesta unas 15 veces más en Windows que en Linux (WSL2)**
   en el mismo equipo. El coste dominante no está en el código de la
   herramienta.

---

## Uso

```
recursos                     menu interactivo (requisito 4)
recursos --archivos          lista los archivos del directorio
recursos --procesos          procesos activos con PID y memoria
recursos --memoria           total, usada y disponible
recursos --autoconsumo       lo que gasta esta misma herramienta
recursos --benchmark         mide los tiempos de las operaciones
recursos --help              ayuda completa
```

Las opciones directas existen para los scripts y las demostraciones; el menú es
la interfaz principal.

---

## Reproducir toda la evidencia

```powershell
powershell -ExecutionPolicy Bypass -File scripts\correr_experimentos.ps1
powershell -ExecutionPolicy Bypass -File scripts\verificar_linux.ps1
```

El primero compila, corre las pruebas, ejecuta los tres experimentos y provoca
los tres casos de error. **Aborta si la suite no pasa.** El segundo compila y
prueba la rama de Linux en WSL, instalando `g++` si hace falta.

---

## Estructura

```
src/core/         Logica pura: estadistica, cronometro, mediciones
src/plataforma/   TODO lo que toca el sistema operativo
src/io/           Menu, reportes, tablas
src/main.cpp      Solo orquesta
tests/            Arnes propio, 26 pruebas, sin framework
scripts/          Experimentos, verificacion de Linux y de documentos
results/          Evidencia generada. Se versiona a proposito
docs/             Documento IEEE, QA y plan de trabajo
```

**La regla del proyecto:** todo el código dependiente del sistema vive en
`src/plataforma/`. Se comprueba buscando `#include <windows.h>` fuera de ahí, y
**la salida vacía es la evidencia**.

Son **382 líneas específicas de plataforma sobre 2 748 totales: el 14 %**. Ese
porcentaje es la medida concreta de lo que costaría portar la herramienta a un
tercer sistema operativo.

---

## Compilación

| Vía | Comando | Estado |
|---|---|---|
| MSVC (Windows) | `build.bat` | Verificada |
| MSVC + pruebas | `build.bat tests` | Verificada |
| g++ (Linux) | `make` | **Verificada** |
| g++ + pruebas | `make tests` | **Verificada** |

---

## Documentos

| Documento | Ruta |
|---|---|
| Documento IEEE | `docs/documento_ieee_recursos.{md,docx,pdf}` |
| Aseguramiento de calidad e ISO/IEC 25010 | `docs/QA_Proyecto4.{md,docx,pdf}` |
| Plan de trabajo | `docs/PLAN_Proyecto4_Recursos.md` |
| Índice de entregables | `ENTREGABLES.md` |

El Markdown es la fuente autoritativa; los `.docx` y `.pdf` se generan con
`scripts/docs/md2docx.js`.

---

## Limitaciones declaradas

1. La verificación de Linux se hizo sobre **WSL2**, no sobre una máquina Linux
   nativa. El núcleo es real y el código ejercitado es el mismo, pero el
   hardware no es el de un servidor.
2. **Los permisos que se muestran en Windows no son los reales**: la biblioteca
   estándar de C++ no traduce las listas de control de acceso y devuelve el
   permiso más amplio. En Linux sí son los bits del inodo. La herramienta lo
   advierte en su salida. Además, en Windows se eliminan también los archivos
   con el atributo de solo lectura.
3. El monitoreo es una **fotografía instantánea**, no un seguimiento continuo.
4. No se mide cobertura de código: no hay herramienta disponible sin instalar
   software adicional.
5. Los tiempos se midieron en una sola máquina; los valores absolutos no son
   extrapolables, las relaciones sí.

La lista completa está en la sección 10 de `docs/QA_Proyecto4.md`.
