# Plan de Trabajo — Proyecto 4: Administrador Simplificado de Recursos

**Autor:** Fabrizio Espinoza Arce
**Curso:** TIIT2007 Sistemas Operativos — Universidad Invenio
**Peso:** 18 % de la nota final · Semanas 9 (apertura) – 11 (entrega)

---

## 1. Alcance: exactamente lo que pide el enunciado

Lección del Proyecto 3: allí se implementaron dos algoritmos de más (OPT y
Clock) que hubo que retirar a última hora porque ampliaban la superficie de
defensa sin que la rúbrica los pidiera. **Aquí no se añade nada que el
enunciado no pida.**

### Requisitos funcionales

| ID | Requisito | Módulo |
|---|---|---|
| RF-1 | Gestión de archivos: crear, listar, eliminar, metadatos, en directorio controlado | `plataforma/Archivos` |
| RF-2 | Listar procesos con PID, nombre y consumo | `plataforma/Sistema*` |
| RF-3 | Memoria total, usada y disponible | `plataforma/Sistema*` |
| RF-4 | Menú de consola que navega entre las tres funciones | `io/Menu` |

### Requisitos no funcionales

| ID | Requisito | Cómo se cumple |
|---|---|---|
| RNF-1 | Errores robustos: archivo inexistente, permisos, comando no disponible | Excepciones con mensaje que dice la condición y el valor; los tres casos tienen prueba |
| RNF-2 | Documentar si es Windows, Linux o ambas | **Ambas, las dos verificadas.** Ver sección 3 |

### Resultados experimentales obligatorios

| ID | Resultado | Evidencia |
|---|---|---|
| RE-1 | Tiempos de operaciones de archivo con 10 / 100 / 1000 archivos | `results/rendimiento_archivos.txt` + CSV |
| RE-2 | Comportamiento bajo carga de procesos/memoria | `results/prueba_carga.txt` |
| RE-3 | Consumo de recursos de la propia herramienta | `results/autoconsumo.txt` |

Los tamaños 10/100/1000 son los del **Dataset 4** del curso; el escenario de
carga es el que el Dataset 4 describe apoyándose en el Laboratorio 4.

## 2. Arquitectura

En el Proyecto 3 la regla de oro fue «el núcleo no hace entrada/salida». Aquí
**esa regla no aplica**: este proyecto *es* llamar al sistema operativo.
Forzarla sería artificial. La adaptación honesta aísla lo que sí varía entre
sistemas:

```
main.cpp          orquesta, no consulta al sistema
   |
   +-- src/io/          menu y presentacion
   |     Menu, ReporteConsola, FormatoTabla, ExportadorCsv
   |
   +-- src/plataforma/  TODO lo que toca el sistema operativo
   |     Archivos.cpp        portable (<filesystem>)
   |     Sistema.h           interfaz de procesos y memoria
   |     SistemaWindows.cpp  tasklist + GlobalMemoryStatusEx
   |     SistemaLinux.cpp    ps + /proc/meminfo
   |
   +-- src/core/       logica pura: metricas, estadistica, cronometro
         sin <windows.h>, sin llamadas al sistema
```

**La regla verificable:** un `grep` de `windows.h` sobre `src/core/` y
`src/io/` debe dar **salida vacía**. Todo lo específico del sistema vive en un
único directorio.

**Por qué importa:** la pregunta de defensa publicada para este proyecto es
*«si tuvieras que portar tu herramienta de Windows a Linux, ¿qué partes
específicas tendrías que reescribir?»*. Con esta separación la respuesta no es
una estimación: es una lista de ficheros.

Dentro de `plataforma/` hay dos clases de fichero, y conviene no confundirlas:

- **`Archivos.cpp`** tiene **una sola** implementación. `<filesystem>` de C++17
  ya resuelve las diferencias entre sistemas.
- **`Sistema*.cpp`** tiene **dos**. El estándar no cubre listar procesos ni
  consultar la memoria del anfitrión.

Esa es exactamente la línea entre lo que hubo que duplicar y lo que no.

## 3. Compatibilidad: las dos ramas, verificadas de verdad

En los Proyectos 2 y 3 la rama de Linux se entregó **escrita pero nunca
ejecutada**, y hubo que declararlo como limitación. Aquí no.

| Entorno | Compilador | Estado |
|---|---|---|
| Windows 11 x64 | MSVC 19.51, `/W4` | Verificado |
| Windows 11 x86 (32 bits) | MSVC 19.51, `/W4` | Verificado |
| Windows, `/std:c++20` | MSVC 19.51 | Verificado |
| Windows, `/Od` sin optimizar | MSVC 19.51 | Verificado |
| **Ubuntu 26.04 (WSL)** | **g++ 15.2, `-Wall -Wextra -Wpedantic`** | **Verificado** |

La rama de Linux se compila y se ejecuta en el Ubuntu de WSL de la máquina de
desarrollo, con su suite de pruebas completa. La evidencia queda en
`results/compatibilidad_linux.txt`.

**Se declara con precisión lo que eso significa y lo que no:** WSL es un kernel
Linux real, así que `/proc/meminfo` y `ps` son los de Linux y el código que se
ejercita es el mismo que correría en una máquina Linux nativa. Lo que WSL no
reproduce es el hardware ni la carga de un servidor real, así que las **cifras**
de las dos plataformas no son comparables entre sí; lo comparable es que **el
mismo código fuente funciona en las dos**.

## 4. Decisiones tomadas y por qué

