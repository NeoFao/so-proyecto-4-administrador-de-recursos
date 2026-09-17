# Simulador de Scheduling: Análisis Comparativo de Algoritmos FCFS y Round Robin

**Pablo Cruz** — Universidad Invenio  
Licenciatura en Tecnologías de Información y Comunicación Empresarial  
TIIT2007 – Sistemas Operativos | Julio 2026

---

## 1. Resumen

Este proyecto implementa un simulador de planificación de procesos en C++17 que modela y compara dos algoritmos clásicos de scheduling: First Come First Served (FCFS) y Round Robin (RR) con quantum configurable. El simulador procesa cargas sintéticas de procesos desde archivos CSV, calcula métricas de rendimiento (tiempo de espera, tiempo de retorno, utilización de CPU) y genera diagramas de Gantt con caracteres de recuadro ASCII extendido y bloques de colores. Se utilizaron dos conjuntos de datos: 10 procesos (llegadas 0–9, ráfagas 1–9) y 50 procesos sintéticos (llegadas 0–100, ráfagas 1–15). Los experimentos muestran que FCFS produce menor tiempo de espera promedio (21.0 unidades en 10 procesos, 115.26 en 50 procesos) frente a RR con quantum=2 (24.6 y 155 respectivamente) y quantum=4 (24.0 y 148), mientras que Round Robin ofrece mejor distribución de respuesta para procesos de ráfaga corta. La utilización de CPU alcanza el 100% (10 procesos) y 99.45% (50 procesos) al no modelar operaciones de E/S. Los resultados se alinean con el comportamiento teórico esperado descrito en Silberschatz, Tanenbaum y OSTEP. El simulador incluye una interfaz CLI con colores ANSI, caracteres de recuadro y pausas interactivas.

**Palabras clave**: scheduling, FCFS, Round Robin, planificación de procesos, diagrama de Gantt, C++17

---

## 2. Introducción

### 2.1 Contexto
La planificación de procesos (scheduling) es uno de los mecanismos fundamentales de todo sistema operativo moderno. El planificador decide qué proceso en estado listo obtiene el uso de la CPU en cada momento, impactando directamente métricas críticas como el tiempo de respuesta, la equidad y el rendimiento general del sistema [1]. Comprender el comportamiento de los distintos algoritmos de planificación es esencial para el diseño de sistemas operativos eficientes.

### 2.2 Objetivo general
Implementar un simulador computacional en C++ que modele los algoritmos FCFS y Round Robin, permitiendo la comparación cuantitativa de su rendimiento bajo cargas de trabajo controladas, con una interfaz de línea de comandos amigable que incluya colores, caracteres de recuadro y diagramas de Gantt visuales.

### 2.3 Objetivos específicos
1. Diseñar una estructura de datos que represente el PCB de un proceso con sus métricas asociadas.
2. Implementar el algoritmo FCFS con ordenamiento por tiempo de llegada.
3. Implementar el algoritmo Round Robin con quantum configurable por parámetro.
4. Calcular y reportar tiempo de espera, tiempo de retorno y utilización de CPU por proceso y en promedio.
5. Generar diagramas de Gantt con caracteres de bloque y recuadro para visualizar la secuencia de ejecución.
6. Comparar cuantitativamente ambos algoritmos con distintos valores de quantum (q=2 y q=4).
7. Proveer una interfaz CLI con colores, pausas interactivas y tablas con formato de recuadro.

### 2.4 Alcance y limitaciones
El simulador asume que todos los procesos son independientes (sin comunicación entre ellos), que no hay operaciones de E/S durante la ráfaga de CPU, y que el costo del context switch es despreciable. No se implementan algoritmos con prioridades, SJF, ni planificación por colas multinivel. La interfaz requiere una terminal compatible con ANSI escape codes (Windows 10+ o Linux).

---

## 3. Marco Teórico

### 3.1 Fundamentos de planificación de procesos
La planificación de procesos es la actividad del sistema operativo que determina cuál de los procesos en estado listo será ejecutado por la CPU. El módulo encargado se denomina planificador de corto plazo (short-term scheduler) y se ejecuta con alta frecuencia para maximizar la utilización de la CPU [1]. Las métricas clave para evaluar un planificador incluyen el tiempo de espera (waiting time), el tiempo de retorno (turnaround time) y la utilización de CPU.

### 3.2 First Come First Served (FCFS)
FCFS es el algoritmo de planificación más simple: el proceso que llega primero es el primero en ser atendido. Es no apropiativo (non-preemptive), lo que significa que una vez que un proceso obtiene la CPU, la mantiene hasta que finaliza su ráfaga. Su principal desventaja es el efecto convoy, donde procesos cortos esperan detrás de procesos largos [2]. La implementación utiliza una cola FIFO simple.

