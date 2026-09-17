# SYLLABUS INSTITUCIONAL
## TIIT2007 – Sistemas Operativos
### Universidad Invenio | Licenciatura en Tecnologías de Información y Comunicación Empresarial

---

## 1. Información General del Curso

| Campo | Detalle |
|---|---|
| Código | TIIT2007 |
| Nombre | Sistemas Operativos |
| Programa | Licenciatura en Tecnologías de Información y Comunicación Empresarial |
| Duración | 12 semanas (11 semanas efectivas + Semana 12: Invenio Fest) |
| Horas semanales | 4 |
| Horas totales | 44 |
| Modalidad | Mixta (Presencial + Virtual Sincrónica) |
| Modelo educativo | Basado en competencias |
| Posición curricular | Curso preparatorio para experiencias duales (NO es práctica dual) |
| Prerrequisitos | Programación, Bases de Datos, Redes |
| Lenguaje base de proyectos | C++ |

---

## 2. Descripción del Curso

Sistemas Operativos es un curso de profundización conceptual y aplicada que estudia los mecanismos fundamentales mediante los cuales un sistema operativo gestiona procesos, memoria, archivos, dispositivos y recursos. El curso **no** es un curso de administración de sistemas, DevOps, Cloud Computing, ingeniería de kernels o ciberseguridad. Es un curso de **comprensión, análisis y modelado** de los mecanismos internos que sostienen el cómputo moderno.

El curso integra cuatro perspectivas complementarias y secuenciales para cada unidad temática:

1. **Fundamento Teórico** — Silberschatz, Tanenbaum, OSTEP.
2. **Implementación Real Simplificada** — observación (no modificación) del código fuente de xv6.
3. **Aplicación Profesional** — laboratorios comparativos en Linux (Bash) y Windows (PowerShell).
4. **Modelado Computacional** — simuladores propios desarrollados en C++.

Esta secuencia pedagógica (Teoría → xv6 → Linux/Windows → Simulación C++ → Documentación IEEE/ISO → Comunicación en Invenio Fest) constituye el eje articulador de todo el curso.

---

## 3. Justificación Curricular

El estudiante de TICE requiere comprender el comportamiento interno de los sistemas que administra, integra y consume profesionalmente, sin necesidad de convertirse en un ingeniero de sistemas operativos. Este curso cierra la brecha entre el conocimiento de programación/redes/bases de datos ya adquirido y la capacidad de razonar sobre el comportamiento de un sistema en ejecución — habilidad indispensable antes de que el estudiante enfrente sus trimestres de práctica dual.

---

## 4. Resultados de Aprendizaje del Curso (RAC)

Al finalizar el curso, el estudiante será capaz de:

- **RAC1.** Explicar la arquitectura, evolución y funciones fundamentales de un sistema operativo moderno, incluyendo modelos monolíticos, microkernel e híbridos.
- **RAC2.** Modelar y analizar algoritmos de planificación de procesos (FCFS, SJF, Round Robin, prioridades) mediante simulación en C++ y comparación con su implementación en xv6.
- **RAC3.** Diagnosticar condiciones de carrera y diseñar soluciones de sincronización (mutex, semáforos) para problemas clásicos de concurrencia, evidenciando comprensión de deadlocks.
- **RAC4.** Modelar estrategias de gestión de memoria virtual (paginación, segmentación, FIFO, LRU) y evaluar su rendimiento experimentalmente.
- **RAC5.** Analizar la organización de sistemas de archivos, mecanismos de permisos y seguridad en plataformas reales (ext4, NTFS).
- **RAC6.** Administrar y monitorear procesos, memoria y recursos de E/S en entornos Linux y Windows mediante herramientas profesionales.
- **RAC7.** Explicar conceptualmente virtualización, contenedores y fundamentos de sistemas distribuidos sin requerir su implementación.
- **RAC8.** Documentar soluciones técnicas bajo el estándar IEEE y evaluarlas conforme a ISO/IEC 25010.
- **RAC9.** Comunicar y defender técnicamente sus soluciones ante audiencias evaluadoras, incluyendo el contexto interdisciplinario de Invenio Fest.

---

