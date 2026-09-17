# ESPECIFICACIONES DE PROYECTOS 1–4
## TIIT2007 – Sistemas Operativos | Universidad Invenio

Todos los proyectos se desarrollan individualmente en C++ (estándar C++17 o superior recomendado), deben compilar sin advertencias críticas, incluir datos de prueba reproducibles y un documento IEEE asociado (ver plantilla dedicada). Cada proyecto se evalúa también bajo ISO/IEC 25010 (ver rúbrica dedicada).

---

## PROYECTO 1 — Simulador de Scheduling

**Semanas:** 2 (apertura) – 4 (entrega), 18% de la nota final.

### Objetivo
Implementar un simulador que reciba una carga de procesos sintética y calcule el comportamiento del sistema bajo dos algoritmos de planificación: **FCFS** y **Round Robin**.

### Requisitos funcionales
1. Leer un conjunto de procesos desde archivo (ID, tiempo de llegada, ráfaga de CPU/burst time).
2. Implementar FCFS (cola simple, sin apropiación).
3. Implementar Round Robin con quantum configurable (parámetro de entrada).
4. Calcular y reportar por proceso y en promedio:
   - Tiempo de espera (waiting time).
   - Tiempo de retorno (turnaround time).
5. Calcular utilización de CPU (% de tiempo no ocioso) durante la simulación.
6. Generar un diagrama de Gantt textual (representación ASCII aceptable) del orden de ejecución.

### Requisitos no funcionales
- Código modular: separar lógica de simulación de la lógica de entrada/salida.
- Manejo de errores ante archivos de entrada malformados.
- Uso de al menos una estructura de datos apropiada (cola, lista enlazada) para representar la cola de listos.

### Resultados experimentales obligatorios
- Tabla de tiempos de espera y retorno por proceso, para FCFS y para Round Robin (mínimo 2 quantum distintos, p. ej. q=2 y q=4).
- Gráfico o tabla de utilización de CPU comparada entre algoritmos.
- Al menos un conjunto de datos con 10+ procesos sintéticos.

### Entregables
- Código fuente (.cpp/.h) + instrucciones de compilación.
- Conjunto(s) de datos de entrada utilizados.
- Resultados experimentales (tablas/gráficos).
- Documento IEEE completo.
- Autoevaluación ISO/IEC 25010.

---

## PROYECTO 2 — Simulador de Concurrencia

**Semanas:** 5 (apertura) – 6 (entrega), 18% de la nota final.

### Objetivo
Implementar el problema **Productor-Consumidor** usando hilos reales de C++ (`std::thread`), demostrando el efecto de la sincronización mediante `std::mutex` (y opcionalmente variables de condición).

### Requisitos funcionales
1. Implementar un buffer compartido de tamaño finito.
2. Implementar al menos 2 hilos productores y 2 hilos consumidores.
3. Versión A (sin sincronización): demostrar evidencia reproducible de condición de carrera (datos corruptos, valores perdidos/duplicados, o violación de tamaño de buffer).
4. Versión B (con sincronización): implementar exclusión mutua correcta con `std::mutex` (y `std::condition_variable` si se desea evitar espera activa).
5. Registrar evidencia cuantitativa de la diferencia entre ambas versiones (p. ej. número de inconsistencias detectadas, tiempo total de ejecución).

### Requisitos no funcionales
- El código de la versión sin sincronización debe ser deliberada y honestamente vulnerable (no simulada artificialmente).
- Documentar claramente en comentarios cuál sección representa la sección crítica.

### Resultados experimentales obligatorios
- Comparación cuantitativa: ejecución sin sincronización vs. con sincronización (mínimo 5 corridas de cada una).
- Evidencia explícita de al menos una condición de carrera capturada (log o salida de consola).
- Análisis de overhead introducido por la sincronización (tiempo de ejecución con vs. sin mutex).

### Entregables
- Código fuente de ambas versiones.
- Logs/evidencia de ejecución.
- Documento IEEE completo.
- Autoevaluación ISO/IEC 25010.

---

## PROYECTO 3 — Simulador de Gestión de Memoria

**Semanas:** 7 (apertura) – 8 (entrega), 18% de la nota final.