| Decisión | Alternativa descartada | Razón |
|---|---|---|
| Parsear `tasklist` / `ps` | API nativa de cada sistema | El enunciado nombra los comandos como vía válida. Mantiene el módulo pequeño, hace el puerto casi literal, y ejercita de verdad el caso «comando no disponible» del RNF-1 |
| `/FO CSV /NH` y `-o pid=,comm=,rss=` | Formato por omisión | Los dos evitan la cabecera y el formato ancho, que **cambian con el idioma del sistema**. Sin esto, un Windows en español rompería el parseo |
| `MemAvailable` en Linux | `MemFree` | `MemFree` no cuenta la caché reclamable y en Linux siempre parece bajísimo. `MemAvailable` es la estimación del propio kernel y es la que reporta `free -h` |
| `<filesystem>` para archivos | API de cada sistema | Es estándar y portable: reduce la superficie específica de plataforma a solo dos ficheros |
| Frontera de directorio comprobada componente a componente | Comparar prefijos de cadena | Comparar cadenas dejaría pasar un directorio hermano con nombre parecido (`trabajo_otro` frente a `trabajo`) |

## 5. Suite de pruebas

Arnés propio sin framework externo, igual que en los tres proyectos
anteriores: en la máquina no hay gestor de paquetes de C++.

| Grupo | Contenido |
|---|---|
| 1 | Gestión de archivos: crear, listar, eliminar, metadatos |
| 2 | **Frontera del directorio controlado**: rechazar `..`, rutas absolutas, nombres vacíos |
| 3 | Contratos de error del RNF-1: archivo inexistente, duplicado, comando no disponible |
| 4 | Memoria: coherencia (`usada + disponible = total`, porcentaje en rango) |
| 5 | Procesos: lista no vacía, PID válido, nombre no vacío |
| 6 | Estadística y utilidades |

Las pruebas corren **en las dos plataformas** y el mismo fichero de pruebas
sirve para ambas.

## 6. Plan experimental

| # | Experimento | Qué mide |
|---|---|---|
| E1 | Crear, listar y consultar metadatos con 10, 100 y 1000 archivos | RE-1. Media ± σ sobre repeticiones |
| E2 | Las mismas operaciones con el sistema bajo carga | RE-2. Se compara con E1 |
| E3 | Consumo de la propia herramienta durante E1 | RE-3 |
| E4 | E1 repetido en Linux | Evidencia de compatibilidad |

**Metodología de tiempos.** Lección del Proyecto 3: una operación sobre 10
archivos tarda microsegundos y medirla una vez da ruido del reloj. Se repite N
veces y se reporta media ± desviación estándar muestral, y el reporte avisa
cuando σ supera la mitad de la media.

**La carga del sistema** se genera desde el propio script de experimentos con
PowerShell, reservando memoria y ocupando CPU, según el Laboratorio 4. No hace
falta software adicional.

## 7. Cronograma

| Fase | Contenido | Punto de control |
|---|---|---|
| **0. Andamiaje** | Estructura, capa de plataforma, compilación en Windows y Linux | **HECHO**: las dos plataformas compilan y corren con cero advertencias |
| 1. Núcleo y plataforma | Archivos, Sistema, métricas, cronómetro | Las tres funciones responden en los dos sistemas |
| 2. Pruebas | Suite en 6 grupos, en ambas plataformas | Suite en verde en Windows y en Linux |
| 3. Menú y presentación | `io/Menu`, reportes, CSV | El menú navega entre las tres funciones |
| 4. Experimentos | E1–E4, scripts, `results/` | `results/` poblado con un comando |
| 5. Documentos | IEEE, QA, README, guía de defensa | PDF revisados página por página |

## 8. Riesgos

| # | Riesgo | Mitigación |
|---|---|---|
| R1 | El formato de `tasklist` cambia con el idioma de Windows | `/FO CSV /NH`, y el campo de memoria se lee descartando todo lo que no sea dígito |
| R2 | Crear 1000 archivos deja basura si el experimento falla | El directorio de trabajo es propio del experimento y se limpia al empezar y al terminar |
| R3 | Los tiempos de 10 archivos son ruido | Repeticiones con media ± σ y aviso automático cuando la medición no es fiable |
| R4 | WSL podría no estar en la máquina del evaluador | La evidencia de Linux se entrega generada; el proyecto compila en Windows sin WSL |
| R5 | Medir el consumo propio en Windows requiere API específica | Se mide con el propio `tasklist` filtrando por el PID de la herramienta: sin código nuevo |
| R6 | `.bat` con finales LF rompen `call :etiqueta` | Trampa descubierta en el P3. `scripts/normalizar_bat_crlf.py` se ejecuta antes de cada verificación |

## 9. Definición de «terminado»

- [ ] Los 4 requisitos funcionales, demostrables desde el menú
- [ ] Los 2 no funcionales, con los tres casos de error probados
- [ ] Los 3 resultados experimentales, con datos medidos
- [ ] Cero advertencias en MSVC `/W4` y en g++ `-Wall -Wextra -Wpedantic`
- [ ] Suite en verde en **Windows y en Linux**
- [ ] `grep` de `windows.h` sobre `core/` e `io/` con salida vacía
- [ ] `results/` reproducible con un comando
- [ ] IEEE de 10 secciones, 4–8 páginas, dos columnas, citas verificadas
- [ ] QA de 12 secciones + ISO/IEC 25010
- [ ] `EJECUTAR.bat` que recorre los 4 requisitos y los 3 resultados, rotulados
- [ ] Guía de defensa