## 5. Unidades Temáticas

| Unidad | Título | Semanas aprox. |
|---|---|---|
| I | Arquitectura de Sistemas Operativos | 1–2 |
| II | Procesos y Scheduling | 3–4 |
| III | Concurrencia y Sincronización | 5–6 |
| IV | Gestión de Memoria | 7–8 |
| V | Sistemas de Archivos y Seguridad | 9 |
| VI | Entrada, Salida y Administración de Recursos | 10 |
| VII | Virtualización, Contenedores y Sistemas Modernos | 11 |
| — | Invenio Fest | 12 |

(Distribución detallada en el documento de Cronograma.)

---

## 6. Metodología

- Clases teóricas con discusión guiada (modalidad mixta).
- Lectura y trazado de código fuente de xv6 (sin modificación).
- Laboratorios comparativos Linux/Windows.
- Desarrollo incremental de 4 proyectos de simulación en C++.
- Documentación técnica bajo norma IEEE.
- Evaluación de calidad de software bajo ISO/IEC 25010.
- Participación en Invenio Fest desde la perspectiva de Sistemas Operativos.

---

## 7. Política de Uso de Inteligencia Artificial

Se permite el uso de ChatGPT, Claude, Gemini, Copilot y NotebookLM como herramientas de apoyo. **Toda solución entregada debe ser comprendida, explicada y defendida por el estudiante.** La incapacidad de explicar una solución propia podrá considerarse evidencia de falta de dominio del aprendizaje y será evaluada como tal en la Defensa Técnica Individual.

---

## 8. Esquema de Evaluación

| Componente | Peso |
|---|---|
| Proyecto 1 (Scheduling) + Documento IEEE | 18% |
| Proyecto 2 (Concurrencia) + Documento IEEE | 18% |
| Proyecto 3 (Gestión de Memoria) + Documento IEEE | 18% |
| Proyecto 4 (Administrador de Recursos) + Documento IEEE | 18% |
| Participación Técnica en Invenio Fest | 10% |
| Defensa Técnica Individual | 8% |
| Portafolio Técnico Final | 10% |
| **Total** | **100%** |

---

## 9. Portafolio Técnico Final

Documento integrador obligatorio que reúne los cuatro proyectos, sus documentos IEEE, resultados experimentales, evaluaciones ISO/IEC 25010, evidencias de laboratorio, evidencias de Invenio Fest y una reflexión profesional del estudiante. (Especificación completa en documento dedicado, Fase 4.)

---

## 10. Recursos Bibliográficos

**Texto base:**
- Silberschatz, A., Galvin, P. B., & Gagne, G. — *Operating System Concepts* (edición vigente).
- Tanenbaum, A. S. — *Modern Operating Systems* (edición vigente).
- Arpaci-Dusseau, R. & Arpaci-Dusseau, A. — *Operating Systems: Three Easy Pieces* (OSTEP, acceso libre).

**Recurso de implementación de referencia:**
- xv6 (MIT) — código fuente utilizado únicamente con fines de observación y análisis pedagógico.

**Normativa:**
- IEEE — plantillas de documentación técnica institucional.
- ISO/IEC 25010 — modelo de calidad de producto de software.

---

## 11. Alcance y Límites Explícitos del Curso

**El curso NO incluye:** administración de servidores, DevOps, Cloud Computing, ingeniería o programación de kernels, ciberseguridad ofensiva/defensiva, ni implementación de sistemas distribuidos.

**El curso SÍ incluye:** comprensión teórica, observación de implementación real (xv6), aplicación profesional comparada (Linux/Windows) y modelado computacional (C++) de los mecanismos fundamentales de un sistema operativo moderno.

---

## 12. Relación con Invenio Fest

Invenio Fest es un proyecto institucional interdisciplinario. **No constituye el proyecto final del curso ni reemplaza los cuatro proyectos de Sistemas Operativos.** La evaluación del curso en Invenio Fest se limita estrictamente a la perspectiva de Sistemas Operativos aplicada al Proyecto Semilla (procesos, memoria, recursos, almacenamiento, seguridad, virtualización), con entregables de póster, presentación, sección técnica y evidencias asociadas.
