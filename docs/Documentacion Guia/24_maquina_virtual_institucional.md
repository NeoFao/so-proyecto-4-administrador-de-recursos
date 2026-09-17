# GUÍA DE CONSTRUCCIÓN — MÁQUINA VIRTUAL INSTITUCIONAL
## TIIT2007 – Sistemas Operativos | Universidad Invenio

No es posible generar aquí una imagen de VM binaria; este documento es la **especificación y guion de construcción** para que el equipo de TI/laboratorio de Invenio produzca una imagen reutilizable, idéntica para todos los estudiantes, evitando que cada uno pierda tiempo de la Semana 1 instalando entorno.

---

## 1. Propósito
Eliminar la variable "el entorno no funciona igual en cada computadora" como fuente de fricción del curso, garantizando que todo estudiante tenga, desde el primer día, xv6+QEMU, herramientas Linux y Docker funcionando de forma idéntica.

## 2. Especificación de la Imagen Base

| Elemento | Especificación recomendada |
|---|---|
| Sistema operativo base | Ubuntu LTS (versión estable vigente al momento de construir la imagen) |
| Recursos mínimos asignados | 2 vCPU, 4 GB RAM, 20 GB disco |
| Hipervisor objetivo | Compatible con VirtualBox y VMware (exportar en formato OVA para portabilidad) |
| Usuario por defecto | `invenio-student` con permisos sudo limitados (no root por defecto) |

## 3. Software a Preinstalar

| Componente | Uso en el curso |
|---|---|
| Build essentials (`gcc`, `make`, `g++` con soporte C++17) | Compilación de los 4 proyectos |
| QEMU (`qemu-system-x86_64`) | Ejecución de xv6 |
| xv6 (código fuente clonado, **sin modificar**, en `/opt/xv6/`, en modo solo lectura para el usuario estándar) | Unidades I–V, Guías de Análisis xv6 |
| Git | Gestión del Repositorio Maestro/individual del estudiante |
| Docker Engine | Laboratorio 7 / Unidad VII |
| Herramientas de monitoreo (`htop`, `vmstat`, `iostat`, `mpstat`, `sysstat`) | Laboratorios 2, 4, 6 |
| Editor de código (VS Code o equivalente ligero) | Desarrollo de los 4 proyectos |
| PowerShell Core (`pwsh`) — opcional, para estudiantes que quieran practicar PowerShell también desde Linux | Equivalencia parcial de ejercicios PowerShell (no reemplaza una máquina Windows real) |

**Nota:** para los laboratorios específicos de PowerShell nativo (ACL de Windows, `Get-Counter`, etc.), se recomienda complementar con acceso a una máquina Windows institucional (física o VM aparte), ya que `pwsh` en Linux no replica completamente las APIs de Windows (ACL, contadores de rendimiento nativos).

## 4. Estructura de Carpetas Preconfigurada dentro de la VM

```
/home/invenio-student/
├── TIIT2007/
│   ├── README.md              # Bienvenida + enlace al Manual del Estudiante
│   ├── xv6-link -> /opt/xv6/  # enlace simbólico de solo lectura
│   ├── laboratorios/          # carpeta vacía, plantilla para el estudiante
│   └── proyectos/             # carpeta vacía, plantilla para el estudiante
```

## 5. Script de Verificación de Entorno (a ejecutar por el estudiante en Semana 1)

Guion conceptual que el equipo técnico debe materializar como script real (`verificar_entorno.sh`):
1. Verifica versión de `g++` y soporte C++17.
2. Verifica que QEMU puede arrancar xv6 sin errores (`make qemu-nox` o equivalente, ejecución de prueba).
3. Verifica que Docker está activo (`docker run hello-world`).
4. Verifica herramientas de monitoreo instaladas.
5. Imprime un resumen "✅ Entorno listo" o lista de componentes faltantes.

Este script debe integrarse como parte de la "Bitácora de instalación de entorno" de la Semana 1 (ver Planes de Clase).

## 6. Política de Modificación de xv6 en la VM

- El código de xv6 se monta en modo **solo lectura** o se ubica fuera del directorio de trabajo habitual del estudiante, reforzando técnicamente (no solo normativamente) que no se modifica ni se compila con cambios.
- Si un estudiante necesita "jugar" con una copia editable por curiosidad personal fuera del alcance del curso, debe clonar su propia copia en una carpeta claramente separada (`~/sandbox-personal/`), no evaluada ni parte de las Guías de Análisis xv6.

## 7. Distribución y Mantenimiento

| Tarea | Responsable | Periodicidad |
|---|---|---|
| Construcción inicial de la imagen | Equipo de TI/laboratorio de Invenio | Una vez, antes del primer trimestre de oferta |
| Actualización de versión de Ubuntu/herramientas | Equipo de TI/laboratorio | Anual, o ante vulnerabilidades críticas reportadas |
| Distribución a estudiantes (descarga o laboratorio físico) | Coordinación académica + TI | Antes de la Semana 1 de cada cohorte |
| Verificación de checksum/integridad de la imagen | Equipo de TI | Cada distribución |

## 8. Alternativa Ligera (si no es viable una VM completa)

Si la infraestructura institucional no permite distribuir una imagen de VM completa, se recomienda como alternativa mínima viable: un contenedor Docker con Ubuntu + toolchain de compilación + QEMU + xv6 preconfigurado, documentado con un `Dockerfile` versionado en el Repositorio Maestro del curso (ver documento dedicado). Esto reduce drásticamente el tamaño de distribución comparado con una VM completa, a costa de no poder ejecutar Docker-dentro-de-Docker fácilmente para el Laboratorio 7 (requeriría ajuste adicional, p. ej. Docker-in-Docker o acceso al socket del host).
