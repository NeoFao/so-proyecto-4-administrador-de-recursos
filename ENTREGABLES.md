# Entregables — Proyecto 4: Administrador Simplificado de Recursos

**Fabrizio Espinoza Arce** · TIIT2007 Sistemas Operativos · Universidad Invenio

Índice de correspondencia entre lo que pide el enunciado y dónde está en este
paquete. Para verlo funcionando, ejecute **`EJECUTAR.bat`**: recorre los cuatro
requisitos funcionales y los resultados experimentales, uno por uno y rotulados.

---

## 1. Código fuente

| Ruta | Contenido |
|---|---|
| `src/plataforma/Archivos.{h,cpp}` | **RF-1**: crear, listar, eliminar y metadatos en directorio controlado |
| `src/plataforma/Sistema.h` | Interfaz de consulta de procesos y memoria |
| `src/plataforma/SistemaWindows.cpp` | **RF-2 y RF-3** en Windows: `tasklist` + `GlobalMemoryStatusEx` |
| `src/plataforma/SistemaLinux.cpp` | **RF-2 y RF-3** en Linux: `ps` + `/proc/meminfo` |
| `src/io/Menu.{h,cpp}` | **RF-4**: menú de navegación entre las tres funciones |
| `src/io/ReporteConsola.{h,cpp}` | Presentación de las tres vistas y de las mediciones |
| `src/core/` | Lógica pura: estadística, cronómetro, tipos de medición |
| `src/main.cpp` | Orquestador |
| `tests/test_recursos.cpp` | 26 pruebas, arnés propio sin framework externo |
| `build.bat`, `Makefile`, `EJECUTAR.bat` | Compilación en Windows y en Linux |

**Nota sobre la arquitectura:** todo el código dependiente del sistema operativo
está en `src/plataforma/`. Son 382 líneas de 2 748, el 14 % del total.

## 2. Evidencia de pruebas de carga

| Archivo | Contenido |
|---|---|
| `results/prueba_carga.txt` | **RE-2**: las mismas mediciones con el sistema cargado, más la comprobación de que la carga existió de verdad |
| `results/csv/carga_10.csv`, `carga_100.csv`, `carga_1000.csv` | Datos crudos bajo carga |
| `results/csv/rendimiento_*.csv` | Los mismos datos sin carga, para comparar |

La carga son doce procesos de CPU en bucle más un proceso que reserva y escribe
memoria, según los Laboratorios 4 y 6. El script la genera, **comprueba que está
aplicada antes y después de medir** —en la corrida entregada: memoria al 86,4 %,
CPU al 100 %, 13 de 13 procesos de carga vivos al terminar— y la retira solo.

## 3. Resultados experimentales

Todo en `results/`, regenerable con **un comando**:

```
powershell -ExecutionPolicy Bypass -File scripts\correr_experimentos.ps1
```

| Archivo | Resultado |
|---|---|
| `results/rendimiento_archivos.txt` | **RE-1**: tiempos con 10, 100 y 1000 archivos |
| `results/prueba_carga.txt` | **RE-2**: comportamiento bajo carga elevada |
| `results/autoconsumo.txt` | **RE-3**: consumo de la propia herramienta |
| `results/vistas_menu.txt` | Las tres funciones del enunciado, capturadas |
| `results/errores_manejados.txt` | **RNF-1**: los tres casos de error, provocados |
| `results/pureza_plataforma.txt` | Evidencia de que `windows.h` vive en un solo sitio |
| `results/compatibilidad_linux.txt` | **RNF-2**: la misma herramienta compilada y probada en Linux |
| `results/pruebas_windows.txt` | Salida completa de las 26 pruebas |
| `results/csv/` | Datos crudos para recalcular cualquier resumen |

## 4. Documento IEEE completo

| Formato | Ruta |
|---|---|
| PDF | `docs/documento_ieee_recursos.pdf` |
| Word | `docs/documento_ieee_recursos.docx` |
| Markdown (fuente) | `docs/documento_ieee_recursos.md` |

Las 10 secciones de la plantilla institucional, a dos columnas, con tablas y
figuras numeradas y referenciadas desde el texto.

## 5. Autoevaluación ISO/IEC 25010

Está en **dos sitios**, con distinto nivel de detalle:

| Dónde | Qué contiene |
|---|---|
| `docs/documento_ieee_recursos.pdf`, **sección 7** | La autoevaluación que pide la plantilla: las 5 características justificadas |
| `docs/QA_Proyecto4.pdf`, **sección 9** | La versión extendida, con la evidencia de cada calificación y el detalle de compatibilidad en la 9.1 |

Las cinco características en **nivel 4**, cada una con su archivo de evidencia.

---

## Los requisitos, uno por uno

| Requisito | Dónde se implementa | Cómo verlo |
|---|---|---|
| RF-1 archivos | `plataforma/Archivos.cpp` | `EJECUTAR.bat` demo 1 |
| RF-2 procesos | `plataforma/Sistema*.cpp` | `EJECUTAR.bat` demo 2 |
| RF-3 memoria | `plataforma/Sistema*.cpp` | `EJECUTAR.bat` demo 3 |
| RF-4 menú | `io/Menu.cpp` | `EJECUTAR.bat` demo 1 |
| RNF-1 errores | Excepciones en toda la capa | `EJECUTAR.bat` demo 4 |
| RNF-2 portabilidad | `src/plataforma/` | `results/compatibilidad_linux.txt` |
| RE-1 rendimiento | — | `EJECUTAR.bat` demo 5 |
| RE-2 carga | — | `results/prueba_carga.txt` |
| RE-3 autoconsumo | — | `EJECUTAR.bat` demo 5 |

## Material adicional (no exigido)

| Ruta | Qué es |
|---|---|
| `docs/QA_Proyecto4.md` | Documento de calidad: trazabilidad, plan de pruebas, gestión de defectos, reproducibilidad y limitaciones |
| `docs/PLAN_Proyecto4_Recursos.md` | Plan de trabajo previo a la implementación |
| `Dockerfile.linux`, `scripts/verificar_linux_interno.sh` | Alternativa a WSL para verificar la rama de Linux en un contenedor |
