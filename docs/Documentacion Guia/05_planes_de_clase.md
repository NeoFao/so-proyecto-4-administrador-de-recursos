# PLANES DE CLASE — 11 SEMANAS
## TIIT2007 – Sistemas Operativos | Universidad Invenio

Formato estándar por semana: Objetivo de sesión → Activación → Desarrollo (Teoría/xv6/Profesional/C++) → Cierre → Evidencia de aprendizaje. Sesión = 4 horas (mixta).

---

## SEMANA 1 — Arquitectura de SO (parte 1)

**Objetivo de sesión:** El estudiante explica la evolución histórica y las funciones esenciales de un sistema operativo, diferenciando kernel y user space.

| Bloque | Duración | Actividad |
|---|---|---|
| Activación | 15 min | Pregunta detonadora: "¿Qué pasa exactamente entre que enciendes la PC y ves el escritorio?" |
| Teoría | 60 min | Línea de tiempo de evolución de SO (batch → time-sharing → multiusuario → SO modernos); funciones esenciales (gestión de procesos, memoria, archivos, dispositivos) |
| Teoría | 45 min | Kernel vs. user space; modos de ejecución (kernel mode / user mode); llamadas al sistema (system calls) |
| Descanso | 10 min | — |
| Profesional | 60 min | Instalación y verificación del entorno de laboratorio: toolchain xv6 (QEMU), terminal Linux, PowerShell |
| Cierre | 30 min | Discusión guiada: ¿por qué separar kernel de user space? Riesgos de no hacerlo |

**Evidencia de aprendizaje:** Bitácora de instalación de entorno (registro fotográfico/log de terminal).
**Material docente:** Diapositivas Unidad I parte 1; capítulos correspondientes de Silberschatz cap. 1–2 y Tanenbaum cap. 1.

---

## SEMANA 2 — Arquitectura de SO (parte 2) + Apertura Proyecto 1

**Objetivo de sesión:** El estudiante compara arquitecturas monolíticas, microkernel e híbridas, y reconoce su implementación conceptual en xv6.

| Bloque | Duración | Actividad |
|---|---|---|
| Activación | 15 min | Repaso rápido (kahoot/quiz oral) de Semana 1 |
| Teoría | 60 min | Arquitecturas monolíticas, microkernel, híbridas; ventajas/desventajas; ejemplos reales (Linux=monolítico modular, Minix=microkernel, Windows NT=híbrido) |
| xv6 | 60 min | Recorrido guiado de la estructura general del repositorio xv6; ubicación de boot, kernel, user programs (solo lectura/observación) |
| Descanso | 10 min | — |
| Profesional | 45 min | Comandos de inspección de arquitectura en Linux (`uname -a`, `lscpu`) y Windows (`systeminfo`) |
| Cierre | 30 min | Apertura formal Proyecto 1: entrega de especificación, formación de criterios de trabajo individual |

**Evidencia de aprendizaje:** Guía de análisis xv6 – Unidad I (entregable).
**Tarea:** Lectura previa OSTEP cap. "Processes" para Semana 3.

---

## SEMANA 3 — Procesos y Scheduling (parte 1)

**Objetivo de sesión:** El estudiante describe el ciclo de vida de un proceso y su representación en el PCB.

| Bloque | Duración | Actividad |
|---|---|---|
| Activación | 15 min | Diagrama en blanco: estudiantes intentan dibujar estados de un proceso antes de la explicación |
| Teoría | 75 min | Proceso vs. programa; PCB; estados (new, ready, running, waiting, terminated); transiciones; context switching |
| Teoría | 30 min | Threads vs. procesos; introducción a IPC (pipes, mensajes, memoria compartida) |
| Descanso | 10 min | — |
| Profesional | 60 min | Bash: `ps aux`, `top`, `htop`; PowerShell: `Get-Process`; identificación de PID, estado, prioridad |
| Cierre | 30 min | Avance guiado de Proyecto 1: diseño de estructura de datos para representar procesos en C++ |

**Evidencia de aprendizaje:** Estructura base del simulador (clase `Proceso`/`PCB` en C++).

---

## SEMANA 4 — Procesos y Scheduling (parte 2) — Cierre Proyecto 1

**Objetivo de sesión:** El estudiante implementa y compara algoritmos de planificación de procesos.

