# BANCO DE 100 PREGUNTAS CONCEPTUALES
## TIIT2007 – Sistemas Operativos | Universidad Invenio

Organizado por unidad temática. Uso sugerido: banco de apoyo para quizzes formativos, Defensa Técnica Individual y autoestudio. Las preguntas marcadas (★) son de mayor profundidad/análisis, recomendadas para Defensa Técnica.

---

### Unidad I — Arquitectura de Sistemas Operativos (Preguntas 1–14)

1. ¿Qué es un sistema operativo y cuáles son sus funciones esenciales?
2. ¿Cuál es la diferencia entre kernel mode y user mode?
3. ¿Qué es una llamada al sistema (system call) y por qué es necesaria?
4. Describe la diferencia entre una arquitectura monolítica y una microkernel.
5. ¿Qué ventajas ofrece un microkernel frente a un kernel monolítico? ¿Y qué desventajas?
6. ¿Por qué se dice que Linux es un kernel "monolítico modular"? ★
7. ¿Qué es un sistema operativo híbrido? Da un ejemplo real.
8. Explica el proceso general de boot de un sistema operativo.
9. ¿Qué diferencia hay entre firmware/BIOS-UEFI y el sistema operativo?
10. ¿Por qué separar el espacio de kernel del espacio de usuario mejora la estabilidad del sistema? ★
11. Menciona tres funciones del sistema operativo relacionadas con la gestión de recursos.
12. ¿Qué es un driver y en qué capa del sistema opera?
13. ¿Cómo evolucionaron los sistemas operativos desde el procesamiento por lotes (batch) hasta los sistemas de tiempo compartido?
14. En xv6, ¿qué archivo(s) representan el punto de entrada del proceso de boot? ★

### Unidad II — Procesos y Scheduling (Preguntas 15–32)

15. ¿Qué es un proceso y en qué se diferencia de un programa?
16. ¿Qué información típicamente contiene un PCB (Process Control Block)?
17. Enumera y describe los estados clásicos de un proceso.
18. ¿Qué es un context switch y por qué tiene costo de rendimiento?
19. ¿Qué es un thread y en qué se diferencia de un proceso?
20. ¿Qué es IPC (comunicación entre procesos) y menciona dos mecanismos.
21. Describe el algoritmo FCFS y una desventaja clave.
22. Describe el algoritmo SJF y el problema de inanición (starvation) que puede generar. ★
23. ¿Cómo funciona Round Robin y qué papel juega el quantum?
24. ¿Qué ocurre si el quantum de Round Robin es demasiado pequeño? ¿Y si es demasiado grande? ★
25. ¿Qué es un algoritmo de planificación por prioridades y qué riesgo conlleva?
26. ¿Qué es la inversión de prioridades? ★
27. Calcula el tiempo de espera promedio para un conjunto simple de 3 procesos bajo FCFS (ejercicio numérico, ver banco de ejercicios).
28. ¿Qué significa "utilización de CPU" en el contexto de planificación?
29. ¿Qué función cumple `scheduler()` en xv6?
30. ¿xv6 implementa un algoritmo de planificación con o sin apropiación (preemptive)? Justifica. ★
31. ¿Qué es un proceso "zombie" y cómo se origina?
32. ¿Qué diferencia hay entre concurrencia y paralelismo? ★

### Unidad III — Concurrencia y Sincronización (Preguntas 33–50)

33. ¿Qué es una condición de carrera (race condition)?
34. ¿Qué es una sección crítica?
35. ¿Qué es la exclusión mutua y por qué es necesaria?
36. ¿Qué es un mutex y cómo funciona a nivel conceptual?
37. ¿Qué es un semáforo y en qué se diferencia de un mutex?
38. ¿Qué diferencia hay entre un semáforo binario y uno contador?
39. ¿Qué es un spinlock y cuándo es preferible a un lock que "duerme"?
40. Describe el problema del Productor-Consumidor.
41. ¿Qué rol cumple el buffer en el problema Productor-Consumidor?
42. Describe el problema de los Lectores-Escritores.
43. Describe el problema de los Filósofos Comensales.
44. ¿Cuáles son las cuatro condiciones necesarias para que ocurra un deadlock (condiciones de Coffman)? ★
45. Menciona una estrategia de prevención de deadlocks.
46. ¿Qué diferencia hay entre prevención, evitación y detección de deadlocks? ★
47. ¿Qué es la inanición (starvation) y en qué se diferencia de un deadlock?
48. ¿Cómo usa xv6 el mecanismo de `spinlock` en su kernel?
49. ¿Qué evidencia observable indica que un programa concurrente tiene una condición de carrera? ★
50. ¿Por qué agregar sincronización introduce overhead de rendimiento? ★

