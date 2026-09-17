# MAPA DE ALINEACIÓN CURRICULAR Y MAPA ABET/CDIO
## TIIT2007 – Sistemas Operativos | Universidad Invenio

---

## PARTE 1 — Mapa de Alineación Curricular

### 1.1 Posicionamiento del curso en el plan de estudios

| Trimestre | Año 1 | Año 2+ |
|---|---|---|
| T1 | Materias | Materias |
| T2 | Materias | **Dual** |
| T3 | Materias | Materias |
| T4 | **Dual** | Dual |

TIIT2007 se ubica en un trimestre de **Materias**, nunca en un trimestre Dual. Su función curricular es preparar al estudiante — conceptual y técnicamente — para los trimestres duales subsiguientes, sin constituirse en una experiencia dual en sí misma.

### 1.2 Prerrequisitos y su aporte directo

| Curso previo | Aporte que habilita TIIT2007 |
|---|---|
| Programación | Dominio de C++ necesario para los 4 simuladores |
| Bases de Datos | Comprensión de gestión de datos persistentes, útil en Unidad V (sistemas de archivos) |
| Redes | Base conceptual para IPC, virtualización y sistemas distribuidos (Unidades II y VII) |
| Arquitectura computacional (básica) | Sustento para Unidad I y Unidad VI (interrupciones, DMA, drivers) |

### 1.3 Cursos posteriores que dependen de TIIT2007

- Cursos de Práctica Dual (Trimestres duales de Año 1 T4 y Año 2+ T2/T4): el estudiante debe poder razonar sobre procesos, memoria y recursos en entornos empresariales reales.
- Cualquier curso posterior de infraestructura, cloud, ciberseguridad o ingeniería de software que asuma comprensión de mecanismos internos de un SO.

### 1.4 Mapa Unidad → Resultado de Aprendizaje → Perspectiva pedagógica

| Unidad | RAC principal | Teoría | xv6 | Linux/Windows | C++ |
|---|---|---|---|---|---|
| I. Arquitectura | RAC1 | ✔ | ✔ (boot, estructura) | ✔ (inspección) | — |
| II. Procesos y Scheduling | RAC2 | ✔ | ✔ (`proc.c`, `scheduler()`) | ✔ (`ps`, `top`, `Get-Process`) | ✔ Proyecto 1 |
| III. Concurrencia | RAC3 | ✔ | ✔ (`spinlock`) | ✔ (observación de hilos) | ✔ Proyecto 2 |
| IV. Memoria | RAC4 | ✔ | ✔ (`vm.c`) | ✔ (`free`, `vmstat`) | ✔ Proyecto 3 |
| V. Archivos y Seguridad | RAC5 | ✔ | ✔ (`fs.c`) | ✔ (`chmod`, ACL) | ✔ Proyecto 4 (inicio) |
| VI. E/S y Recursos | RAC6 | ✔ | — | ✔ (monitoreo) | ✔ Proyecto 4 (cont.) |
| VII. Virtualización y Sistemas Modernos | RAC7 | ✔ | — | ✔ (contenedores, observación) | — |

### 1.5 Alineación con el Modelo Educativo por Competencias de Invenio

- **Aprendizaje activo:** proyectos progresivos de complejidad creciente (Proyectos 1–4).
- **Evaluación auténtica:** documentos IEEE y evaluación ISO/IEC 25010 replican condiciones de ejercicio profesional real.
- **Integración interdisciplinaria:** Invenio Fest conecta el curso con el Proyecto Semilla institucional.
- **Trazabilidad:** cada resultado de aprendizaje cuenta con evidencia y evaluación específicas (ver Matriz Competencia → Resultado → Evidencia → Evaluación).

---

## PARTE 2 — Mapa ABET / CDIO

### 2.1 Alineación con criterios ABET (Student Outcomes, Criterion 3)

| Resultado ABET (genérico, programa de Computing/IT) | RAC del curso que lo evidencia | Evidencia concreta |
|---|---|---|
| **(1)** Analizar problemas complejos de computación aplicando principios de computación e ingeniería | RAC2, RAC3, RAC4 | Proyectos 1–3: análisis y modelado de algoritmos de SO |
| **(2)** Diseñar, implementar y evaluar soluciones basadas en cómputo que satisfagan requisitos | RAC2–RAC6 | Diseño e implementación de los 4 simuladores; evaluación ISO/IEC 25010 |
| **(3)** Comunicarse efectivamente en diversos contextos | RAC9 | Documentos IEEE; Defensa Técnica Individual; Invenio Fest |
| **(4)** Reconocer responsabilidades profesionales y éticas, y tomar decisiones informadas considerando el impacto | RAC5, RAC6, política de uso de IA | Unidad V (seguridad, permisos); política de IA con exigencia de comprensión y defensa propia |
| **(5)** Funcionar efectivamente en equipos que establecen metas, planifican tareas y cumplen objetivos | RAC9 | Invenio Fest (proyecto interdisciplinario) |
| **(6)** Aplicar técnicas, habilidades y herramientas modernas de cómputo necesarias para la práctica | RAC2–RAC7 | xv6, Bash, PowerShell, simuladores C++, contenedores (observación) |

### 2.2 Alineación con los Estándares CDIO (Conceive – Design – Implement – Operate)

| Fase CDIO | Actividad del curso | Unidad/Proyecto |
|---|---|---|
| **Conceive** (Concebir) | Comprensión teórica del problema (p. ej. ¿por qué planificar procesos? ¿por qué sincronizar?) | Fundamento teórico de cada unidad |
| **Design** (Diseñar) | Diseño del algoritmo/simulador antes de codificar; diseño documentado en sección "Diseño de la Solución" del IEEE | Proyectos 1–4 |
| **Implement** (Implementar) | Codificación en C++ de los simuladores | Proyectos 1–4 |
| **Operate** (Operar) | Ejecución, recolección de resultados experimentales, monitoreo en Linux/Windows, evaluación ISO/IEC 25010 | Proyectos 1–4; Unidad VI |

### 2.3 Estándares CDIO específicos cubiertos

| Estándar CDIO | Cobertura en el curso |
|---|---|
| CDIO Standard 2 (Learning Outcomes) | RAC1–RAC9 explícitos y medibles |
| CDIO Standard 3 (Integrated Curriculum) | Integración teoría–xv6–profesional–simulación en cada unidad |
| CDIO Standard 5 (Design-Implement Experiences) | Los 4 proyectos cubren ciclo completo diseño-implementación |
| CDIO Standard 7 (Integrated Learning Experiences) | Vinculación con Invenio Fest como experiencia interdisciplinaria |
| CDIO Standard 11 (Learning Assessment) | Rúbricas analíticas, ISO/IEC 25010, Defensa Técnica, Portafolio |

---

## PARTE 3 — Síntesis de Trazabilidad Global

```
Perfil de Egreso (TICE)
        ↓
Competencias del curso (8) — ver Matriz Competencia→Resultado→Evidencia→Evaluación
        ↓
Resultados de Aprendizaje (RAC1–RAC9) — ver Syllabus
        ↓
Unidades Temáticas (I–VII) — ver Cronograma
        ↓
Proyectos 1–4 + Invenio Fest — evidencia concreta
        ↓
Instrumentos de evaluación (rúbricas, IEEE, ISO/IEC 25010) — Fase 3
        ↓
Acreditación institucional (ABET/CDIO) — este documento
```

Esta cadena de trazabilidad será reutilizada íntegramente en el documento de **Evidencias requeridas para acreditación** e **Indicadores de calidad del curso** (Fase 4).