### 3.3 Round Robin (RR)
Round Robin es un algoritmo apropiativo (preemptive) diseñado para sistemas de tiempo compartido. Cada proceso recibe un quantum fijo de tiempo de CPU; si el proceso no ha terminado al expirar el quantum, es devuelto a la cola de listos. El tamaño del quantum es crítico: valores muy pequeños aumentan el overhead de context switch, mientras que valores muy grandes degradan el algoritmo hacia FCFS [3]. La implementación utiliza un `std::deque` como cola de listos circular.

### 3.4 Relación con xv6
El kernel de xv6 implementa una variante de Round Robin en su función `scheduler()`, localizada en `proc.c`. La implementación recorre la tabla de procesos en busca del siguiente proceso en estado RUNNABLE, asignándole la CPU. A diferencia de xv6, nuestro simulador utiliza una cola explícita (`std::deque`) para gestionar el orden de los procesos listos, lo que permite un control más preciso sobre la rotación y la inserción de nuevos procesos en tiempo real [observación de xv6, Unidad II].

---

## 4. Diseño de la Solución

### 4.1 Arquitectura general
El simulador sigue una arquitectura modular de cinco capas, cada una con responsabilidades claramente definidas:

- **Capa de datos**: estructura `Proceso` (PCB simplificado) y `CargadorDatos` (lectura CSV)
- **Capa de algoritmos**: módulos `FCFS` y `RoundRobin` (lógica de planificación pura)
- **Capa de métricas**: módulo `Metricas` (cálculo de indicadores)
- **Capa de presentación**: módulo `Gantt` (visualización con caracteres de recuadro y bloques)
- **Capa de interfaz**: módulo `UI` (colores ANSI, caracteres de recuadro, pausas interactivas)

### 4.2 Estructuras de datos
La estructura `Proceso` (ver Fragmento 1) modela el PCB con campos para ID, tiempo de llegada, ráfaga de CPU, ráfaga restante (para RR), y métricas calculadas (tiempo de inicio, fin, espera, retorno). Para Round Robin se utiliza un `std::deque<int>` como cola de listos, que almacena índices de procesos. Esta estructura fue seleccionada porque permite inserción y extracción en ambos extremos en tiempo O(1), lo cual es necesario cuando nuevos procesos llegan durante la simulación y deben agregarse al final de la cola.

```
struct Proceso {
    std::string id;
    int tiempo_llegada;
    int rafaga_cpu;
    int rafaga_restante;
    int tiempo_inicio = -1;
    int tiempo_fin = 0;
    int tiempo_espera = 0;
    int tiempo_retorno = 0;
    void reiniciar();
};
```
*Fragmento 1: Estructura PCB del proceso.*

### 4.3 Supuestos de diseño
- Los procesos llegan en el orden definido por el archivo de entrada
- No hay operaciones de E/S durante la ráfaga de CPU
- El context switch tiene costo cero
- Todos los procesos están listos al llegar (no hay bloqueos)
- Los IDs de proceso siguen el formato Pn (ej: P1, P2, ..., P50)

---

## 5. Implementación

### 5.1 Lenguaje y estándar
C++17 con la biblioteca estándar. Compilado con Microsoft Visual C++ (cl.exe) en Windows 10. No se requieren dependencias externas. El código es compatible con GCC en Linux/MinGW.

### 5.2 Módulos principales
- **`Proceso.h`**: definición del PCB con método `reiniciar()` para reuso entre simulaciones
- **`CargadorDatos`**: parser CSV con manejo de excepciones para archivos malformados, líneas vacías y valores no numéricos
- **`FCFS`**: ordena procesos por `tiempo_llegada` y ejecuta secuencialmente; calcula inicio, fin, espera y retorno
- **`RoundRobin`**: mantiene cola rotativa (`std::deque`) con inserción de nuevos procesos en tiempo real; quantum configurable
- **`Metricas`**: cálculo de promedios y generación de tablas comparativas con formato de recuadro
- **`Gantt`**: construcción de diagrama de Gantt con caracteres de bloque `█` y recuadro doble `╔╗╚╝`
- **`UI`**: configuración de consola ANSI/UTF-8, colores por proceso, pausas interactivas

### 5.3 Compilación y ejecución
```
cl.exe /Zi /EHsc /nologo /Fescheduling.exe src\CargadorDatos.cpp src\FCFS.cpp src\Gantt.cpp src\main.cpp src\Metricas.cpp src\RoundRobin.cpp
.\scheduling.exe [archivo.csv]
```

