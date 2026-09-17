# DATASET EXPERIMENTAL PARA LOS PROYECTOS
## TIIT2007 – Sistemas Operativos | Universidad Invenio

Conjuntos de datos sintéticos de referencia para que todos los estudiantes dispongan de un caso base común (pueden y deben complementarse con datos propios adicionales conforme a la especificación de cada proyecto).

---

## Dataset 1 — Scheduling (Proyecto 1)

**Formato sugerido de archivo de entrada (`procesos.csv`):**
```
id,llegada,rafaga
P1,0,8
P2,1,4
P3,2,9
P4,3,5
P5,4,2
P6,5,6
P7,6,3
P8,7,7
P9,8,1
P10,9,4
```

**Parámetros de prueba obligatorios:** Round Robin con quantum = 2 y quantum = 4.

**Dataset extendido (recomendado para métricas de utilización de CPU):** generar 50 procesos sintéticos con llegada distribuida uniformemente entre 0–100 y ráfaga entre 1–15 (puede generarse con script auxiliar en C++ o Python fuera del entregable).

---

## Dataset 2 — Concurrencia (Proyecto 2)

**Parámetros base obligatorios:**
- Tamaño de buffer: 5 elementos.
- Productores: 2 hilos, cada uno genera 20 elementos (40 elementos totales).
- Consumidores: 2 hilos, cada uno debe consumir hasta vaciar la producción total.
- Repeticiones para evidencia comparativa: mínimo 5 corridas por versión (sin sincronización / con sincronización).

**Métrica de corrupción de datos sugerida:** contador de elementos perdidos o duplicados al finalizar cada corrida sin sincronización (debe ser mayor a 0 en al menos 3 de las 5 corridas para considerarse evidencia válida).

---

## Dataset 3 — Gestión de Memoria (Proyecto 3)

**Cadena de referencia corta (traza manual, `referencias_corta.txt`):**
```
7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1
```

**Tamaños de marco obligatorios para esta cadena:** 3, 4 y 5.

**Cadena de referencia larga (métricas de rendimiento):** generar 1000+ referencias sintéticas con distribución que combine localidad (70% de referencias dentro de una "ventana" de 10 páginas recientes) y aleatoriedad (30% de referencias completamente aleatorias entre 0–99), para reflejar comportamiento realista de programas.

---

## Dataset 4 — Administrador de Recursos (Proyecto 4)

**Escenarios de prueba de archivos obligatorios:**
| Escenario | Cantidad de archivos | Tamaño aproximado por archivo |
|---|---|---|
| Carga baja | 10 | 1 KB |
| Carga media | 100 | 1 KB |
| Carga alta | 1000 | 1 KB |

**Escenario de prueba de carga del sistema:** ejecutar el Administrador de Recursos mientras corre simultáneamente el script de carga de memoria del Laboratorio 4 (Unidad IV), registrando tiempo de respuesta de cada operación bajo esta condición vs. condición normal.

---

## Notas de Uso

- Todos los datasets aquí definidos son el **mínimo obligatorio**; los estudiantes pueden y deben generar datasets adicionales propios para enriquecer su análisis experimental, siempre que documenten claramente su método de generación en la sección de Resultados Experimentales del documento IEEE.
- Se recomienda que el docente publique estos datasets como archivos descargables junto con un script generador simple (Python o C++) para los datasets extendidos, de modo que cada estudiante pueda regenerar variaciones con semillas distintas si se desea evitar duplicación exacta de resultados entre estudiantes.