| Bloque | Duración | Actividad |
|---|---|---|
| Activación | 15 min | Caso: cola de impresión — ¿qué orden de atención es más justo? |
| Teoría | 60 min | FCFS, SJF, Round Robin, prioridades; cálculo manual de tiempo de espera/retorno con ejemplos |
| xv6 | 45 min | Lectura guiada de `scheduler()` en xv6: identificación de la política de planificación implementada |
| Descanso | 10 min | — |
| Profesional | 30 min | Bash `nice`/`renice`; Windows administrador de tareas — ajuste de prioridad |
| Laboratorio dirigido | 60 min | Trabajo autónomo supervisado: finalización de Proyecto 1 |
| Cierre | 20 min | Checklist de entrega: código, resultados experimentales, documento IEEE |

**Evidencia de aprendizaje:** **Proyecto 1 completo + Documento IEEE (18%)**. Guía de análisis xv6 – Unidad II.

---

## SEMANA 5 — Concurrencia y Sincronización (parte 1)

**Objetivo de sesión:** El estudiante identifica condiciones de carrera y aplica mecanismos básicos de exclusión mutua.

| Bloque | Duración | Actividad |
|---|---|---|
| Activación | 15 min | Demostración en vivo: dos hilos incrementando una variable compartida sin protección — resultado inesperado |
| Teoría | 60 min | Race conditions; sección crítica; exclusión mutua; mutex |
| Teoría | 45 min | Semáforos (contadores y binarios); comparación mutex vs. semáforo |
| Descanso | 10 min | — |
| xv6 | 45 min | Observación de `spinlock` en el kernel de xv6: ¿dónde y por qué se usa? |
| Cierre | 30 min | Apertura Proyecto 2: especificación y formación de plan de trabajo |

**Evidencia de aprendizaje:** Código de demostración de race condition documentado en bitácora.

---

## SEMANA 6 — Concurrencia y Sincronización (parte 2) — Cierre Proyecto 2

**Objetivo de sesión:** El estudiante implementa una solución sincronizada a un problema clásico de concurrencia y documenta evidencia comparativa.

| Bloque | Duración | Actividad |
|---|---|---|
| Activación | 15 min | Pregunta: ¿qué pasaría si dos filósofos comensales toman el mismo tenedor? |
| Teoría | 60 min | Deadlocks: condiciones necesarias (Coffman), estrategias de prevención/evitación; casos clásicos: Productor-Consumidor, Lectores-Escritores, Filósofos Comensales |
| Profesional | 45 min | Laboratorio comparativo de concurrencia con scripts controlados (Bash/PowerShell) |
| Descanso | 10 min | — |
| Laboratorio dirigido | 70 min | Trabajo autónomo supervisado: finalización de Proyecto 2 (Productor-Consumidor con threads y mutex) |
| Cierre | 20 min | Checklist de entrega: comparación con/sin sincronización, evidencia de condición de carrera |

**Evidencia de aprendizaje:** **Proyecto 2 completo + Documento IEEE (18%)**. Guía de análisis xv6 – Unidad III.

---

## SEMANA 7 — Gestión de Memoria (parte 1)

**Objetivo de sesión:** El estudiante diferencia memoria física y virtual, y explica los mecanismos de paginación y segmentación.

| Bloque | Duración | Actividad |
|---|---|---|
| Activación | 15 min | Analogía: biblioteca con estanterías (paginación) vs. secciones temáticas (segmentación) |
| Teoría | 75 min | Memoria física vs. virtual; espacio de direcciones; paginación (tablas de páginas, traducción de direcciones); segmentación |
| Descanso | 10 min | — |
| xv6 | 45 min | Lectura guiada de `vm.c`: implementación de memoria virtual en xv6 |
| Profesional | 45 min | Linux `free`, `vmstat`; Windows Administrador de tareas / `Get-Counter` (memoria) |
| Cierre | 20 min | Apertura Proyecto 3: especificación |

**Evidencia de aprendizaje:** Diagrama propio de traducción de direcciones virtuales a físicas.

---

## SEMANA 8 — Gestión de Memoria (parte 2) — Cierre Proyecto 3

**Objetivo de sesión:** El estudiante implementa y compara algoritmos de reemplazo de páginas (FIFO, LRU).