### 5.4 Interfaz de usuario
La interfaz CLI incluye:
- **Colores ANSI**: cada proceso se muestra con un color distinto (cian, verde, amarillo, magenta, azul, rojo, blanco)
- **Caracteres de recuadro**: uso de `╔╗╚╝║═` para bordes dobles y `│─└┘├┤` para bordes simples
- **Bloques de relleno**: uso de `█` para las barras del diagrama de Gantt
- **Pausas interactivas**: el usuario presiona ENTER entre cada simulación
- **Resaltado de métricas**: el mejor valor se muestra en verde, el peor en rojo

---

## 6. Resultados Experimentales

### 6.1 Entorno de prueba
- **CPU**: Intel(R) Core(TM) i5
- **RAM**: 16 GB
- **SO**: Windows 10
- **Compilador**: Microsoft Visual C++ 19.x (cl.exe)
- **Terminal**: PowerShell 7

### 6.2 Dataset de 10 procesos
El dataset `procesos10.csv` contiene 10 procesos con llegadas entre 0 y 9, y ráfagas entre 1 y 9 unidades de tiempo. Los resultados se muestran en la Tabla 1.

**Tabla 1: Comparativa de métricas promedio (10 procesos)**

| Métrica | FCFS | RR (q=2) | RR (q=4) |
|---------|:----:|:--------:|:--------:|
| T. Espera Prom. | 21.0 | 24.6 | 24.0 |
| T. Retorno Prom. | 25.9 | 29.5 | 28.9 |
| CPU Utilización | 100% | 100% | 100% |

### 6.3 Dataset extendido de 50 procesos
El dataset `procesos50.csv` contiene 50 procesos sintéticos generados con llegadas aleatorias entre 0 y 100, y ráfagas entre 1 y 15. Los resultados se muestran en la Tabla 2.

**Tabla 2: Comparativa de métricas promedio (50 procesos)**

| Métrica | FCFS | RR (q=2) | RR (q=4) |
|---------|:----:|:--------:|:--------:|
| T. Espera Prom. | 115.26 | 155.00 | 148.00 |
| T. Retorno Prom. | 122.48 | 162.22 | 155.22 |
| CPU Utilización | 99.45% | 99.45% | 99.45% |

### 6.4 Diagramas de Gantt
Los diagramas de Gantt generados utilizan caracteres de bloque `█` coloreados por proceso, con recuadro doble `╔╗╚╝` y escala de tiempo numérica. Cada proceso aparece en una fila independiente, mostrando visualmente los intervalos de ejecución. La Figura 1 muestra un ejemplo para FCFS con 10 procesos.

```
        01234567890123456789012345678901234567890123456789
        ╔═════════════════════════════════════════════════╗
  P1   │████████                                         │
  P2   │        ████                                     │
  P3   │            █████████                            │
  P4   │                     █████                       │
  P5   │                          ██                     │
  P6   │                            ██████               │
  P7   │                                  ███            │
  P8   │                                     ███████    │
  P9   │                                            █   │
  P10  │                                             ████│
        ╚═════════════════════════════════════════════════╝
        01234567890123456789012345678901234567890123456789
```
*Figura 1: Diagrama de Gantt para FCFS con 10 procesos.*

### 6.5 Comparación cuantitativa
La Tabla 3 resume la comparación entre ambos datasets, mostrando que FCFS consistentemente produce menor tiempo de espera y retorno promedio. Round Robin con quantum=4 se aproxima más al rendimiento de FCFS que Round Robin con quantum=2.

**Tabla 3: Comparación cruzada de algoritmos**

| Escenario | Mejor tiempo espera | Mejor tiempo retorno | Mejor utilización |
|-----------|:-------------------:|:--------------------:|:-----------------:|
| 10 procesos | FCFS (21.0) | FCFS (25.9) | Empate (100%) |
| 50 procesos | FCFS (115.26) | FCFS (122.48) | Empate (99.45%) |

---

## 7. Evaluación ISO/IEC 25010

| Característica | Calificación | Justificación |
|---------------|:-----------:|---------------|
| **Rendimiento** | 4 | La simulación se completa en milisegundos incluso con 50 procesos. Las métricas se calculan en O(n) donde n es el número de procesos. Se midió el tiempo de ejecución: < 100ms para 10 procesos, < 500ms para 50 procesos. |
| **Fiabilidad** | 3 | Manejo de errores para archivos inexistentes, líneas malformadas y valores no numéricos. Los resultados son reproducibles (mismos datos de entrada producen mismos resultados). Sin embargo, no se implementaron pruebas unitarias automatizadas. |
| **Usabilidad** | 4 | Interfaz CLI con colores, recuadros, pausas interactivas y diagramas visuales de Gantt. Cada proceso tiene un color distinto. Las tablas tienen formato de recuadro claro. El flujo guía al usuario paso a paso con indicaciones de "Presiona ENTER para continuar". |
| **Compatibilidad** | 3 | Compila y funciona en Windows (MSVC) y Linux (GCC) sin modificaciones. La configuración de consola ANSI/UTF-8 se maneja automáticamente en Windows. En Linux no requiere configuración adicional. Dependencia de `windows.h` solo para la configuración de consola en Windows. |
| **Mantenibilidad** | 4 | Código modular con 7 archivos separados por responsabilidad (Proceso, CargadorDatos, FCFS, RoundRobin, Metricas, Gantt, UI). Nombres descriptivos en inglés y español. Comentarios en secciones clave. Separación clara entre lógica de simulación y presentación. |

