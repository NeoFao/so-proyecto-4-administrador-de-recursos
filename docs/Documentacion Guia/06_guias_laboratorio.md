# GUÍAS DE LABORATORIO
## TIIT2007 – Sistemas Operativos | Universidad Invenio

Cada guía sigue el formato: Objetivo → Requisitos previos → Procedimiento (Linux/Bash y Windows/PowerShell en paralelo) → Preguntas de análisis → Entregable. Ninguna guía requiere privilegios administrativos destructivos; todo procedimiento es observacional o de configuración reversible.

---

## LABORATORIO 1 — Identificación de Arquitectura del Sistema (Unidad I)

**Objetivo:** Identificar características arquitectónicas del sistema operativo en uso (kernel, versión, tipo de arquitectura).

**Requisitos previos:** Acceso a terminal Linux y consola PowerShell.

**Procedimiento:**

| Paso | Linux (Bash) | Windows (PowerShell) |
|---|---|---|
| 1 | `uname -a` | `systeminfo \| findstr /B /C:"OS"` |
| 2 | `lscpu` | `Get-ComputerInfo \| Select CsProcessors` |
| 3 | `cat /proc/version` | `Get-ComputerInfo \| Select OsVersion, OsBuildNumber` |
| 4 | `lsmod \| head` (módulos del kernel cargados) | `Get-WindowsDriver -Online \| Select -First 10` |

**Preguntas de análisis:**
1. ¿Qué evidencia encontraste de que Linux es un kernel monolítico modular?
2. ¿Qué información de `systeminfo` sugiere la arquitectura híbrida de Windows NT?
3. Compara la cantidad de módulos/drivers cargados — ¿qué te dice sobre la superficie del kernel?

**Entregable:** Captura de cada comando + respuestas de análisis (1 página máx., integrarse a bitácora).

---

## LABORATORIO 2 — Observación de Procesos (Unidad II)

**Objetivo:** Observar el ciclo de vida y atributos de procesos en ejecución.

**Procedimiento:**

| Paso | Linux (Bash) | Windows (PowerShell) |
|---|---|---|
| 1 | `ps aux` | `Get-Process` |
| 2 | `top` (observar 2 minutos, registrar PID con mayor %CPU) | `Get-Process \| Sort-Object CPU -Descending \| Select -First 5` |
| 3 | `ps -o pid,ppid,state,cmd -p <PID>` | `Get-Process -Id <PID> \| Format-List *` |
| 4 | `nice -n 10 sleep 60 &` luego `ps -o ni -p <PID>` | Ajustar prioridad desde Administrador de Tareas > Detalles > Establecer prioridad |

**Preguntas de análisis:**
1. ¿Qué estados de proceso observaste (running, sleeping, zombie, etc.)?
2. ¿Qué proceso(s) consumieron más CPU y por qué crees que ocurrió?
3. ¿Cómo afectó el cambio de prioridad (`nice`) el comportamiento del proceso?

**Entregable:** Tabla de 5 procesos observados con PID, estado, %CPU, prioridad (Linux y Windows) + análisis comparativo.

---

## LABORATORIO 3 — Evidencia de Condiciones de Carrera (Unidad III)

**Objetivo:** Provocar y observar evidencia de una condición de carrera controlada.

**Procedimiento (conceptual, complementario al Proyecto 2 en C++):**

| Paso | Linux (Bash) | Windows (PowerShell) |
|---|---|---|
| 1 | Ejecutar script con dos procesos en background escribiendo al mismo archivo sin bloqueo: `echo "A" >> race.txt & echo "B" >> race.txt &` | Ejecutar dos jobs en paralelo escribiendo al mismo archivo: `Start-Job { "A" \| Out-File race.txt -Append }` |
| 2 | Repetir 10 veces, observar inconsistencias en `race.txt` | Repetir 10 veces, observar inconsistencias en el archivo |
| 3 | Repetir el experimento usando `flock` para serializar el acceso | Repetir usando `Mutex` de .NET vía PowerShell |

**Preguntas de análisis:**
1. ¿Qué inconsistencias observaste sin protección?
2. ¿Desaparecieron al usar `flock`/`Mutex`? ¿Por qué?
3. Relaciona esta evidencia con la condición de carrera que tu Proyecto 2 debe demostrar formalmente.

**Entregable:** Evidencia (capturas) sin sincronización vs. con sincronización + explicación. Insumo directo para Documento IEEE de Proyecto 2.

---

## LABORATORIO 4 — Monitoreo de Memoria (Unidad IV)

**Objetivo:** Observar el comportamiento de la memoria del sistema bajo distintas cargas.

**Procedimiento:**

