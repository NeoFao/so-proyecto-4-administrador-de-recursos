# PLANTILLA IEEE INSTITUCIONAL
## TIIT2007 – Sistemas Operativos | Universidad Invenio

Esta plantilla aplica a los cuatro proyectos del curso. El documento debe entregarse en formato de dos columnas (estilo IEEE conference) cuando se use Word/LaTeX para la versión final; para borradores de trabajo se acepta formato de una columna siguiendo esta estructura.

---

### Encabezado (primera página)

```
[Título del Proyecto]
[Nombre del Estudiante] — Universidad Invenio
Licenciatura en Tecnologías de Información y Comunicación Empresarial
TIIT2007 – Sistemas Operativos | [Fecha]
```

---

## 1. Resumen (Abstract)
*150–250 palabras.* Síntesis del problema abordado, metodología, resultados principales y conclusión más relevante. Debe poder leerse de forma independiente al resto del documento.

## 2. Introducción
- Contexto del problema (¿qué mecanismo de SO se está modelando y por qué es relevante?).
- Objetivo general y objetivos específicos del proyecto.
- Alcance y limitaciones explícitas (qué se implementó y qué no).

## 3. Marco Teórico
- Fundamentos conceptuales necesarios (citar Silberschatz/Tanenbaum/OSTEP según corresponda).
- Descripción de los algoritmos/mecanismos involucrados (p. ej. FCFS, Round Robin, mutex, FIFO, LRU según el proyecto).
- Relación con la observación realizada en xv6 (si aplica a la unidad correspondiente).

## 4. Diseño de la Solución
- Arquitectura general del simulador/herramienta (diagrama de clases o de flujo).
- Estructuras de datos seleccionadas y justificación.
- Supuestos de diseño y decisiones de alcance.

## 5. Implementación
- Lenguaje y estándar utilizado (C++17 o superior).
- Descripción de los módulos/clases principales (sin pegar el código completo; fragmentos relevantes únicamente).
- Instrucciones de compilación y ejecución.

## 6. Resultados Experimentales
- Presentación de los datos obligatorios definidos en la especificación del proyecto correspondiente (tablas, gráficos).
- Descripción del entorno de prueba (hardware/SO usado para correr los experimentos).
- Al menos una comparación cuantitativa explícita entre algoritmos/escenarios.

## 7. Evaluación ISO/IEC 25010
- Autoevaluación de las características de calidad aplicables (rendimiento, eficiencia, fiabilidad, usabilidad, compatibilidad, mantenibilidad) usando la Rúbrica ISO/IEC 25010 institucional.
- Justificación breve de cada calificación otorgada.

## 8. Discusión de Resultados
- Interpretación de los resultados obtenidos: ¿qué algoritmo/escenario fue superior y bajo qué condiciones?
- Relación con la teoría: ¿los resultados coinciden con lo esperado teóricamente? ¿Por qué sí o por qué no?
- Limitaciones encontradas durante el desarrollo.

## 9. Conclusiones
- Síntesis de los hallazgos principales (no repetir el resumen literalmente).
- Aprendizajes clave del estudiante respecto al mecanismo de SO estudiado.
- Posibles extensiones futuras (fuera del alcance del curso).

## 10. Referencias
- Formato IEEE numerado: `[1] Apellido, Inicial. (Año). Título. Editorial.`
- Mínimo 3 referencias: los tres textos base del curso (Silberschatz, Tanenbaum, OSTEP) deben citarse cuando sea pertinente; pueden añadirse fuentes adicionales.
- Toda fuente citada en el cuerpo del documento debe aparecer aquí, y viceversa.

---

## Normas de Formato

| Elemento | Norma |
|---|---|
| Fuente | Times New Roman 10pt (cuerpo), 24pt (título) — o equivalente si se usa otra herramienta |
| Extensión | 4–8 páginas (excluyendo referencias y anexos) |
| Figuras/Tablas | Numeradas y referenciadas en el texto (`Tabla 1`, `Figura 2`) con título descriptivo |
| Código fuente | No se incluye in extenso en el documento; se entrega como anexo/repositorio aparte, con fragmentos breves solo si ilustran un punto específico |
| Citas | Formato IEEE: número entre corchetes `[1]` en el texto |

## Checklist de Entrega del Documento IEEE

- [ ] Las 10 secciones están presentes y en el orden correcto.
- [ ] El resumen es autocontenible (no requiere leer el resto para entenderlo).
- [ ] Los resultados experimentales obligatorios del proyecto correspondiente están incluidos.
- [ ] La autoevaluación ISO/IEC 25010 está justificada, no solo calificada.
- [ ] Todas las referencias citadas en el texto aparecen en la sección 10, y viceversa.
- [ ] El documento fue revisado por el estudiante y puede ser defendido oralmente sin apoyo de notas externas.