---

## 8. Discusión de Resultados

### 8.1 Interpretación de resultados
Los resultados confirman el comportamiento esperado teóricamente: FCFS minimiza el tiempo de espera promedio cuando los procesos de ráfaga corta no quedan bloqueados detrás de procesos largos (efecto convoy). En el dataset de 10 procesos, FCFS supera a RR en todas las métricas. En el dataset de 50 procesos, la diferencia se amplía: FCFS tiene un tiempo de espera 25.6% menor que RR q=2 y 22.1% menor que RR q=4.

Round Robin, al ser apropiativo, distribuye la CPU más equitativamente pero introduce latencia adicional en los procesos de ráfaga larga, incrementando el tiempo de retorno promedio. Se observa que RR con quantum=4 tiene mejor rendimiento que RR con quantum=2, ya que un quantum más grande reduce la frecuencia de los context switches implícitos.

### 8.2 Relación con la teoría
Los resultados coinciden con lo descrito en la literatura [1][2][3]:
- FCFS produce el mínimo tiempo de espera cuando no hay efecto convoy significativo
- Round Robin con quantum pequeño (q=2) maximiza la equidad pero aumenta el tiempo de retorno
- Round Robin con quantum más grande (q=4) se aproxima al comportamiento de FCFS
- La utilización de CPU del 100% (o 99.45%) se explica porque el simulador no modela operaciones de E/S

### 8.3 Limitaciones encontradas
- El simulador no modela el overhead de context switch, lo que favorece a Round Robin en los resultados
- No se implementaron operaciones de E/S durante la ráfaga de CPU
- La generación de datos sintéticos usa una semilla fija (42), lo que garantiza reproducibilidad pero limita la variedad de escenarios
- El diagrama de Gantt para 50 procesos es muy ancho (363 unidades de tiempo), lo que dificulta su visualización en terminales estrechas

---

## 9. Conclusiones

Se implementó exitosamente un simulador de scheduling que permite comparar FCFS y Round Robin bajo condiciones controladas, cumpliendo con todos los requisitos funcionales especificados. FCFS demostró menor tiempo de espera promedio en ambos datasets, mientras que Round Robin ofrece mejor equidad en la distribución de CPU.

**Aprendizajes clave:**
- La selección del algoritmo de scheduling tiene un impacto directo y cuantificable en las métricas de rendimiento del sistema
- El tamaño del quantum en Round Robin es un parámetro crítico: valores pequeños aumentan la equidad pero degradan el rendimiento general
- La modularidad del código permite extender el simulador con nuevos algoritmos sin modificar la lógica existente
- La interfaz de usuario en CLI puede ser significativamente mejorada con colores y caracteres de recuadro, haciendo la herramienta más accesible

### Posibles extensiones
- Implementar SJF (Shortest Job First) y planificación por prioridades
- Modelar el overhead de context switch como parámetro configurable
- Agregar soporte para operaciones de E/S durante la ráfaga
- Generar gráficos SVG/PNG del diagrama de Gantt además de la versión textual

---

## 10. Referencias

[1] Silberschatz, A., Galvin, P. B., & Gagne, G. *Operating System Concepts*. 10th ed., Wiley, 2018, Capítulo 5: CPU Scheduling.

[2] Tanenbaum, A. S. *Modern Operating Systems*. 4th ed., Pearson, 2014, Capítulo 2: Processes and Threads.

[3] Arpaci-Dusseau, R. & Arpaci-Dusseau, A. *Operating Systems: Three Easy Pieces (OSTEP)*. Arpaci-Dusseau Books, 2018, Capítulo 7: Scheduling.

---

## Anexo A: Checklist de entrega

- [x] Las 10 secciones están presentes y en el orden correcto
- [x] El resumen es autocontenible (150-250 palabras)
- [x] Los resultados experimentales obligatorios están incluidos (10 y 50 procesos)
- [x] La autoevaluación ISO/IEC 25010 está justificada
- [x] Todas las referencias citadas aparecen en la sección 10
- [x] Código fuente entregado en repositorio aparte
- [x] Datos de entrada incluidos (procesos10.csv, procesos50.csv)
- [x] Resultados guardados (comparativa.txt, simulacion_completa.txt, simulacion_50procesos.txt)
- [x] Compilación sin errores ni advertencias