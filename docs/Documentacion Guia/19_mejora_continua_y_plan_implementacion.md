# ESTRATEGIA DE MEJORA CONTINUA Y PLAN DE IMPLEMENTACIÓN DOCENTE
## TIIT2007 – Sistemas Operativos | Universidad Invenio

---

# PARTE 1 — Estrategia de Mejora Continua

## 1.1 Modelo de Ciclo de Mejora

El curso adopta un ciclo PHVA (Planificar–Hacer–Verificar–Actuar) aplicado a cada cohorte:

```
PLANIFICAR → Syllabus, Cronograma, Especificaciones (Fases 1–3)
        ↓
HACER → Ejecución del curso (Planes de Clase, Laboratorios, Proyectos)
        ↓
VERIFICAR → Indicadores de calidad + Auditorías académicas (documento anterior)
        ↓
ACTUAR → Ajustes documentados antes de la siguiente cohorte
        ↓
(vuelve a PLANIFICAR)
```

## 1.2 Fuentes de Información para la Mejora

| Fuente | Qué revela |
|---|---|
| Indicadores de resultado por RAC | Qué unidades/competencias presentan menor desempeño |
| Tasa de entrega a tiempo por proyecto | Si la carga de trabajo está bien distribuida en el cronograma |
| Resultados de Defensa Técnica | Si hay uso superficial de IA generativa no detectado a tiempo |
| Encuesta de satisfacción de cierre | Percepción de pertinencia y claridad metodológica |
| Hallazgos de auditoría académica | Desviaciones de implementación entre docentes/secciones |
| Retroalimentación cualitativa de Invenio Fest | Pertinencia real de la perspectiva de SO en proyectos interdisciplinarios |

## 1.3 Matriz de Decisión de Mejora

| Hallazgo | Acción de mejora típica |
|---|---|
| RAC con desempeño consistentemente bajo (\<75%) en 2+ cohortes | Revisar plan de clase de la unidad asociada; reforzar laboratorio o guía xv6 correspondiente |
| Baja tasa de entrega a tiempo en un proyecto específico | Evaluar si el cronograma asigna tiempo insuficiente a esa unidad; considerar redistribuir semanas |
| Defensa Técnica revela uso superficial recurrente de IA | Reforzar comunicación de la Política de IA desde semana 1; aumentar peso relativo de preguntas de variación |
| Baja satisfacción en claridad metodológica | Revisar redacción de especificaciones de proyecto y plantilla IEEE; considerar sesión adicional de orientación en semana 2 |
| Retroalimentación de Invenio Fest indica desconexión con el Proyecto Semilla | Reforzar en semana 11 la preparación específica de la sección técnica de SO |

## 1.4 Periodicidad y Responsables

| Actividad | Periodicidad | Responsable |
|---|---|---|
| Recolección de indicadores | Al cierre de cada cohorte | Docente titular |
| Análisis consolidado entre cohortes | Anual (o según calendario académico institucional) | Coordinación académica |
| Revisión y actualización de documentos del curso | Antes del inicio de cada cohorte, si hay hallazgos pendientes | Docente titular + coordinación académica |
| Comunicación de cambios a docentes de nuevas secciones | Al inicio de cada trimestre con oferta del curso | Coordinación académica |

## 1.5 Registro Histórico de Mejoras (plantilla)

| Cohorte | Hallazgo principal | Acción tomada | Resultado en la siguiente cohorte |
|---|---|---|---|
| [Trimestre/Año] | | | |
| [Trimestre/Año] | | | |

Este registro debe mantenerse vivo y disponible para procesos de acreditación como evidencia de mejora continua institucionalizada (no anecdótica).

---

# PARTE 2 — Plan de Implementación Docente

## 2.1 Propósito
Guiar a un docente — titular o nuevo — en la adopción ordenada del paquete completo de documentación de TIIT2007, evitando implementaciones parciales o inconsistentes entre secciones.

## 2.2 Fases de Implementación

### Fase A — Preparación (2–3 semanas antes del inicio del trimestre)
- [ ] Leer íntegramente Syllabus, Manual del Profesor y Cronograma.
- [ ] Verificar disponibilidad de entorno técnico: xv6 + QEMU, acceso a Linux y Windows en laboratorio, Docker (Unidad VII).
- [ ] Revisar y, si es necesario, regenerar los datasets experimentales (Documento de Dataset Experimental) con semillas propias para la cohorte.
- [ ] Configurar el repositorio GitHub de referencia (plantilla) y, si aplica, el NotebookLM institucional del curso.
- [ ] Revisar el Registro Histórico de Mejoras de cohortes anteriores (si existe) para incorporar ajustes ya identificados.

