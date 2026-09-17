# Proyecto 1 — Simulador de Scheduling

Simulador de algoritmos de planificación FCFS y Round Robin en C++17 con interfaz CLI mejorada (colores ANSI, caracteres de recuadro, diagramas de Gantt con bloques).

## Compilación

### Windows (MSVC)
```bash
cl.exe /Zi /EHsc /nologo /Fescheduling.exe src\CargadorDatos.cpp src\FCFS.cpp src\Gantt.cpp src\main.cpp src\Metricas.cpp src\RoundRobin.cpp
```

### Linux / MinGW (GCC)
```bash
g++ -std=c++17 -I src src/*.cpp -o scheduling.exe
```

## Ejecución

```bash
.\scheduling.exe                      # Dataset por defecto (10 procesos)
.\scheduling.exe data/procesos50.csv  # Dataset extendido (50 procesos)
```

## Requisitos

- Compilador con soporte C++17 (MSVC, GCC, Clang)
- Terminal compatible con ANSI escape codes (Windows 10+, PowerShell, CMD, Linux)
- No requiere bibliotecas externas

## Estructura del proyecto

```
proyecto1-scheduling/
├── src/
│   ├── Proceso.h            # Estructura PCB (ID, llegada, ráfaga, métricas)
│   ├── CargadorDatos.h/cpp  # Carga de CSV con manejo de errores
│   ├── FCFS.h/cpp           # Algoritmo First Come First Served
│   ├── RoundRobin.h/cpp     # Algoritmo Round Robin con quantum configurable
│   ├── Metricas.h/cpp       # Cálculo y reporte de métricas (tablas con recuadro)
│   ├── Gantt.h/cpp          # Diagrama Gantt con caracteres de bloque █ y recuadro
│   ├── UI.h                 # Utilidades: colores ANSI, caracteres de recuadro, pausas
│   └── main.cpp             # Punto de entrada integrador con flujo interactivo
├── data/
│   ├── procesos10.csv       # Dataset estándar (10 procesos, doc 12)
│   └── procesos50.csv       # Dataset extendido (50 procesos sintéticos)
├── results/
│   ├── simulacion_completa.txt        # Salida completa 10 procesos
│   ├── simulacion_50procesos.txt      # Salida completa 50 procesos
│   └── comparativa.txt                # Tabla comparativa resumen
├── docs/
│   └── documento_ieee.md              # Documento IEEE del proyecto
├── .vscode/
│   └── tasks.json                     # Tareas de compilación VS Code
└── README.md
```

## Características de la interfaz

- **Colores ANSI**: cada proceso se muestra con un color distinto
- **Caracteres de recuadro**: bordes dobles ╔╗╚╝║═ y simples │─└┘├┤
- **Diagramas de Gantt**: barras con bloque lleno █ coloreado por proceso
- **Pausas interactivas**: el usuario presiona ENTER entre simulaciones
- **Resaltado de métricas**: mejor valor en verde, peor en rojo
- **Barras de comparación visual**: █ para tiempo de espera

## Resultados clave

### Dataset de 10 procesos

| Métrica | FCFS | RR (q=2) | RR (q=4) |
|---------|:----:|:--------:|:--------:|
| T. Espera Prom. | 21.0 | 24.6 | 24.0 |
| T. Retorno Prom. | 25.9 | 29.5 | 28.9 |
| CPU Utilización | 100% | 100% | 100% |

### Dataset de 50 procesos

| Métrica | FCFS | RR (q=2) | RR (q=4) |
|---------|:----:|:--------:|:--------:|
| T. Espera Prom. | 115.18 | 154.78 | 147.80 |
| T. Retorno Prom. | 122.40 | 162.00 | 155.02 |
| CPU Utilización | 99.45% | 99.45% | 99.45% |

FCFS presenta menor tiempo de espera promedio. Round Robin distribuye la CPU más equitativamente.

## Estado del proyecto

- [x] Proceso.h — PCB completo con método reiniciar()
- [x] CargadorDatos — carga CSV con manejo de errores
- [x] FCFS — implementado y verificado
- [x] Round Robin — implementado con quantum configurable
- [x] Metricas — waiting time, turnaround, CPU utilization con tabla de recuadro
- [x] Gantt — diagrama con caracteres de bloque █ y recuadro doble ╔╗╚╝
- [x] UI — colores ANSI, caracteres de recuadro, pausas interactivas
- [x] Dataset 10 procesos
- [x] Dataset 50 procesos extendido
- [x] Resultados experimentales guardados
- [x] Documento IEEE completo
- [x] Compilación desde VS Code (tasks.json con dos tareas)
- [x] Sin errores de compilación ni enlace