| Paso | Linux (Bash) | Windows (PowerShell) |
|---|---|---|
| 1 | `free -h` | `Get-Counter '\Memory\Available MBytes'` |
| 2 | `vmstat 2 5` (observar columnas `si`/`so` — swap) | `Get-Counter '\Paging File(_Total)\% Usage'` |
| 3 | Generar carga de memoria controlada (script que reserva arreglos grandes) y repetir paso 1–2 | Generar carga equivalente en PowerShell y repetir paso 1–2 |

**Preguntas de análisis:**
1. ¿Qué cambios observaste en memoria disponible y actividad de swap bajo carga?
2. ¿Hubo evidencia de thrashing? ¿Cómo lo identificaste?
3. Relaciona lo observado con los algoritmos FIFO/LRU de tu Proyecto 3.

**Entregable:** Tabla de métricas antes/durante carga + interpretación.

---

## LABORATORIO 5 — Permisos y Seguridad de Archivos (Unidad V)

**Objetivo:** Configurar y analizar permisos de archivos en ambos sistemas.

**Procedimiento:**

| Paso | Linux (Bash) | Windows (PowerShell) |
|---|---|---|
| 1 | `touch test.txt; ls -l test.txt` | `New-Item test.txt; Get-Acl test.txt \| Format-List` |
| 2 | `chmod 640 test.txt; ls -l test.txt` | `Set-Acl` con regla restringida equivalente |
| 3 | `chown <usuario>:<grupo> test.txt` (si aplica) | `icacls test.txt /grant <usuario>:R` |
| 4 | Crear usuario de prueba (entorno controlado) y verificar acceso denegado | Crear usuario local de prueba y verificar acceso denegado |

**Preguntas de análisis:**
1. ¿Cómo se representan los permisos en notación octal vs. ACL de Windows?
2. ¿Qué ventajas ofrece el modelo ACL de Windows sobre el modelo clásico Unix (rwx)?
3. ¿Qué riesgo de seguridad ilustra este laboratorio si los permisos se configuran incorrectamente?

**Entregable:** Capturas antes/después de cada cambio de permiso + tabla comparativa Linux/Windows.

---

## LABORATORIO 6 — Monitoreo Integral de Recursos (Unidad VI)

**Objetivo:** Construir un panel comparativo de métricas de CPU, memoria y procesos.

**Procedimiento:**

| Paso | Linux (Bash) | Windows (PowerShell) |
|---|---|---|
| 1 | `mpstat 2 5` o `top -bn1` | `Get-Counter '\Processor(_Total)\% Processor Time'` |
| 2 | `iostat -x 2 5` (si disponible) | `Get-Counter '\PhysicalDisk(_Total)\Disk Reads/sec'` |
| 3 | `ps aux --sort=-%mem \| head -5` | `Get-Process \| Sort-Object WS -Descending \| Select -First 5` |
| 4 | Consolidar resultados en tabla única | Consolidar resultados en tabla única |

**Preguntas de análisis:**
1. ¿Qué proceso(s) dominan el uso de CPU/memoria en cada sistema?
2. ¿Qué diferencias notas en la forma en que cada SO expone estas métricas?
3. ¿Qué insumo aporta este laboratorio al componente de monitoreo del Proyecto 4?

**Entregable:** Panel comparativo (tabla o captura de dashboard simple) + interpretación.

---

## LABORATORIO 7 — Observación de Contenedores (Unidad VII)

**Objetivo:** Observar (no administrar en profundidad) el funcionamiento básico de un contenedor.

**Procedimiento:**

| Paso | Linux (Bash, con Docker instalado) | Windows (PowerShell, con Docker Desktop) |
|---|---|---|
| 1 | `docker run hello-world` | `docker run hello-world` |
| 2 | `docker ps -a` | `docker ps -a` |
| 3 | `docker run -it ubuntu ps aux` (observar PID 1 dentro del contenedor) | Mismo comando vía PowerShell |
| 4 | Comparar el PID 1 visto dentro del contenedor vs. el proceso real visto desde el host (`ps aux \| grep docker`) | Comparar de forma equivalente |

**Preguntas de análisis:**
1. ¿Por qué el proceso dentro del contenedor tiene PID 1 aunque el host ya tenga muchos procesos corriendo?
2. ¿Qué mecanismos del kernel (namespaces, cgroups — mencionar conceptualmente) hacen esto posible?
3. ¿En qué se diferencia esto de una máquina virtual completa?

**Entregable:** Capturas + respuesta de análisis. Este laboratorio es insumo de discusión, no de evaluación sumativa directa.

---

## Nota General para el Docente

Todas las guías son reproducibles en máquinas con recursos modestos y no requieren modificación del sistema operativo anfitrión. Se recomienda ejecutar los laboratorios de Unidad III y IV en máquinas virtuales o contenedores desechables si el aula no cuenta con entornos aislados por estudiante.