### Fase B — Apertura del Curso (Semana 1–2)
- [ ] Entregar Syllabus y Manual del Estudiante en la primera sesión.
- [ ] Comunicar explícitamente los límites del curso (qué NO es) para evitar expectativas desalineadas.
- [ ] Comunicar la Política de Uso de IA y su vínculo con la Defensa Técnica desde el primer día.
- [ ] Verificar que todos los estudiantes tengan el entorno técnico funcional antes de cerrar la Semana 2.

### Fase C — Ejecución (Semanas 3–11)
- [ ] Seguir los Planes de Clase semana a semana, sin alterar la secuencia teoría→xv6→profesional→C++.
- [ ] Publicar cada especificación de proyecto junto con su rúbrica y dataset en la semana de apertura correspondiente (ver Cronograma).
- [ ] Aplicar evaluación formativa (bancos de preguntas/casos) como apertura de clase, no como carga adicional fuera de horario.
- [ ] Registrar incidencias relevantes (entornos fallidos, ajustes de fecha) para alimentar la Estrategia de Mejora Continua.

### Fase D — Cierre (Semana 12)
- [ ] Ejecutar Defensa Técnica Individual con al menos una pregunta de variación por estudiante.
- [ ] Aplicar Rúbrica 4 exclusivamente a la perspectiva de SO en Invenio Fest (no evaluar la interdisciplinariedad completa).
- [ ] Recolectar Portafolios Técnicos Finales y aplicar su rúbrica dedicada.
- [ ] Aplicar encuesta de satisfacción de cierre de curso.

### Fase E — Post-Cierre (1–2 semanas después de finalizado el curso)
- [ ] Calcular indicadores de calidad del curso (documento de Indicadores).
- [ ] Completar el Registro Histórico de Mejoras con hallazgos de la cohorte.
- [ ] Archivar evidencias de acreditación conforme al documento de Evidencias Requeridas.
- [ ] Entregar a coordinación académica un informe breve de cierre (1–2 páginas) con hallazgos y recomendaciones para la siguiente cohorte.

## 2.3 Tabla de Verificación Rápida para Docente Nuevo (primera vez impartiendo el curso)

| Pregunta de autoevaluación | Sí/No |
|---|---|
| ¿Entiendo por qué el curso usa xv6 solo como observación y no como entorno de desarrollo? | |
| ¿Tengo claro que Invenio Fest no reemplaza los 4 proyectos? | |
| ¿Sé dónde y cuándo introducir cada Guía de Análisis de xv6? | |
| ¿Tengo lista la Plantilla IEEE para entregar en la Semana 2? | |
| ¿Comprendo cómo aplicar la Rúbrica ISO/IEC 25010 más allá de una calificación numérica (exigiendo justificación)? | |
| ¿Sé qué tipo de pregunta de variación debo formular en la Defensa Técnica? | |
| ¿Tengo acceso a los indicadores de cohortes anteriores (si existen) para no repetir errores ya identificados? | |

## 2.4 Riesgos Comunes de Implementación y Mitigación

| Riesgo | Mitigación |
|---|---|
| El curso deriva hacia administración de sistemas por comodidad del docente | Revisar constantemente la sección "Filosofía del Curso" del Manual del Profesor antes de cada unidad |
| Falta de entorno técnico uniforme entre estudiantes | Priorizar máquina virtual institucional preconfigurada con xv6/QEMU/Docker |
| Sobrecarga en la semana de cierre de un proyecto | Verificar en Fase A que el cronograma no se haya comprimido por feriados u otras actividades institucionales |
| Defensa Técnica superficial por falta de tiempo | Planificar bloques de tiempo suficientes en Semana 12; considerar evaluación distribuida en más de un día si la cantidad de estudiantes lo requiere |

---

Con este documento se cierra el paquete de 30 entregables solicitados para TIIT2007 – Sistemas Operativos, cubriendo planificación curricular, ejecución semanal, proyectos y evaluación, manuales, e infraestructura de calidad/acreditación institucional.