### Unidad IV — Gestión de Memoria (Preguntas 51–66)

51. ¿Qué diferencia hay entre memoria física y memoria virtual?
52. ¿Qué es el espacio de direcciones de un proceso?
53. Explica el concepto de paginación.
54. Explica el concepto de segmentación.
55. ¿Qué ventajas ofrece la memoria virtual frente al uso directo de memoria física? ★
56. ¿Qué es una tabla de páginas?
57. ¿Qué es un fallo de página (page fault)?
58. ¿Qué es el swapping?
59. ¿Qué es el thrashing y por qué es perjudicial para el rendimiento? ★
60. Describe el algoritmo de reemplazo de páginas FIFO.
61. Describe el algoritmo de reemplazo de páginas LRU.
62. ¿Por qué LRU generalmente supera a FIFO, y en qué casos no? ★
63. ¿Qué es la anomalía de Belady? ★
64. ¿Qué función cumple `vm.c` dentro del kernel de xv6?
65. ¿xv6 implementa reemplazo de páginas? ¿Qué implica esto sobre su simplicidad pedagógica? ★
66. ¿Cómo se traduce una dirección virtual a una dirección física en un sistema paginado? ★

### Unidad V — Sistemas de Archivos y Seguridad (Preguntas 67–80)

67. ¿Qué es un sistema de archivos?
68. ¿Qué es un inodo y qué información típicamente contiene?
69. Compara conceptualmente ext4 y NTFS.
70. ¿Qué es una ACL (Access Control List)?
71. ¿Qué diferencia hay entre el modelo de permisos clásico de Unix (rwx) y el modelo ACL de Windows? ★
72. ¿Qué representan los tres dígitos de un permiso octal en Linux (p. ej. 750)?
73. ¿Qué es un directorio desde la perspectiva del sistema de archivos?
74. ¿Qué riesgo de seguridad ilustra un permiso de archivo mal configurado?
75. ¿Qué función cumple `fs.c` en xv6?
76. ¿Cómo navega xv6 desde un inodo hasta los bloques de datos de un archivo? ★
77. ¿Qué es un usuario y un grupo en el contexto de permisos de sistema operativo?
78. ¿Por qué los sistemas de archivos modernos usan journaling? ★
79. ¿Qué diferencia hay entre eliminar un archivo y eliminar su entrada de directorio? ★
80. ¿Qué riesgos de seguridad existen si un proceso corre con privilegios elevados innecesariamente?

### Unidad VI — Entrada, Salida y Administración de Recursos (Preguntas 81–90)

81. ¿Qué es una interrupción y por qué es fundamental para la E/S eficiente?
82. ¿Qué es el DMA (Direct Memory Access) y qué problema resuelve?
83. ¿Qué es un driver de dispositivo?
84. ¿Qué diferencia hay entre E/S programada (polling) y E/S por interrupciones? ★
85. ¿Qué ventajas ofrece una unidad SSD/NVMe frente a un disco mecánico, desde la perspectiva del SO?
86. ¿Qué métricas son relevantes al monitorear el uso de CPU de un sistema?
87. ¿Qué métricas son relevantes al monitorear el uso de memoria de un sistema?
88. ¿Por qué es importante el monitoreo continuo de procesos en un entorno empresarial?
89. ¿Qué herramientas de monitoreo conoces para Linux y cuáles para Windows?
90. ¿Cómo se relaciona el manejo de interrupciones con la capacidad multitarea de un SO? ★

### Unidad VII — Virtualización, Contenedores y Sistemas Modernos (Preguntas 91–100)

91. ¿Qué es un hipervisor y qué diferencia hay entre tipo 1 y tipo 2?
92. ¿Qué es una máquina virtual?
93. ¿Qué es un contenedor y en qué se diferencia fundamentalmente de una máquina virtual? ★
94. ¿Qué papel cumplen los namespaces y cgroups en la implementación de contenedores (a nivel conceptual)? ★
95. ¿Qué es Docker?
96. ¿Qué es la transparencia en el contexto de sistemas distribuidos?
97. ¿Qué es la escalabilidad en el contexto de sistemas distribuidos?
98. ¿Qué es la tolerancia a fallos en el contexto de sistemas distribuidos?
99. ¿Por qué este curso no implementa sistemas distribuidos pese a abordarlos conceptualmente? ★
100. ¿Cómo se conectan los conceptos de procesos, memoria y recursos estudiados durante el curso con el funcionamiento de un contenedor moderno? ★ (pregunta integradora, recomendada para cierre de Defensa Técnica)
