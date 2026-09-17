# RÚBRICAS DE EVALUACIÓN
## TIIT2007 – Sistemas Operativos | Universidad Invenio

Escala de desempeño usada en todas las rúbricas analíticas: **Excelente (4) — Competente (3) — En desarrollo (2) — Insuficiente (1) — No presentado (0)**.

---

## RÚBRICA 1 — Rúbrica Analítica General de Proyecto (aplica a Proyectos 1–4)

Esta rúbrica determina el 18% asignado a cada proyecto. Pondera el código/implementación, los resultados experimentales y el documento IEEE de forma integrada.

| Criterio | Peso dentro del proyecto | Excelente (4) | Competente (3) | En desarrollo (2) | Insuficiente (1) |
|---|---|---|---|---|---|
| **Corrección funcional** | 30% | Implementa todos los requisitos funcionales correctamente, sin errores en casos de prueba | Implementa todos los requisitos con errores menores no críticos | Implementa la mayoría de los requisitos, con al menos un error significativo | Implementación incompleta o con errores que invalidan los resultados |
| **Calidad del diseño/código** | 15% | Código modular, legible, con estructuras de datos justificadas y manejo de errores robusto | Código organizado con manejo de errores básico | Código funcional pero con baja modularidad o manejo de errores deficiente | Código desorganizado, difícil de seguir o sin manejo de errores |
| **Resultados experimentales** | 20% | Todos los resultados obligatorios presentes, con datos reproducibles y bien presentados (tablas/gráficos claros) | Resultados obligatorios presentes con presentación mejorable | Resultados parciales o con inconsistencias menores | Resultados ausentes, inventados o no reproducibles |
| **Documento IEEE** | 25% | Las 10 secciones completas, bien redactadas, con análisis crítico genuino | 10 secciones presentes con análisis superficial en alguna sección | Documento incompleto (faltan secciones) o con análisis mínimo | Documento ausente o gravemente incompleto |
| **Evaluación ISO/IEC 25010** | 10% | Autoevaluación justificada y coherente con la evidencia del proyecto | Autoevaluación presente pero con justificación débil | Autoevaluación superficial (solo calificaciones, sin justificar) | Ausente |

**Cálculo:** Nota del proyecto = Σ (puntaje del criterio × peso). Resultado se escala al 18% correspondiente del curso.

---

## RÚBRICA 2 — Rúbrica ISO/IEC 25010

Aplica a la autoevaluación de cada proyecto (sección 7 del documento IEEE) y es verificada/contrastada por el docente.

| Característica de calidad (ISO/IEC 25010) | Pregunta guía para el estudiante | Excelente (4) | Competente (3) | En desarrollo (2) | Insuficiente (1) |
|---|---|---|---|---|---|
| **Rendimiento (Performance Efficiency)** | ¿Qué tan eficiente es la solución en tiempo y uso de recursos? | Mide y reporta tiempos/recursos con datos concretos y los compara entre escenarios | Mide tiempos/recursos pero sin comparación profunda | Menciona rendimiento sin datos concretos | No aborda rendimiento |
| **Fiabilidad (Reliability)** | ¿La solución produce resultados consistentes y maneja errores? | Maneja errores explícitamente y produce resultados consistentes en ejecuciones repetidas | Maneja errores básicos, resultados mayormente consistentes | Manejo de errores mínimo | Sin manejo de errores; resultados inconsistentes |
| **Usabilidad (Usability)** | ¿La interfaz de consola es comprensible para un usuario nuevo? | Interfaz clara, mensajes de ayuda/error comprensibles | Interfaz funcional con mensajes básicos | Interfaz confusa pero usable con instrucciones externas | Interfaz inutilizable sin leer el código fuente |
| **Compatibilidad (Compatibility)** | ¿La solución es portable o declara explícitamente sus dependencias de plataforma? | Compila/corre en múltiples entornos o declara explícitamente limitaciones de plataforma con justificación | Funciona en al menos un entorno, documentado | Funciona solo en el entorno del desarrollador, sin documentar por qué | No compila de forma reproducible |
| **Mantenibilidad (Maintainability)** | ¿El código podría ser modificado por otro desarrollador sin dificultad? | Código modular, comentado, con nombres descriptivos | Código razonablemente organizado | Código funcional pero difícil de modificar | Código no mantenible |

