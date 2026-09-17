# GUÍAS DE ANÁLISIS DE xv6
## TIIT2007 – Sistemas Operativos | Universidad Invenio

**Principio rector (aplica a todas las guías):** xv6 se utiliza **exclusivamente con fines de observación y análisis pedagógico**. El estudiante **no modifica, no compila con cambios, ni implementa componentes** de xv6. El objetivo es leer código real y conectar lo observado con la teoría y con el comportamiento de sistemas profesionales (Linux/Windows).

Formato estándar: Archivo(s) a observar → Preguntas guía → Conexión teórica → Conexión profesional → Entregable.

---

## GUÍA 1 — Unidad I: Arquitectura y Boot de xv6

**Archivos a observar:** estructura general del repositorio; `bootasm.S` / `bootmain.c` (proceso de boot); `main.c` (inicialización del kernel).

**Preguntas guía:**
1. ¿Qué hace el sistema en los primeros milisegundos tras el boot, antes de poder ejecutar un programa de usuario?
2. ¿Dónde se traza la línea entre "código de kernel" y "código de usuario" dentro de la estructura de carpetas/archivos?
3. ¿Qué evidencia encuentras de que xv6 es un kernel monolítico simplificado?

**Conexión teórica:** Relacionar con el modelo de arquitectura monolítica visto en Silberschatz/Tanenbaum.

**Conexión profesional:** Comparar conceptualmente con el proceso de boot de Linux (GRUB → kernel → init/systemd) observado vía `dmesg | head` (sin necesidad de modificar nada).

**Entregable:** Diagrama propio del flujo de boot de xv6 (boot → kernel init → primer proceso de usuario) + media página de análisis.

---

## GUÍA 2 — Unidad II: Procesos en xv6 (`proc.c`, `scheduler()`)

**Archivos a observar:** `proc.h` (estructura `struct proc`), `proc.c`, función `scheduler()`.

**Preguntas guía:**
1. ¿Qué campos componen el PCB (`struct proc`) en xv6? ¿Cómo se comparan con el PCB teórico visto en clase?
2. ¿Qué estados de proceso define xv6 (`enum procstate`)? ¿Coinciden con el modelo teórico de 5 estados?
3. ¿Qué algoritmo de planificación implementa `scheduler()`? ¿Es FCFS, Round Robin, u otro? Justifica con evidencia del código.
4. ¿Cómo realiza xv6 el context switch (función `swtch`)?

**Conexión teórica:** Relacionar directamente con FCFS/Round Robin/prioridades estudiados en Unidad II.

**Conexión profesional:** Comparar con la salida de `ps -o pid,stat,cmd` en Linux — ¿qué estados observados ahí corresponden a los definidos en xv6?

**Entregable:** Tabla comparativa "Estados de proceso: xv6 vs. modelo teórico vs. Linux observado" + identificación textual del algoritmo de planificación con cita de línea/función.

---

## GUÍA 3 — Unidad III: Sincronización en xv6 (`spinlock.c`)

**Archivos a observar:** `spinlock.h`, `spinlock.c`; búsqueda de usos de `acquire()`/`release()` en `proc.c` y otros archivos del kernel.

**Preguntas guía:**
1. ¿Qué es un spinlock y en qué se diferencia de un mutex que "duerme" (sleep lock)?
2. ¿En qué situaciones del kernel xv6 utiliza spinlocks? Identifica al menos 2 ejemplos concretos.
3. ¿Qué riesgo (relacionado con condiciones de carrera) previene cada uso identificado?

**Conexión teórica:** Relacionar con exclusión mutua, mutex y semáforos vistos en Unidad III.

**Conexión profesional:** Relacionar con la evidencia de condición de carrera generada en el Laboratorio 3 y con el diseño de sincronización del Proyecto 2.

**Entregable:** Lista de 2–3 usos de `acquire()/release()` localizados en el código, con explicación de qué recurso compartido protegen.

---

## GUÍA 4 — Unidad IV: Memoria Virtual en xv6 (`vm.c`)

**Archivos a observar:** `vm.c`, estructuras de tabla de páginas (`pde_t`, funciones como `walkpgdir`, `mappages`).

**Preguntas guía:**
1. ¿Cómo representa xv6 una tabla de páginas? ¿Qué función traduce una dirección virtual a una entrada de tabla de páginas?
2. ¿xv6 implementa swapping o reemplazo de páginas (FIFO/LRU)? ¿Qué evidencia (presencia o ausencia) encuentras en el código?
3. ¿Cómo se relaciona esto con la simplicidad pedagógica de xv6 frente a un SO de producción?

**Conexión teórica:** Relacionar con paginación, traducción de direcciones y los algoritmos FIFO/LRU de la Unidad IV.

**Conexión profesional:** Comparar con la salida de `free -h`/`vmstat` del Laboratorio 4 — xv6 no tiene gestión de swap comparable a Linux; discutir por qué.

**Entregable:** Explicación de la función `walkpgdir` en lenguaje propio + reflexión sobre qué le falta a xv6 frente a un gestor de memoria de producción.

---

## GUÍA 5 — Unidad V: Sistema de Archivos en xv6 (`fs.c`)

**Archivos a observar:** `fs.h` (estructura `struct dinode`), `fs.c` (funciones de manejo de inodos y bloques).

**Preguntas guía:**
1. ¿Qué información contiene un inodo en xv6 (`struct dinode`)? ¿Cómo se compara con un inodo de ext4?
2. ¿Cómo localiza xv6 los bloques de datos de un archivo a partir de su inodo?
3. ¿xv6 implementa permisos de usuario/grupo equivalentes a Unix (rwx, uid/gid)? ¿Qué tan simplificado es su modelo de seguridad frente a ext4/NTFS?

**Conexión teórica:** Relacionar con inodos, directorios y permisos de la Unidad V.

**Conexión profesional:** Comparar con la salida de `ls -l` y `stat <archivo>` del Laboratorio 5 sobre un sistema ext4 real.

**Entregable:** Diagrama de cómo xv6 navega de inodo → bloques de datos + tabla comparativa "Modelo de permisos: xv6 vs. ext4 vs. NTFS".

---

## Nota Metodológica para el Docente

Estas guías pueden trabajarse en parejas o de forma individual, y se recomienda asignarlas como tarea dirigida posterior a cada sesión teórica (ver Planes de Clase, Fase 2), con una breve puesta en común al inicio de la siguiente sesión. No representan una unidad de evaluación sumativa independiente; se integran como evidencia formativa dentro del Portafolio Técnico Final y como insumo para la Defensa Técnica Individual.
