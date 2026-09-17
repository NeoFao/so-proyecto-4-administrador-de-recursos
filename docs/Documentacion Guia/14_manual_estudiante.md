# MANUAL DEL ESTUDIANTE
## TIIT2007 – Sistemas Operativos | Universidad Invenio

---

## 1. Bienvenida y Propósito del Curso

Este curso te lleva a comprender **cómo funciona realmente** un sistema operativo: cómo gestiona procesos, memoria, archivos y recursos. No es un curso de administración de servidores ni de Linux/Windows avanzado — es un curso de **comprensión, análisis y modelado**. Al final, podrás explicar y simular en C++ los mecanismos que hacen posible que tu computadora ejecute múltiples programas a la vez sin colapsar.

---

## 2. ¿Qué NO es este curso?

- No es Administración Linux ni DevOps.
- No es Cloud Computing.
- No es ingeniería ni programación de kernels.
- No es Ciberseguridad.
- **No vas a modificar ni programar xv6** — solo lo observarás como ejemplo real simplificado.

## 3. ¿Qué SÍ es este curso?

Vas a recorrer, en cada unidad, cuatro perspectivas:
1. **Teoría** (Silberschatz, Tanenbaum, OSTEP).
2. **xv6** — observar cómo un SO real simplificado implementa lo que aprendes.
3. **Linux y Windows** — aplicar los conceptos con herramientas profesionales reales (Bash y PowerShell).
4. **Simulación en C++** — construir tú mismo un modelo del mecanismo estudiado.

---

## 4. Estructura del Curso

- 11 semanas efectivas + Semana 12: Invenio Fest.
- 4 horas semanales, modalidad mixta.
- 4 proyectos progresivos en C++ (Scheduling, Concurrencia, Memoria, Administrador de Recursos), cada uno con su documento técnico IEEE.
- Defensa Técnica Individual y Portafolio Técnico Final al cierre del curso.

## 5. ¿Cómo se evalúa mi nota?

| Componente | Peso |
|---|---|
| Proyecto 1 + IEEE | 18% |
| Proyecto 2 + IEEE | 18% |
| Proyecto 3 + IEEE | 18% |
| Proyecto 4 + IEEE | 18% |
| Participación Técnica en Invenio Fest | 10% |
| Defensa Técnica Individual | 8% |
| Portafolio Técnico Final | 10% |

---

## 6. Tus Proyectos, en Breve

1. **Simulador de Scheduling** (Semana 4): FCFS y Round Robin; mides tiempos de espera, retorno y utilización de CPU.
2. **Simulador de Concurrencia** (Semana 6): Productor-Consumidor con hilos reales; demuestras una condición de carrera y luego la corriges con mutex.
3. **Simulador de Gestión de Memoria** (Semana 8): FIFO y LRU; comparas fallos de página.
4. **Administrador Simplificado de Recursos** (Semana 11): gestión de archivos + monitoreo de procesos y memoria del sistema real.

Cada proyecto requiere: código funcional, resultados experimentales reales (no inventados) y un documento técnico bajo norma IEEE.

---

## 7. Sobre el Uso de Inteligencia Artificial

Puedes usar ChatGPT, Claude, Gemini, Copilot o NotebookLM como apoyo. **Regla de oro: si lo entregas, debes poder explicarlo.** En tu Defensa Técnica Individual se te pedirá explicar decisiones de tu propio proyecto, incluyendo preguntas que no estaban explícitamente cubiertas en tu documento. No poder explicar tu propia solución se considera evidencia de que no dominaste el aprendizaje, sin importar qué tan bien funcione el código.

---

## 8. Sobre xv6

xv6 es un sistema operativo real, simplificado, creado con fines educativos en el MIT. Lo vas a **leer**, no a modificar. Cada unidad incluye una Guía de Análisis de xv6 con preguntas guía sobre archivos específicos del código fuente (`proc.c`, `vm.c`, `fs.c`, etc.). El objetivo es que veas con tus propios ojos cómo la teoría se convierte en código real.

---

## 9. Sobre Invenio Fest

Invenio Fest es un evento institucional interdisciplinario, **no** el proyecto final de este curso. Tu participación se evalúa solo desde la perspectiva de Sistemas Operativos aplicada al Proyecto Semilla en el que participes: ¿qué procesos corren?, ¿cómo se usa la memoria?, ¿qué recursos consume?, ¿cómo se almacenan los datos?, ¿qué medidas de seguridad tiene?, ¿usa virtualización o contenedores? Prepararás un póster, una presentación, una sección técnica y evidencias asociadas.

---

## 10. Tu Portafolio Técnico Final

Al final del curso, reunirás en un solo portafolio: tus 4 proyectos, sus documentos IEEE, resultados experimentales, autoevaluaciones ISO/IEC 25010, evidencias de laboratorio, evidencias de Invenio Fest y una reflexión profesional personal sobre lo aprendido. (Especificación completa en documento dedicado.)

---

## 11. Consejos Prácticos para Tener Éxito

- No dejes el documento IEEE para el final — escríbelo en paralelo a tu código; te ayudará a pensar mejor el diseño.
- Ejecuta tus experimentos varias veces; los datos deben ser reales y reproducibles, no estimados.
- Aprovecha las Guías de Análisis de xv6 — conectan directamente con lo que se te preguntará en la Defensa Técnica.
- Practica explicar tu código en voz alta antes de la Defensa Técnica, como si se lo explicaras a un compañero que no lo ha visto.
- Si usaste IA para generar parte de tu solución, tómate el tiempo de entender cada línea — te la pueden preguntar.

---

## 12. ¿Qué Necesito Tener Listo Cada Semana?

Revisa el Cronograma del curso: cada semana tiene una entrega esperada (bitácora, guía de análisis xv6, avance de proyecto, o entrega formal del proyecto). Llegar al día con cada hito evita la acumulación de trabajo antes de las semanas 4, 6, 8 y 11.
