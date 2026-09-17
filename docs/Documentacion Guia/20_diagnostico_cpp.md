# DIAGNÓSTICO EXPLÍCITO DE C++
## TIIT2007 – Sistemas Operativos | Universidad Invenio

---

## 1. Propósito

Verificar, de forma temprana (Semana 1–2) y no asumida, que el estudiante posee el nivel de C++ necesario para enfrentar los 4 proyectos del curso. El curso no enseña C++ desde cero; este diagnóstico existe para **detectar brechas a tiempo**, no para filtrar/reprobar estudiantes.

---

## 2. Momento de Aplicación

Aplicar al cierre de la Semana 1 o apertura de la Semana 2, antes de la apertura formal del Proyecto 1. Duración sugerida: 45–60 minutos, formato individual, con código abierto/IDE permitido (es diagnóstico, no examen de memoria).

---

## 3. Competencias Mínimas Requeridas (mapeadas a los 4 proyectos)

| Competencia de C++ | Necesaria para |
|---|---|
| Estructuras de control (condicionales, bucles) | Todos los proyectos |
| Funciones y paso de parámetros (por valor/referencia) | Todos los proyectos |
| Structs/clases básicas | Proyecto 1 (PCB), Proyecto 3 (páginas) |
| Arreglos, vectores (`std::vector`) | Proyectos 1, 3 |
| Listas enlazadas o uso de `std::list`/`std::queue` | Proyectos 1, 3 |
| Punteros y referencias básicas | Todos los proyectos |
| Lectura/escritura de archivos (`fstream`) | Todos los proyectos (datasets de entrada) |
| Hilos (`std::thread`) y `std::mutex` | Proyecto 2 (puede no dominarlo aún — ver sección 6) |
| Manejo básico de errores (excepciones o validación manual) | Todos los proyectos |

---

## 4. Instrumento Diagnóstico

### Parte A — Ejercicios de código (30 min)

**A1.** Escribe una función que reciba un vector de enteros y retorne el promedio.

**A2.** Define una estructura (`struct`) que represente un proceso con: id (entero), tiempo de llegada (entero), ráfaga de CPU (entero). Crea un vector de 3 procesos y muéstralos por pantalla.

**A3.** Escribe una función que lea una línea de un archivo de texto separada por comas (`id,llegada,rafaga`) y la convierta en los 3 valores correspondientes.

**A4.** Implementa una cola simple (puede usar `std::queue` o lista enlazada propia) e inserta/extrae 3 elementos, mostrando el orden de salida.

**A5.** Explica (en comentario, no requiere código funcional) qué diferencia hay entre pasar un parámetro por valor y por referencia en C++, y por qué importa para el rendimiento de un simulador.

### Parte B — Autoevaluación de confianza (10 min)

El estudiante califica su propia confianza (1–5) en cada competencia de la tabla de la sección 3, **antes** de ver los resultados de la Parte A. Esto permite detectar brechas de autopercepción (estudiantes que se sobreestiman o subestiman).

---

## 5. Rúbrica de Diagnóstico (no es nota sumativa del curso)

| Resultado | Significado | Acción recomendada |
|---|---|---|
| **Verde** (4–5 de 5 ejercicios correctos) | Nivel adecuado para iniciar Proyecto 1 sin refuerzo | Ninguna acción adicional |
| **Amarillo** (2–3 de 5 correctos) | Brechas puntuales identificables | Asignar 1–2 ejercicios de refuerzo del Banco de Ejercicios Bash/PowerShell adaptado a C++ (ver nota 5.1), o sesión de consulta dirigida en Semana 2–3 |
| **Rojo** (0–1 de 5 correctos) | Brecha significativa en prerrequisitos | Reunión individual con el docente; evaluar plan de refuerzo intensivo antes de Semana 4 (cierre de Proyecto 1); en casos extremos, consulta con coordinación académica sobre la pertinencia de cursar el curso en este momento |

**5.1 Nota:** Se recomienda que el docente prepare un mini-banco de 5–10 ejercicios de refuerzo de C++ (estructuras, vectores, archivos, hilos básicos) para estudiantes en zona Amarilla/Roja, a desarrollar localmente según el perfil real de cada cohorte.

---

## 6. Caso Especial — Hilos y Concurrencia (Proyecto 2)

Es esperable que muchos estudiantes **no** dominen `std::thread`/`std::mutex` al inicio del curso, ya que no es un prerrequisito típico de los cursos de Programación previos. Por ello:
- El diagnóstico **no** penaliza desconocimiento de hilos en la Parte A (ejercicios A1–A4 son los determinantes para el semáforo).
- La Unidad III (Semanas 5–6) incluye explícitamente la introducción práctica a `std::thread`/`std::mutex` antes de exigir su uso en el Proyecto 2.

---

## 7. Uso de los Resultados

- **No forma parte del 100% de la nota del curso.**
- Se archiva como evidencia formativa en el expediente del curso (no en el Portafolio Técnico del estudiante).
- Alimenta el indicador "Nivel de entrada de la cohorte" dentro de la Estrategia de Mejora Continua y el nuevo Indicador de Evolución entre Proyectos (ver documento dedicado), al servir como línea base de comparación contra el desempeño en Proyecto 1.

---

## 8. Plantilla de Registro de Resultados (uso docente)

| Estudiante | A1 | A2 | A3 | A4 | A5 | Semáforo | Autoconfianza promedio | Acción tomada |
|---|---|---|---|---|---|---|---|---|
| | | | | | | | | |
