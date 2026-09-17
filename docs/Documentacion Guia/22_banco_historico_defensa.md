# BANCO HISTÓRICO DE PREGUNTAS DE DEFENSA TÉCNICA
## TIIT2007 – Sistemas Operativos | Universidad Invenio

---

## 1. Propósito

El Banco de 100 Preguntas Conceptuales (Fase 3) es un banco **estático** de apoyo general. Este documento es distinto: es un **registro vivo**, acumulativo entre cohortes, de las preguntas de variación realmente formuladas durante las Defensas Técnicas — junto con qué tan bien funcionaron para detectar comprensión genuina. Su objetivo es profesionalizar y dar consistencia a la Defensa Técnica entre distintos docentes y secciones a lo largo del tiempo.

---

## 2. Diferencia con el Banco de 100 Preguntas Conceptuales

| | Banco de 100 Preguntas (Fase 3) | Banco Histórico de Defensa (este documento) |
|---|---|---|
| Naturaleza | Estático, preparado antes del curso | Vivo, crece cada cohorte |
| Tipo de pregunta | Conceptual general por unidad | Pregunta de variación atada a un proyecto específico del estudiante |
| Uso principal | Quizzes formativos, autoestudio | Defensa Técnica Individual |
| Quién lo alimenta | Diseño curricular inicial | Cada docente, después de cada Defensa Técnica |

---

## 3. Estructura del Registro

Cada entrada debe documentarse inmediatamente después de la Defensa Técnica en la que se usó (no de memoria semanas después).

| Campo | Descripción |
|---|---|
| Cohorte/Trimestre | Ej. "2026-T3" |
| Proyecto asociado | 1, 2, 3 o 4 |
| Pregunta formulada | Texto exacto de la pregunta de variación |
| Tipo de variación | Cambio de parámetro / Caso límite / Hipotético "¿qué pasaría si...?" / Comparación con xv6 / Conexión con caso empresarial |
| Efectividad observada | Alta (distinguió claramente comprensión real vs. superficial) / Media / Baja (la mayoría respondió igual de bien sin importar su dominio real) |
| Nota del docente | Observación libre — p. ej. "buena para detectar uso no comprendido de mutex" |

---

## 4. Banco Inicial — Preguntas Semilla por Proyecto

(Punto de partida; cada cohorte debe añadir nuevas entradas según la tabla de la sección 3. Estas preguntas semilla no tienen aún datos de efectividad — se completan con el uso real.)

### Proyecto 1 — Scheduling
- "Si en lugar de Round Robin con quantum fijo usaras un quantum que cambia dinámicamente según la carga, ¿qué tendrías que modificar en tu código y qué efecto esperarías en el tiempo de espera promedio?"
- "¿Qué pasaría con tus resultados si dos procesos llegan exactamente en el mismo tiempo de llegada? ¿Tu implementación lo maneja correctamente?"
- "Si tuvieras que agregar un quinto algoritmo (SJF) a tu simulador, ¿qué parte de tu diseño actual reutilizarías y qué tendrías que rediseñar?"

### Proyecto 2 — Concurrencia
- "¿Qué pasaría si tu buffer compartido tuviera tamaño 1 en lugar de 5? ¿Cambiaría el riesgo de condición de carrera o solo la frecuencia con la que ocurre?"
- "Si agregaras un tercer tipo de hilo (por ejemplo, un 'inspector' que solo lee el buffer sin consumir), ¿qué nuevo mecanismo de sincronización necesitarías investigar?"
- "¿Tu solución de mutex podría producir un deadlock bajo alguna condición que no probaste? ¿Cómo lo verificarías?"

### Proyecto 3 — Gestión de Memoria
- "¿Qué pasaría con tus resultados de FIFO si el tamaño de marco fuera 1? ¿Es un caso límite que tu código maneja bien?"
- "Si tu cadena de referencias tuviera un patrón completamente secuencial (0,1,2,3,4,5...) en lugar de tener localidad, ¿esperarías que LRU siga superando a FIFO? ¿Por qué sí o no?"
- "¿Cómo extenderías tu simulador para implementar el algoritmo del reloj (Clock/Segunda oportunidad)? ¿Qué estructura de datos cambiarías?"

### Proyecto 4 — Administrador de Recursos
- "Si tu herramienta tuviera que monitorear 10,000 archivos en lugar de 1,000, ¿qué parte de tu implementación esperarías que se vuelva el cuello de botella?"
- "¿Qué pasaría si el sistema anfitrión no tiene permisos para que tu programa lea la lista de procesos? ¿Tu código lo maneja con un error claro o falla silenciosamente?"
- "Si tuvieras que portar tu herramienta del sistema operativo en el que la probaste al otro (Linux↔Windows), ¿qué partes específicas de tu código tendrías que reescribir?"

### Preguntas Integradoras (cualquier proyecto / cierre de Defensa)
- "De los cuatro mecanismos de SO que estudiaste, ¿cuál crees que tiene mayor impacto práctico en el rendimiento de un sistema empresarial real, y por qué?"
- "Si tuvieras que explicarle a un gerente no técnico por qué tu Proyecto [X] importa para el negocio, ¿qué le dirías en dos frases?"

---

## 5. Protocolo de Actualización del Banco

| Momento | Acción | Responsable |
|---|---|---|
| Durante cada Defensa Técnica | Registrar la(s) pregunta(s) de variación formulada(s) | Docente a cargo |
| Al cierre de cada cohorte | Consolidar nuevas entradas en este documento, eliminar/marcar como "agotadas" las preguntas que ya son de conocimiento común entre estudiantes (riesgo de filtración entre cohortes) | Docente titular |
| Anualmente | Revisar el banco completo y retirar preguntas de baja efectividad reportada | Coordinación académica + docente titular |

---

## 6. Advertencia de Uso

- **No reutilizar la misma pregunta exacta en cohortes consecutivas de forma predecible** — el valor de la pregunta de variación depende de que no pueda prepararse de memoria. Se recomienda rotar y combinar preguntas semilla con variaciones nuevas generadas a partir del proyecto específico de cada estudiante.
- Este banco es de uso docente interno; no debe compartirse con estudiantes antes de su Defensa Técnica.
