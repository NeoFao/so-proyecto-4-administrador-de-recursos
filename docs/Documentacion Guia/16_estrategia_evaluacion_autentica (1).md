# ESTRATEGIA DE EVALUACIÓN AUTÉNTICA
## TIIT2007 – Sistemas Operativos | Universidad Invenio

---

## 1. Fundamento

La evaluación auténtica mide el desempeño del estudiante en condiciones que replican la práctica profesional real, en lugar de limitarse a la recuperación de información. TIIT2007 implementa este enfoque mediante tres pilares: **(1)** producción de artefactos técnicos reales (simuladores funcionales), **(2)** documentación bajo normas profesionales vigentes (IEEE, ISO/IEC 25010), y **(3)** comunicación y defensa técnica ante audiencias evaluadoras (docente e Invenio Fest).

---

## 2. Principios que Rigen la Evaluación del Curso

1. **Nada se evalúa solo por su resultado final** — todo proyecto requiere evidencia experimental reproducible, no solo "que funcione".
2. **Toda solución debe ser explicable por su autor** — la Defensa Técnica Individual verifica comprensión genuina, independientemente de las herramientas (incluida IA) usadas en su construcción.
3. **La calidad se documenta, no se asume** — cada proyecto incluye autoevaluación ISO/IEC 25010 justificada, no solo calificada.
4. **El aprendizaje se demuestra en contexto real** — los laboratorios comparativos Linux/Windows y la observación de xv6 anclan la teoría en sistemas reales, no solo en simulación abstracta.
5. **La integración interdisciplinaria es evidencia de transferencia** — Invenio Fest evalúa si el estudiante puede aplicar lo aprendido fuera del contexto controlado del curso.

---

## 3. Matriz de Autenticidad por Instrumento

| Instrumento | Condición auténtica que replica | Qué NO mide |
|---|---|---|
| Proyectos 1–4 | Desarrollo de software con datos experimentales reales, como en un entorno profesional de ingeniería | Memorización de definiciones |
| Documento IEEE | Comunicación técnica escrita bajo norma profesional real | Capacidad de copiar una plantilla sin comprensión |
| Rúbrica ISO/IEC 25010 | Evaluación de calidad de software como ocurre en la industria | Opinión subjetiva sin evidencia |
| Defensa Técnica Individual | Sustentación técnica ante un evaluador, como en una revisión de código o entrevista técnica | Habilidad de leer notas preparadas |
| Invenio Fest | Comunicación a una audiencia no especializada y transferencia a un contexto interdisciplinario real | Dominio aislado de un solo mecanismo de SO |
| Portafolio Técnico Final | Capacidad de síntesis y reflexión profesional, como en un dossier de carrera | Cantidad de páginas o volumen de evidencia bruta |

---

## 4. Estrategias Específicas de Implementación

### 4.1 Datos experimentales verificables
Todo proyecto debe poder re-ejecutarse en vivo. El docente puede solicitar, durante la Defensa Técnica, que el estudiante ejecute su simulador con un dataset distinto al usado en el documento entregado, para verificar la autenticidad de los resultados.

### 4.2 Preguntas de variación (no anticipadas)
En cada Defensa Técnica, el docente plantea al menos una pregunta de tipo "¿qué pasaría si...?" que no fue explícitamente respondida en el documento IEEE del estudiante (ver Banco de Casos Empresariales y Banco de 100 Preguntas Conceptuales como fuente).

### 4.3 Observación de xv6 como ancla de realismo
Las Guías de Análisis de xv6 evitan que el curso se vuelva puramente teórico-abstracto: el estudiante confronta código real de un SO funcional antes de construir su propio modelo simplificado.

### 4.4 Transferencia interdisciplinaria (Invenio Fest)
Se evalúa explícitamente la capacidad de aplicar conceptos de SO a un contexto que el estudiante no controla por completo (el Proyecto Semilla de otro equipo/disciplina), lo cual mide transferencia real del aprendizaje y no solo ejecución de instrucciones del curso.

### 4.5 Reflexión metacognitiva obligatoria
El Portafolio Técnico Final exige reflexión profesional explícita (no opcional), forzando al estudiante a articular su propio proceso de aprendizaje — un componente reconocido de la evaluación auténtica que distingue comprensión profunda de ejecución mecánica.

---

## 5. Calendario de Evidencia Auténtica vs. Calendario de Notas

| Semana | Evidencia auténtica generada | ¿Tiene nota directa? |
|---|---|---|
| 1–2 | Bitácora de entorno, primera guía xv6 | No (formativo) |
| 4, 6, 8, 11 | Proyectos + IEEE + ISO 25010 | Sí (18% c/u) |
| 12 | Defensa Técnica, Invenio Fest, Portafolio | Sí (8% + 10% + 10%) |

Esta separación entre evidencia formativa (sin nota directa pero con valor pedagógico) y evidencia sumativa (con nota) es intencional: protege el espacio de aprendizaje exploratorio (lectura de xv6, laboratorios) de la presión evaluativa directa, mientras concentra la rendición de cuentas en los artefactos de mayor autenticidad profesional (proyectos, defensa, portafolio).

---

## 6. Validez y Confiabilidad del Modelo de Evaluación

- **Validez de contenido:** cada instrumento se deriva directamente de la Matriz Competencia → Resultado → Evidencia → Evaluación (Fase 1), garantizando que se evalúa lo que se declaró enseñar.
- **Validez de constructo:** la Defensa Técnica Individual existe específicamente para separar "comprensión genuina" de "entrega funcional", abordando el riesgo de uso superficial de IA generativa.
- **Confiabilidad entre evaluadores:** las rúbricas analíticas (Fase 3) reducen subjetividad al descomponer cada nota en criterios observables y graduados.