**Nota:** "Eficiencia" se entiende integrada dentro de Rendimiento en este modelo simplificado para fines de curso; el estudiante puede profundizar la distinción ISO/IEC 25010 completa (eficiencia de recursos vs. comportamiento temporal) como ejercicio de profundización opcional.

---

## RÚBRICA 3 — Defensa Técnica Individual (8% de la nota final)

Evaluación oral individual donde el estudiante explica y defiende su trabajo del curso (proyectos y conceptos teóricos), sin apoyo de notas externas más allá de su propio código/documentación.

| Criterio | Peso | Excelente (4) | Competente (3) | En desarrollo (2) | Insuficiente (1) |
|---|---|---|---|---|---|
| **Dominio conceptual** | 35% | Explica con precisión los conceptos teóricos detrás de su(s) proyecto(s) sin apoyo | Explica correctamente con apoyo ocasional de su documentación | Explica con errores conceptuales menores | No puede explicar los conceptos base de su propio proyecto |
| **Capacidad de explicar decisiones de diseño** | 30% | Justifica cada decisión de diseño (estructuras de datos, algoritmos) de forma coherente | Justifica la mayoría de las decisiones | Justifica parcialmente, con vacíos notorios | No puede justificar sus propias decisiones |
| **Respuesta a preguntas no anticipadas** | 25% | Responde con seguridad a variaciones/hipotéticos planteados por el evaluador (p. ej. "¿qué pasaría si...?") | Responde con apoyo de razonamiento parcial | Responde con dificultad notable | No puede responder a variaciones del problema |
| **Comunicación técnica** | 10% | Lenguaje técnico preciso, estructura clara | Comunicación clara con imprecisiones menores | Comunicación confusa en partes | Comunicación inadecuada para el nivel del curso |

**Nota importante (Política de IA):** Si el estudiante no puede explicar una solución que entregó como propia, esto se considera evidencia de falta de dominio del aprendizaje y se refleja directamente en el criterio "Dominio conceptual" y "Capacidad de explicar decisiones de diseño".

---

## RÚBRICA 4 — Participación Técnica en Invenio Fest (10% de la nota final)

Evalúa exclusivamente la perspectiva de Sistemas Operativos aplicada al Proyecto Semilla institucional (no evalúa el proyecto interdisciplinario en su totalidad).

| Criterio | Peso | Excelente (4) | Competente (3) | En desarrollo (2) | Insuficiente (1) |
|---|---|---|---|---|---|
| **Análisis técnico de SO aplicado al Proyecto Semilla** | 35% | Identifica y analiza con profundidad al menos 3 de: procesos, memoria, recursos, almacenamiento, seguridad, virtualización, aplicados al proyecto | Identifica y analiza correctamente 2 de los elementos | Identifica al menos 1 elemento con análisis superficial | No conecta el proyecto con conceptos de SO |
| **Calidad de la sección técnica (póster/documento)** | 25% | Sección técnica clara, bien estructurada, con terminología precisa de SO | Sección técnica adecuada con imprecisiones menores | Sección técnica incompleta o genérica | Sección técnica ausente o irrelevante al curso |
| **Calidad de la presentación oral** | 25% | Presenta con claridad, responde preguntas del público/jurado con dominio | Presenta correctamente con respuestas básicas a preguntas | Presentación débil o respuestas evasivas | No presenta o presentación inadecuada |
| **Evidencias asociadas** | 15% | Evidencias completas y bien organizadas (capturas, datos, código relacionado) | Evidencias presentes con organización mejorable | Evidencias parciales | Evidencias ausentes |

---

## Tabla Resumen de Instrumentos por Componente de Nota

| Componente de nota | Peso | Instrumento(s) aplicado(s) |
|---|---|---|
| Proyecto 1 + IEEE | 18% | Rúbrica 1 + Rúbrica 2 |
| Proyecto 2 + IEEE | 18% | Rúbrica 1 + Rúbrica 2 |
| Proyecto 3 + IEEE | 18% | Rúbrica 1 + Rúbrica 2 |
| Proyecto 4 + IEEE | 18% | Rúbrica 1 + Rúbrica 2 |
| Participación Invenio Fest | 10% | Rúbrica 4 |
| Defensa Técnica Individual | 8% | Rúbrica 3 |
| Portafolio Técnico Final | 10% | Rúbrica dedicada (ver Fase 4) |
