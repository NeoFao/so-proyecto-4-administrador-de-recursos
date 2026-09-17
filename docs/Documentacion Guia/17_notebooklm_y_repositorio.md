# DISEÑO DE NOTEBOOKLM INSTITUCIONAL Y ESTRUCTURA DE REPOSITORIO GITHUB
## TIIT2007 – Sistemas Operativos | Universidad Invenio

---

# PARTE 1 — Diseño de NotebookLM Institucional del Curso

## 1.1 Propósito
Configurar un "notebook" (cuaderno de fuentes) en NotebookLM dedicado al curso, que sirva como asistente de estudio fundamentado exclusivamente en las fuentes oficiales del curso — evitando que el estudiante reciba respuestas genéricas no alineadas con el alcance y filosofía de TIIT2007.

## 1.2 Fuentes a Cargar en el Notebook

| Fuente | Tipo |
|---|---|
| Syllabus institucional | PDF/documento |
| Cronograma de 11 semanas | PDF/documento |
| Los 7 Planes de Clase semanales (consolidados) | PDF/documento |
| Las 7 Guías de Laboratorio | PDF/documento |
| Las 5 Guías de Análisis de xv6 | PDF/documento |
| Las 4 Especificaciones de Proyecto | PDF/documento |
| Plantilla IEEE institucional | PDF/documento |
| Rúbricas (4) | PDF/documento |
| Banco de 100 preguntas conceptuales | PDF/documento |
| Capítulos relevantes (resúmenes propios, no escaneos completos por derechos de autor) de Silberschatz, Tanenbaum y OSTEP | Notas/resúmenes elaborados por el docente |

**Nota de derechos de autor:** no cargar capítulos completos escaneados de los textos base sin licencia institucional vigente; usar resúmenes elaborados por el docente o fragmentos permitidos por la licencia de la editorial/OSTEP (de acceso abierto).

## 1.3 Usos Pedagógicos Sugeridos

- **Estudio guiado:** el estudiante pregunta sobre un concepto y recibe respuesta fundamentada únicamente en las fuentes del curso (evita desviaciones hacia contenido de administración de sistemas no cubierto).
- **Generación de resúmenes de audio** por unidad, como apoyo de repaso antes de cada cierre de proyecto.
- **Generación de guías de preguntas y respuestas** personalizadas por unidad, complementarias al Banco de 100 Preguntas.
- **Apoyo a la Defensa Técnica:** el estudiante puede autoevaluarse pidiendo al notebook que le formule preguntas de variación sobre su propio proyecto (sin que esto sustituya la preparación activa de explicar su código).

## 1.4 Política de Uso del Notebook Institucional

- El notebook es una herramienta de **apoyo al estudio**, no un sustituto de la lectura de las fuentes primarias ni de la elaboración propia de los documentos IEEE.
- Su uso está cubierto por la misma Política de IA del Syllabus: toda comprensión debe poder explicarse y defenderse sin apoyo de la herramienta.
- El docente debe revisar y actualizar las fuentes cargadas cada vez que se actualice algún documento oficial del curso, para evitar desincronización.

## 1.5 Mantenimiento

| Responsable | Tarea | Frecuencia |
|---|---|---|
| Docente titular | Actualizar fuentes tras cualquier cambio a syllabus/cronograma/especificaciones | Cada vez que se modifique un documento oficial |
| Docente titular | Revisar calidad de respuestas generadas (muestreo) | Quincenal durante el curso |
| Coordinación académica | Auditar que el notebook no contenga material con derechos de autor no autorizado | Una vez por trimestre |

---

# PARTE 2 — Estructura de Repositorio GitHub Oficial del Curso

## 2.1 Propósito
Proveer una estructura estándar para que cada estudiante organice el código fuente de sus 4 proyectos de forma profesional, y que el docente pueda auditar consistentemente entre estudiantes y cohortes.

## 2.2 Estructura de Carpetas (repositorio individual del estudiante)

```
TIIT2007-sistemas-operativos/
│
├── README.md                     # Descripción general, datos del estudiante, índice de proyectos
│
├── proyecto1-scheduling/
│   ├── src/                      # Código fuente C++
│   ├── data/                     # Datasets de entrada usados
│   ├── results/                  # Resultados experimentales (tablas/gráficos exportados)
│   ├── docs/
│   │   └── documento_ieee.pdf    # Documento IEEE del proyecto
│   └── README.md                 # Instrucciones de compilación y ejecución
│
├── proyecto2-concurrencia/
│   ├── src/
│   ├── data/
│   ├── results/
│   ├── docs/
│   └── README.md
│
├── proyecto3-memoria/
│   ├── src/
│   ├── data/
│   ├── results/
│   ├── docs/
│   └── README.md
│
├── proyecto4-administrador-recursos/
│   ├── src/
│   ├── data/
│   ├── results/
│   ├── docs/
│   └── README.md
│
├── invenio-fest/
│   ├── poster/
│   ├── presentacion/
│   └── evidencias/
│
└── portafolio/
    └── portafolio_tecnico_final.pdf
```

## 2.3 Normas Mínimas del README Raíz

El `README.md` raíz debe incluir:
1. Nombre del estudiante y curso.
2. Tabla con enlaces directos a cada carpeta de proyecto.
3. Instrucciones generales del entorno de compilación (compilador, versión de C++, sistema operativo usado para pruebas).
4. Declaración breve de uso de herramientas de IA generativa (qué se usó y para qué, conforme a la Política de IA del curso).

## 2.4 Convenciones de Commits (recomendadas, no obligatorias para la nota)

- Se recomienda un historial de commits incremental que refleje el progreso real del proyecto (evidencia adicional de autenticidad ante dudas de autoría), aunque el historial de commits **no es un criterio formal de la rúbrica** salvo que el docente lo incorpore explícitamente.

## 2.5 Uso del Repositorio en la Evaluación

- El repositorio es el medio de entrega del código fuente y evidencias para los 4 proyectos y el Portafolio Técnico Final.
- El enlace al repositorio se entrega junto con cada documento IEEE.
- El docente puede clonar y compilar el repositorio durante la Defensa Técnica como verificación de autenticidad y reproducibilidad.

## 2.6 Privacidad y Buenas Prácticas

- Se recomienda repositorio privado con acceso otorgado al docente, dado que el código es trabajo académico individual.
- No incluir credenciales, datos personales sensibles, ni información institucional confidencial en el repositorio.