### Objetivo
Implementar un simulador de reemplazo de páginas que compare los algoritmos **FIFO** y **LRU** ante una misma cadena de referencias de memoria.

### Requisitos funcionales
1. Leer una cadena de referencias de páginas desde archivo (secuencia de números de página).
2. Implementar el algoritmo FIFO con tamaño de marco configurable.
3. Implementar el algoritmo LRU con tamaño de marco configurable.
4. Contabilizar fallos de página (page faults) para cada algoritmo.
5. Reportar el estado de los marcos de memoria en cada paso (traza paso a paso) para al menos una cadena de referencia.

### Requisitos no funcionales
- Estructura de datos eficiente para LRU (se recomienda lista enlazada + estructura de acceso rápido, o justificar alternativa).
- Parámetros (tamaño de marco, archivo de entrada) configurables sin recompilar.

### Resultados experimentales obligatorios
- Tabla de fallos de página por algoritmo, con al menos 3 tamaños de marco distintos (p. ej. 3, 4, 5 marcos).
- Comparación gráfica o tabular: FIFO vs. LRU, fallos de página vs. tamaño de marco.
- Métricas de rendimiento: tiempo de ejecución de la simulación para cadenas largas (1000+ referencias sintéticas).

### Entregables
- Código fuente.
- Cadenas de referencia utilizadas (mínimo 2: una corta para traza manual, una larga para métricas).
- Resultados experimentales.
- Documento IEEE completo.
- Autoevaluación ISO/IEC 25010.

---

## PROYECTO 4 — Administrador Simplificado de Recursos

**Semanas:** 9 (apertura) – 11 (entrega), 18% de la nota final.

### Objetivo
Implementar una herramienta de consola en C++ que integre gestión básica de archivos y monitoreo de procesos/memoria del sistema operativo anfitrión, consolidando las competencias de las Unidades V y VI.

### Requisitos funcionales
1. **Gestión básica de archivos:** crear, listar, eliminar y mostrar metadatos (tamaño, fecha de modificación, permisos básicos) de archivos dentro de un directorio de trabajo controlado.
2. **Monitoreo de procesos:** listar procesos activos del sistema anfitrión (puede invocar comandos del SO — `ps`/`tasklist` — y parsear su salida, o usar APIs del sistema si el estudiante lo prefiere) mostrando PID, nombre y consumo aproximado de recursos.
3. **Monitoreo de memoria:** reportar memoria total, usada y disponible del sistema anfitrión.
4. Interfaz de consola con menú de navegación entre las tres funciones.

### Requisitos no funcionales
- Manejo robusto de errores (archivo inexistente, permisos insuficientes, comando no disponible).
- Portabilidad razonable: el estudiante debe documentar explícitamente si su implementación es específica de Linux, Windows, o ambas (con bloques condicionales de compilación si aplica).

### Resultados experimentales obligatorios
- Comparación de rendimiento: tiempo de respuesta de las operaciones de archivo bajo distinta cantidad de archivos (p. ej. 10 vs. 100 vs. 1000 archivos).
- Pruebas de carga: comportamiento de la herramienta mientras el sistema tiene una carga de procesos/memoria elevada (puede apoyarse en cargas generadas en los Laboratorios 4 y 6).
- Métricas de recursos: consumo de recursos de la propia herramienta mientras opera.

### Entregables
- Código fuente.
- Evidencia de pruebas de carga (capturas/logs).
- Resultados experimentales.
- Documento IEEE completo.
- Autoevaluación ISO/IEC 25010.

---

## Consideraciones Transversales a los 4 Proyectos

- **Honestidad de los datos experimentales:** los resultados deben provenir de ejecuciones reales y reproducibles, no de valores inventados. El docente puede solicitar re-ejecución en vivo durante la Defensa Técnica.
- **Trazabilidad con IA:** si el estudiante usó herramientas de IA generativa en el desarrollo, debe poder explicar y defender cada decisión de diseño e implementación (ver Política de IA, Syllabus).
- **Reutilización de código entre proyectos:** se permite reutilizar estructuras de datos genéricas (p. ej. una clase `Proceso`) entre proyectos, siempre que se declare explícitamente en el documento IEEE correspondiente.
