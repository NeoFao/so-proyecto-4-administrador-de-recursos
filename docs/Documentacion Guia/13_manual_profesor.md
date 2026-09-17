# MANUAL DEL PROFESOR
## TIIT2007 – Sistemas Operativos | Universidad Invenio

---

## 1. Propósito de este Manual

Este manual orienta al docente (titular o nuevo) en la implementación fiel del curso TIIT2007, garantizando consistencia entre secciones y cohortes, y preservando la filosofía pedagógica del curso: comprensión y modelado de mecanismos de SO, **no** administración de sistemas ni ingeniería de kernels.

---

## 2. Filosofía del Curso — Lo que el Docente Debe Resguardar

- El curso **no** es Administración Linux, DevOps, Cloud, Ciberseguridad ni programación de kernels.
- xv6 se usa **solo para observación**. Si un estudiante propone modificar o compilar xv6 con cambios, el docente debe redirigir la actividad a su propósito original.
- Invenio Fest **no** sustituye los 4 proyectos del curso; su evaluación se limita a la perspectiva de SO.
- El curso prepara para la práctica dual, pero no es en sí una experiencia dual.

---

## 3. Secuencia Pedagógica Obligatoria por Unidad

Cada unidad debe recorrer, en este orden: **Teoría → xv6 (observación) → Linux/Windows (aplicación profesional) → Simulación en C++ → Documentación IEEE/ISO → (integración en Invenio Fest, semana 12)**. Saltarse esta secuencia (p. ej. ir directo a codificar sin teoría) debilita la trazabilidad de competencias del curso.

---

## 4. Uso de los Documentos del Curso

| Documento | Cuándo usarlo |
|---|---|
| Syllabus | Primera sesión; entrega oficial al estudiante |
| Cronograma | Planificación semanal; referencia constante |
| Planes de clase | Guía minuto a minuto de cada sesión |
| Guías de laboratorio | Bloques "Profesional" de cada plan de clase |
| Guías de análisis xv6 | Bloques "xv6" de cada plan de clase; tarea dirigida post-sesión |
| Especificaciones de proyectos | Entregar íntegras al abrir cada proyecto |
| Plantilla IEEE | Entregar en la primera apertura de proyecto (Semana 2) |
| Rúbricas | Publicar junto con cada especificación de proyecto, antes de iniciar el trabajo |
| Banco de preguntas/casos/ejercicios | Quizzes formativos, activaciones de clase, apoyo a Defensa Técnica |
| Dataset experimental | Publicar junto con cada especificación de proyecto |

---

## 5. Calendario de Hitos de Evaluación

| Semana | Hito |
|---|---|
| 4 | Cierre Proyecto 1 (18%) |
| 6 | Cierre Proyecto 2 (18%) |
| 8 | Cierre Proyecto 3 (18%) |
| 11 | Cierre Proyecto 4 (18%) |
| 12 | Invenio Fest (10%), Defensa Técnica (8%), Portafolio (10%) |

**Recomendación:** abrir el espacio de retroalimentación de cada proyecto dentro de los 5 días hábiles posteriores a su cierre, para que el estudiante pueda incorporar aprendizajes en el siguiente proyecto.

---

## 6. Gestión de la Política de IA en el Aula

- Permitir el uso de IA generativa abiertamente; no es necesario perseguir su uso.
- Durante la Defensa Técnica, plantear siempre al menos una pregunta de variación ("¿qué pasaría si...?") no contemplada explícitamente en el proyecto entregado, como mecanismo de verificación de comprensión genuina.
- Si un estudiante no puede explicar su propio código, esto se registra como evidencia de falta de dominio (no como falta disciplinaria, salvo que se determine deshonestidad académica conforme al reglamento institucional vigente, que rige sobre cualquier disposición de este curso).

---

## 7. Gestión de Invenio Fest desde el Curso

- Recordar a los estudiantes, desde la Semana 2, que el Proyecto Semilla institucional eventualmente deberá analizarse desde la óptica de SO (procesos, memoria, recursos, almacenamiento, seguridad, virtualización).
- En la Semana 11, dedicar tiempo explícito a la preparación de la sección técnica de SO para el póster/presentación.
- El docente del curso evalúa **únicamente** la perspectiva de SO en Invenio Fest, usando la Rúbrica 4 (Participación Técnica en Invenio Fest); no evalúa la interdisciplinariedad completa del Proyecto Semilla.

---

## 8. Manejo de Casos Especiales

| Situación | Recomendación |
|---|---|
| Estudiante sin prerrequisitos firmes en C++ | Reforzar con ejercicios adicionales de las primeras semanas; no reducir el alcance de los proyectos |
| Estudiante que solo usó IA sin comprender el resultado | Detectarlo en Defensa Técnica; usar como oportunidad formativa antes de la nota final si el calendario lo permite |
| Entorno de laboratorio no disponible para todos (Docker, QEMU/xv6) | Priorizar máquinas virtuales institucionales compartidas o sesiones de laboratorio presenciales supervisadas |
| Estudiante de práctica dual avanzado que ya conoce administración de sistemas | Reencauzar su profundización hacia el modelado/simulación (núcleo del curso), no hacia administración avanzada fuera de alcance |

---

## 9. Errores Comunes a Evitar (Lecciones de Implementación)

- Convertir el curso en un curso de administración Linux/Windows por exceso de tiempo en laboratorios "profesionales" en detrimento de la teoría y la simulación.
- Permitir que los estudiantes modifiquen o intenten compilar xv6 con cambios — esto desvía el propósito pedagógico y consume tiempo no planificado.
- Evaluar Invenio Fest con criterios ajenos a Sistemas Operativos (eso corresponde a otras asignaturas del proyecto interdisciplinario).
- Aceptar documentos IEEE incompletos "para completarse después" — esto rompe la trazabilidad de evaluación.

---

## 10. Recursos de Apoyo Docente

- Plantillas editables de diapositivas por unidad (a desarrollar localmente por el docente, alineadas a los Planes de Clase).
- Entorno de referencia: distribución Linux con QEMU + xv6 preinstalado (recomendado para evitar pérdida de tiempo en instalación individual).
- Canal de consulta institucional para dudas técnicas del docente nuevo (a definir por la coordinación académica).

Este manual se complementa con el **Plan de Implementación Docente** (documento dedicado), que detalla el proceso de adopción del curso para un docente que lo imparte por primera vez.
