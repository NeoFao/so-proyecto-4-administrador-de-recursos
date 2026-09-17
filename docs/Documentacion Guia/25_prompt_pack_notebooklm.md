# PROMPT PACK PARA NOTEBOOKLM INSTITUCIONAL
## TIIT2007 – Sistemas Operativos | Universidad Invenio

Prompts listos para usar dentro de NotebookLM, una vez cargadas las fuentes indicadas en `docs/17_notebooklm_y_repositorio.md`. Organizados por uso. Copiar y pegar directamente; los textos entre `[corchetes]` deben personalizarse.

---

## 1. Prompts de Configuración Inicial (al crear el notebook)

**Nombre sugerido del notebook:** `TIIT2007 – Sistemas Operativos – Universidad Invenio`

**Prompt de orientación inicial (pegar como primera nota del notebook, no como pregunta):**
```
Este notebook contiene exclusivamente las fuentes oficiales del curso TIIT2007 –
Sistemas Operativos de la Licenciatura en TICE de Universidad Invenio. El curso NO
es administración de sistemas, DevOps, Cloud Computing, ciberseguridad ni
programación de kernels — es un curso de comprensión, análisis y modelado de
mecanismos fundamentales de sistemas operativos modernos. Toda respuesta debe
basarse únicamente en las fuentes cargadas y respetar este alcance. Si una
pregunta se sale del alcance declarado (p. ej. administración avanzada de
servidores), señalarlo explícitamente en lugar de responder como si fuera
contenido del curso.
```

---

## 2. Prompts para el Estudiante (estudio guiado)

**Repaso conceptual por unidad:**
```
Actúa como tutor del curso TIIT2007. Hazme 5 preguntas de repaso sobre la
Unidad [número/nombre], basadas únicamente en las fuentes del curso. Después
de que yo responda cada una, dame retroalimentación indicando si mi respuesta
fue correcta, incompleta, o si tengo un concepto equivocado — y por qué.
```

**Conexión teoría–xv6–profesional:**
```
Explícame cómo se conecta el concepto de [concepto, ej. "exclusión mutua"]
en estas tres capas: (1) la teoría vista en clase, (2) su implementación
observable en xv6, y (3) su aplicación profesional en Linux o Windows.
Usa solo lo que está en las fuentes del curso.
```

**Preparación para Defensa Técnica (autoestudio, NO sustituye preparar el propio código):**
```
Voy a presentar mi Defensa Técnica del Proyecto [1/2/3/4]. Basado en las
preguntas de variación típicas del curso, hazme 3 preguntas de tipo
"¿qué pasaría si...?" sobre mi proyecto, sin decirme la respuesta primero.
Espera mi respuesta antes de darme retroalimentación.
```

**Generación de resumen de audio (usar función nativa de NotebookLM, con este enfoque):**
```
Genera un resumen de audio de la Unidad [número] enfocado en: (1) los
conceptos teóricos clave, (2) qué se observa en xv6 para esta unidad,
y (3) cómo se relaciona con el proyecto correspondiente del curso.
```

---

## 3. Prompts para el Docente (preparación de clase)

**Generación de banco de preguntas adicional para quiz formativo:**
```
A partir de las fuentes del curso, genera 10 preguntas conceptuales nuevas
sobre la Unidad [número], distintas a las ya existentes en el Banco de 100
Preguntas Conceptuales. Indica para cada una si es de nivel básico,
intermedio o de análisis (★).
```

**Verificación de alineación de una actividad nueva:**
```
Voy a proponer la siguiente actividad para la Unidad [número]: [describir
actividad]. Basado en el Syllabus y el Mapa de Alineación Curricular del
curso, ¿esta actividad es coherente con el alcance declarado del curso?
¿Con qué Resultado de Aprendizaje (RAC) se conecta más directamente?
```

**Apoyo para retroalimentación de Documento IEEE:**
```
Aquí está la sección de Marco Teórico de un documento IEEE de un estudiante
sobre [proyecto]: [pegar texto]. Basado en la Plantilla IEEE institucional
y el Marco Teórico esperado para este proyecto, ¿qué le falta o qué
podría mejorar el estudiante? No reescribas el texto por el estudiante,
solo da retroalimentación.
```

---

## 4. Prompts para Coordinación Académica (uso administrativo)

**Verificación de trazabilidad documental:**
```
Revisa la Matriz Competencia→Resultado→Evidencia→Evaluación y el Mapa
ABET/CDIO cargados en este notebook. Identifica si existe algún Resultado
de Aprendizaje (RAC) que no tenga un instrumento de evaluación claramente
asociado, según las fuentes disponibles.
```

**Resumen ejecutivo para informe de acreditación:**
```
Genera un resumen ejecutivo de 1 página, en tono institucional formal,
que describa: (1) el propósito del curso TIIT2007, (2) su metodología
pedagógica de 4 perspectivas (teoría-xv6-profesional-simulación), y
(3) cómo se garantiza la trazabilidad de competencias hacia ABET/CDIO,
basado únicamente en las fuentes cargadas.
```

---

## 5. Prompts de Mantenimiento del Notebook (uso interno, no para estudiantes)

**Auditoría de desactualización:**
```
Compara el contenido de la fuente "Cronograma" con la fuente "Planes de
Clase". Señala cualquier inconsistencia de semanas, fechas, o entregables
que detectes entre ambos documentos.
```

**Verificación de alcance ("guardrail check"):**
```
Revisa las últimas fuentes que cargué. ¿Alguna de ellas introduce contenido
que contradiga el alcance declarado del curso (es decir, que se acerque a
administración de sistemas, DevOps, ciberseguridad ofensiva, o programación
de kernels más allá de observación)? Señálalo si lo encuentras.
```

---

## 6. Buenas Prácticas de Uso (comunicar a estudiantes y docentes)

- El notebook responde **solo** con base en las fuentes cargadas — esto es una ventaja pedagógica (evita respuestas genéricas de internet) pero significa que preguntas fuera del alcance del curso no tendrán buena respuesta, y eso es intencional.
- Ningún prompt de este pack sustituye la lectura de las fuentes primarias (Silberschatz, Tanenbaum, OSTEP) ni la elaboración propia del código y los documentos IEEE.
- Recordar siempre la Política de IA del curso: todo lo producido con apoyo de NotebookLM debe poder explicarse sin él, especialmente de cara a la Defensa Técnica Individual.
