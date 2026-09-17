# CRONOGRAMA DETALLADO — 11 SEMANAS EFECTIVAS
## TIIT2007 – Sistemas Operativos | Universidad Invenio

Cada semana = 4 horas (modalidad mixta: presencial + virtual sincrónica). Toda semana sigue, cuando aplica, la secuencia: **Teoría → xv6 → Linux/Windows → Simulación C++ → IEEE/ISO → (Invenio Fest en semana 12)**.

---

### SEMANA 1 — Unidad I: Arquitectura de Sistemas Operativos (parte 1)
- **Teoría:** Historia y evolución de los SO; funciones del sistema operativo; kernel vs. user space.
- **xv6:** Estructura general del código fuente; proceso de boot.
- **Profesional:** Instalación y configuración del entorno de laboratorio (Linux + xv6 + entorno Windows/PowerShell).
- **Entregable:** Ninguno formal. Inicio de bitácora de laboratorio.

### SEMANA 2 — Unidad I: Arquitectura de Sistemas Operativos (parte 2)
- **Teoría:** Arquitecturas monolíticas, microkernel e híbridas; comparación crítica.
- **xv6:** Recorrido del kernel y de los programas de usuario (user programs).
- **Profesional:** Identificación de arquitectura en Linux y Windows (comandos de inspección del sistema).
- **Entregable:** Guía de análisis de xv6 – Unidad I (ver documento dedicado, Fase 2).
- **Apertura:** Proyecto 1 (Simulador de Scheduling).

### SEMANA 3 — Unidad II: Procesos y Scheduling (parte 1)
- **Teoría:** Procesos, PCB, estados de un proceso, context switching, threads, IPC.
- **xv6:** Lectura de `proc.c` — estructura del PCB en xv6.
- **Profesional:** Bash — `ps`, `top`, `htop`; PowerShell — `Get-Process`.
- **Entregable:** Avance Proyecto 1 (estructura base del simulador).

### SEMANA 4 — Unidad II: Procesos y Scheduling (parte 2)
- **Teoría:** Algoritmos FCFS, SJF, Round Robin, prioridades.
- **xv6:** Lectura de `scheduler()` — política de planificación de xv6.
- **Profesional:** Comparación de prioridades de procesos en Linux (`nice`/`renice`) y Windows (administrador de tareas).
- **Entregable:** **Proyecto 1 completo + Documento IEEE (18%)**. Guía de análisis xv6 – Unidad II.

### SEMANA 5 — Unidad III: Concurrencia y Sincronización (parte 1)
- **Teoría:** Race conditions, exclusión mutua, mutex, semáforos.
- **xv6:** Mecanismos de bloqueo (`spinlock`) en el kernel de xv6.
- **Profesional:** Observación de hilos y procesos concurrentes en Linux/Windows.
- **Entregable:** Apertura Proyecto 2 (Simulador de Concurrencia).

### SEMANA 6 — Unidad III: Concurrencia y Sincronización (parte 2)
- **Teoría:** Deadlocks; casos clásicos — Productor-Consumidor, Lectores-Escritores, Filósofos Comensales.
- **Profesional:** Laboratorio comparativo de condiciones de carrera (scripts Bash/PowerShell controlados).
- **Entregable:** **Proyecto 2 completo + Documento IEEE (18%)**. Guía de análisis xv6 – Unidad III.

### SEMANA 7 — Unidad IV: Gestión de Memoria (parte 1)
- **Teoría:** Memoria física y virtual; paginación; segmentación.
- **xv6:** Lectura de `vm.c` — gestión de memoria virtual en xv6.
- **Profesional:** Monitoreo de memoria en Linux (`free`, `vmstat`) y Windows (Administrador de tareas / `Get-Counter`).
- **Entregable:** Apertura Proyecto 3 (Simulador de Gestión de Memoria).

### SEMANA 8 — Unidad IV: Gestión de Memoria (parte 2)
- **Teoría:** Swapping, thrashing; algoritmos FIFO y LRU.
- **Profesional:** Laboratorio de presión de memoria y observación de comportamiento del sistema.
- **Entregable:** **Proyecto 3 completo + Documento IEEE (18%)**. Guía de análisis xv6 – Unidad IV.

### SEMANA 9 — Unidad V: Sistemas de Archivos y Seguridad
- **Teoría:** Archivos, directorios, inodos; NTFS vs. ext4; ACL, usuarios, grupos, permisos.
- **xv6:** Lectura de `fs.c` — sistema de archivos de xv6.
- **Profesional:** Gestión de permisos en Linux (`chmod`, `chown`) y Windows (ACL vía PowerShell).
- **Entregable:** Apertura Proyecto 4 (Administrador Simplificado de Recursos). Guía de análisis xv6 – Unidad V.

### SEMANA 10 — Unidad VI: Entrada, Salida y Administración de Recursos
- **Teoría:** Interrupciones, DMA, drivers, SSD/NVMe.
- **Profesional:** Monitoreo integral de CPU, memoria y procesos en Linux y Windows.
- **Entregable:** Avance significativo Proyecto 4.

### SEMANA 11 — Unidad VII: Virtualización, Contenedores y Sistemas Modernos
- **Teoría:** Hipervisores, máquinas virtuales, Docker, contenedores; introducción conceptual a Cloud Computing y sistemas distribuidos (transparencia, escalabilidad, tolerancia a fallos). **No se implementan sistemas distribuidos.**
- **Profesional:** Laboratorio introductorio de contenedores (observación, no administración avanzada).
- **Entregable:** **Proyecto 4 completo + Documento IEEE (18%)**. Cierre de Portafolio Técnico (primera versión). Preparación de material para Invenio Fest.

### SEMANA 12 — INVENIO FEST
- Evento institucional interdisciplinario.
- Evaluación exclusiva desde la perspectiva de Sistemas Operativos sobre el Proyecto Semilla: procesos, memoria, recursos, almacenamiento, seguridad, virtualización.
- **Entregables:** Póster, presentación, sección técnica y evidencias del curso.
- **Entregable de curso:** **Participación Técnica en Invenio Fest (10%)**, **Defensa Técnica Individual (8%)**, **Portafolio Técnico Final (10%)**.

---

## Resumen de Entregables Evaluados por Semana

| Semana | Entregable evaluado | Peso |
|---|---|---|
| 4 | Proyecto 1 + IEEE | 18% |
| 6 | Proyecto 2 + IEEE | 18% |
| 8 | Proyecto 3 + IEEE | 18% |
| 11 | Proyecto 4 + IEEE | 18% |
| 12 | Invenio Fest | 10% |
| 12 | Defensa Técnica Individual | 8% |
| 12 | Portafolio Técnico Final | 10% |
| | **Total** | **100%** |

---

## Notas de Implementación Docente

- Las "Guías de análisis de xv6" deben entregarse como apoyo formativo (no implican modificación de código) y pueden evaluarse formativamente sin peso directo en la nota final, o integrarse como evidencia del Portafolio.
- Cada proyecto tiene dos semanas de desarrollo (apertura + cierre), excepto el Proyecto 4, que se extiende sobre 3 semanas (9–11) dada su mayor integración de subsistemas.
- Se recomienda reservar los primeros 20–30 minutos de cada sesión presencial para resolución de dudas técnicas del proyecto en curso.