| Bloque | Duración | Actividad |
|---|---|---|
| Activación | 15 min | Caso: caché de navegador llena — ¿qué página eliminar? |
| Teoría | 60 min | Swapping, thrashing; algoritmos FIFO y LRU; cálculo manual de fallos de página con cadena de referencias |
| Profesional | 30 min | Laboratorio de presión de memoria (observación de comportamiento del sistema bajo carga) |
| Descanso | 10 min | — |
| Laboratorio dirigido | 75 min | Trabajo autónomo supervisado: finalización de Proyecto 3 |
| Cierre | 20 min | Checklist de entrega: fallos de página, comparación de algoritmos, métricas |

**Evidencia de aprendizaje:** **Proyecto 3 completo + Documento IEEE (18%)**. Guía de análisis xv6 – Unidad IV.

---

## SEMANA 9 — Sistemas de Archivos y Seguridad

**Objetivo de sesión:** El estudiante analiza la organización de sistemas de archivos y los mecanismos de permisos/seguridad.

| Bloque | Duración | Actividad |
|---|---|---|
| Activación | 15 min | Pregunta: ¿qué es realmente un archivo para el sistema operativo? |
| Teoría | 60 min | Archivos, directorios, inodos; comparación NTFS vs. ext4 |
| Teoría | 30 min | ACL, usuarios, grupos, permisos |
| Descanso | 10 min | — |
| xv6 | 45 min | Lectura guiada de `fs.c`: sistema de archivos de xv6 |
| Profesional | 60 min | Linux `chmod`, `chown`, `ls -l`; Windows gestión de ACL vía PowerShell (`Get-Acl`, `Set-Acl`) |
| Cierre | 20 min | Apertura Proyecto 4 (componente de gestión de archivos) |

**Evidencia de aprendizaje:** Guía de análisis xv6 – Unidad V. Ejercicios de permisos resueltos (Bash/PowerShell).

---

## SEMANA 10 — Entrada, Salida y Administración de Recursos

**Objetivo de sesión:** El estudiante explica los mecanismos de E/S y monitorea recursos del sistema en entornos reales.

| Bloque | Duración | Actividad |
|---|---|---|
| Activación | 15 min | Pregunta: ¿por qué una impresora no congela la CPU mientras imprime? |
| Teoría | 60 min | Interrupciones; DMA; drivers; nociones de SSD/NVMe |
| Profesional | 90 min | Laboratorio integral de monitoreo: CPU, memoria y procesos en Linux y Windows; construcción de panel comparativo de métricas |
| Descanso | 10 min | — |
| Laboratorio dirigido | 45 min | Avance significativo de Proyecto 4 (monitoreo de procesos/memoria) |

**Evidencia de aprendizaje:** Avance documentado de Proyecto 4; tabla comparativa de métricas Linux vs. Windows.

---

## SEMANA 11 — Virtualización, Contenedores y Sistemas Modernos — Cierre Proyecto 4

**Objetivo de sesión:** El estudiante explica conceptualmente virtualización, contenedores y fundamentos de sistemas distribuidos; finaliza Proyecto 4.

| Bloque | Duración | Actividad |
|---|---|---|
| Activación | 15 min | Pregunta: ¿en qué se diferencia una VM de un contenedor? |
| Teoría | 50 min | Hipervisores (tipo 1 y 2), máquinas virtuales, Docker, contenedores |
| Teoría | 40 min | Introducción conceptual a Cloud Computing y sistemas distribuidos: transparencia, escalabilidad, tolerancia a fallos (**sin implementación**) |
| Descanso | 10 min | — |
| Laboratorio dirigido | 75 min | Finalización de Proyecto 4: gestión básica de archivos, monitoreo de procesos/memoria, pruebas de carga |
| Cierre | 30 min | Checklist de entrega Proyecto 4; revisión de avance de Portafolio Técnico; preparación logística para Invenio Fest |

**Evidencia de aprendizaje:** **Proyecto 4 completo + Documento IEEE (18%)**. Primera versión del Portafolio Técnico Final.

---

## SEMANA 12 — INVENIO FEST (ver documento dedicado de Fase 3/4)

Sesión fuera del formato de clase regular. Evaluación de Participación Técnica en Invenio Fest (10%), Defensa Técnica Individual (8%) y entrega del Portafolio Técnico Final (10%).
