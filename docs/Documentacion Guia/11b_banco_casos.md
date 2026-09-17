# BANCO DE CASOS EMPRESARIALES
## TIIT2007 – Sistemas Operativos | Universidad Invenio

Casos contextualizados al entorno empresarial costarricense/latinoamericano, diseñados para discusión guiada, evaluación formativa o insumo de Defensa Técnica. Cada caso vincula explícitamente con una o más unidades del curso.

---

### CASO 1 — El sistema de facturación que se congela (Unidad II)
Una pyme de servicios en Heredia usa un sistema de facturación que se vuelve extremadamente lento cada fin de mes, cuando contabilidad genera cientos de reportes simultáneamente. El equipo de soporte reporta que "la CPU está al 100%" pero no entienden por qué unos reportes terminan rápido y otros tardan minutos.
**Preguntas guía:** ¿Qué algoritmo de planificación podría explicar por qué algunos reportes "se quedan esperando"? ¿Qué recomendarías investigar primero (prioridades, quantum, cantidad de procesos)?

### CASO 2 — Doble cobro en el sistema de caja (Unidad III)
Una cadena de tiendas reporta que, en horas pico, algunos clientes son cobrados dos veces por la misma compra cuando dos cajeros usan el sistema simultáneamente desde la misma terminal de backend.
**Preguntas guía:** ¿Qué tipo de problema describe este caso? ¿Qué mecanismo de sincronización recomendarías implementar en el sistema de caja?

### CASO 3 — El servidor que "se llena de memoria" cada tarde (Unidad IV)
El servidor de un sistema de gestión documental empresarial empieza a responder lento cada tarde; el administrador nota que la memoria RAM está casi al 100% y que el disco "trabaja mucho" aunque nadie está copiando archivos grandes.
**Preguntas guía:** ¿Qué fenómeno estudiado en el curso podría explicar el comportamiento del disco? ¿Qué medirías para confirmarlo?

### CASO 4 — La fuga de información en el servidor compartido (Unidad V)
En una empresa, un practicante descubre que puede leer archivos de nómina de otros departamentos porque "nadie configuró los permisos". El gerente de TI pide una auditoría rápida.
**Preguntas guía:** ¿Qué conceptos de permisos (Unix/ACL) aplicarías para diagnosticar el problema? ¿Qué comandos usarías en Linux y cuáles en Windows para auditar permisos?

### CASO 5 — La impresora que detiene todo el sistema (Unidad VI)
Un usuario reporta que, al imprimir un documento grande, su computadora "se congela" por varios segundos. Un compañero con una impresora más moderna no tiene este problema.
**Preguntas guía:** ¿Qué mecanismo de E/S (interrupciones, DMA, polling) podría explicar la diferencia? ¿Qué rol cumple el driver de cada impresora?

### CASO 6 — Migración a contenedores (Unidad VII)
Una empresa de desarrollo de software decide migrar su aplicación, antes desplegada en máquinas virtuales completas, hacia contenedores Docker, argumentando "ahorro de recursos". El equipo de operaciones no entiende técnicamente por qué esto ahorraría recursos.
**Preguntas guía:** ¿Cómo explicarías, en términos de procesos y aislamiento del kernel, por qué los contenedores consumen menos recursos que las VMs? ¿Qué se gana y qué se pierde con esta migración?

### CASO 7 — El sistema distribuido que "a veces falla" (Unidad VII)
Una aplicación empresarial distribuida entre tres servidores presenta inconsistencias: a veces un cliente ve datos desactualizados al consultar desde distintos servidores.
**Preguntas guía:** ¿Qué propiedad de los sistemas distribuidos (transparencia, tolerancia a fallos, consistencia) está en juego? (Caso de discusión conceptual — no requiere implementación, conforme al alcance del curso.)

### CASO 8 — Auditoría de Invenio Fest: Proyecto Semilla (Integrador)
Un equipo del Proyecto Semilla institucional pide apoyo técnico para identificar qué partes de su prototipo dependen críticamente de procesos en segundo plano, uso de memoria y permisos de archivos antes de la presentación en Invenio Fest.
**Preguntas guía:** Diseña un checklist de 5 puntos (basado en las unidades del curso) que el equipo del Proyecto Semilla debería revisar antes de su presentación.

---

## Nota de Uso Docente

Estos casos pueden emplearse como apertura de clase (15 min de discusión dirigida), como pregunta de quiz formativo, o como insumo adicional de la Defensa Técnica Individual (el docente puede plantear una variación de un caso como pregunta no anticipada, conforme a la Rúbrica de Defensa Técnica).
