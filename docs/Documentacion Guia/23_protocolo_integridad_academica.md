# PROTOCOLO DE INTEGRIDAD ACADÉMICA Y MANEJO DE DISCREPANCIAS
## TIIT2007 – Sistemas Operativos | Universidad Invenio

---

## 1. Propósito

La Política de IA del Syllabus establece el principio ("toda solución debe ser comprendida, explicada y defendida"), pero no define **el procedimiento** a seguir cuando, en la práctica, se detecta una discrepancia entre lo entregado y lo que el estudiante puede explicar. Este documento cierra ese vacío con un protocolo aplicable y consistente entre docentes.

---

## 2. Principio Rector

El uso de IA generativa **no es la falta**. La falta es **no poder explicar y defender una solución propia entregada como tal**. Este protocolo trata, por tanto, principalmente una discrepancia de **dominio del aprendizaje**, distinta — aunque puede converger — de un caso de **deshonestidad académica** (plagio, suplantación, entrega de trabajo ajeno sin reconocimiento), que se rige por el reglamento disciplinario institucional vigente de Universidad Invenio, el cual prevalece sobre cualquier disposición de este documento.

---

## 3. Señales de Discrepancia (a observar durante Defensa Técnica)

| Señal | Ejemplo observable |
|---|---|
| Incapacidad de explicar una decisión de diseño propia | No puede justificar por qué eligió `std::vector` sobre una lista enlazada |
| Incapacidad de responder a una pregunta de variación simple | No puede predecir qué pasaría con un cambio menor de parámetro |
| Vocabulario técnico inconsistente con el nivel del código entregado | Código avanzado pero el estudiante no reconoce términos básicos usados en su propio documento IEEE |
| Incapacidad de modificar su propio código en vivo ante un ajuste menor solicitado | Se le pide cambiar el tamaño del buffer y no sabe dónde hacerlo |
| Respuestas genéricas que repiten frases del documento IEEE sin elaboración propia | "Porque es más eficiente" sin poder explicar en qué sentido |

**Importante:** una sola señal aislada no es evidencia suficiente; el protocolo se activa ante un **patrón consistente** durante la Defensa Técnica, no ante un único momento de nerviosismo o duda puntual (que es normal y esperable).

---

## 4. Procedimiento Escalonado

### Nivel 1 — Dentro de la misma Defensa Técnica
- El docente reformula la pregunta de forma más simple o concreta, dando una segunda oportunidad genuina de demostrar comprensión (esto es justicia evaluativa, no debilidad del protocolo).
- Si la discrepancia persiste en 2 o más preguntas distintas, se documenta como hallazgo de Nivel 1 (ver formato en sección 6), sin necesariamente escalar aún a un proceso formal.

### Nivel 2 — Evaluación de Dominio del Aprendizaje
- El hallazgo de Nivel 1 se traduce directamente en una calificación más baja en los criterios "Dominio conceptual" y "Capacidad de explicar decisiones de diseño" de la Rúbrica 3 (Defensa Técnica Individual) — **este es el mecanismo de aplicación ordinario y esperado** para la mayoría de los casos.
- No requiere reporte a coordinación académica; es parte normal de la evaluación del curso.

### Nivel 3 — Sospecha de Deshonestidad Académica
- Se activa **solo** si, además de la discrepancia de dominio, existe evidencia adicional de que el trabajo no es genuinamente del estudiante (p. ej. código idéntico a otro estudiante sin explicación de colaboración autorizada, incapacidad total de ejecutar o modificar el código propio, contradicciones graves entre lo declarado en la Política de IA del repositorio y lo observado).
- El docente **no determina sanciones por sí mismo**; documenta el hallazgo (formato sección 6) y lo eleva a coordinación académica conforme al reglamento institucional de integridad académica vigente.

---

## 5. Lo que este Protocolo NO Hace

- No prohíbe ni desincentiva el uso de herramientas de IA generativa (eso ya está resuelto en la Política de IA del Syllabus).
- No convierte automáticamente una discrepancia de dominio en un caso disciplinario — la gran mayoría de los casos se resuelven en Nivel 2, como parte normal de la calificación.
- No sustituye el reglamento disciplinario institucional; en caso de conflicto, dicho reglamento prevalece.

---

## 6. Formato de Registro de Discrepancia

| Campo | Detalle |
|---|---|
| Estudiante / Proyecto | |
| Fecha de Defensa Técnica | |
| Pregunta(s) que evidenciaron la discrepancia | |
| Nivel alcanzado (1 / 2 / 3) | |
| Evidencia adicional (si aplica, Nivel 3) | |
| Acción tomada | Ej. "Reformulación de pregunta", "Calificación ajustada en Rúbrica 3", "Elevado a coordinación académica" |
| Docente responsable | |

Este registro se conserva como parte del expediente de evaluación del curso (no del Portafolio del estudiante) y puede usarse como insumo del Banco Histórico de Preguntas de Defensa para identificar qué tipo de preguntas son más efectivas detectando comprensión genuina.

---

## 7. Comunicación a Estudiantes

Se recomienda explicar este protocolo en términos simples durante la Semana 1, junto con la Política de IA:

> "Puedes usar IA todo lo que quieras para construir tu proyecto. Lo único que te puede bajar la nota — y en casos extremos generar un proceso disciplinario — es entregar algo que no puedes explicar como si fuera tuyo. La Defensa Técnica no busca atraparte; busca confirmar que aprendiste."
