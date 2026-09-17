# BANCO DE EJERCICIOS — BASH Y POWERSHELL
## TIIT2007 – Sistemas Operativos | Universidad Invenio

Ejercicios organizados por unidad, con versión equivalente en Bash (Linux) y PowerShell (Windows). Nivel de dificultad: ● básico, ●● intermedio, ●●● avanzado.

---

## Unidad I — Arquitectura

**E1.1 ●** Muestra información del kernel y la arquitectura del sistema.
- Bash: `uname -a`
- PowerShell: `Get-ComputerInfo | Select-Object OsName, OsArchitecture`

**E1.2 ●●** Lista los primeros 10 módulos/drivers cargados por el kernel.
- Bash: `lsmod | head -10`
- PowerShell: `Get-WindowsDriver -Online | Select-Object -First 10 DriverVersion, OriginalFileName`

---

## Unidad II — Procesos

**E2.1 ●** Lista todos los procesos activos con su PID y consumo de CPU.
- Bash: `ps aux --sort=-%cpu | head -10`
- PowerShell: `Get-Process | Sort-Object CPU -Descending | Select-Object -First 10`

**E2.2 ●●** Cambia la prioridad de un proceso específico y verifica el cambio.
- Bash: `renice -n 5 -p <PID>` luego `ps -o ni -p <PID>`
- PowerShell: `(Get-Process -Id <PID>).PriorityClass = "BelowNormal"`

**E2.3 ●●● ** Escribe un script que liste los 5 procesos con mayor uso de memoria, junto a su PID y nombre, en formato de tabla legible.
- Bash: combinar `ps aux --sort=-%mem | head -6 | awk '{print $2, $4, $11}'`
- PowerShell: `Get-Process | Sort-Object WS -Descending | Select-Object -First 5 Id, ProcessName, WS`

---

## Unidad III — Concurrencia

**E3.1 ●●** Lanza dos procesos en segundo plano que escriban simultáneamente al mismo archivo y observa el resultado.
- Bash: `echo "linea A" >> out.txt & echo "linea B" >> out.txt & wait; cat out.txt`
- PowerShell: `Start-Job { "linea A" | Out-File out.txt -Append }; Start-Job { "linea B" | Out-File out.txt -Append }; Wait-Job *; Get-Content out.txt`

**E3.2 ●●●** Repite el ejercicio E3.1 usando un mecanismo de bloqueo (`flock` en Bash, `Mutex` en PowerShell) y compara los resultados.
- Bash: usar `flock out.lock -c '...'` envolviendo cada escritura.
- PowerShell: usar `[System.Threading.Mutex]` para serializar el acceso.

---

## Unidad IV — Memoria

**E4.1 ●** Muestra el estado actual de memoria del sistema.
- Bash: `free -h`
- PowerShell: `Get-Counter '\Memory\Available MBytes'`

**E4.2 ●●** Observa la actividad de swap durante 10 segundos, muestreando cada 2 segundos.
- Bash: `vmstat 2 5`
- PowerShell: `Get-Counter '\Paging File(_Total)\% Usage' -Continuous -SampleInterval 2 -MaxSamples 5`

**E4.3 ●●●** Escribe un script que reserve progresivamente bloques de memoria (arreglos) y registre el uso de memoria disponible en cada paso, deteniéndose antes de comprometer la estabilidad del sistema (límite sugerido: no superar 70% de uso).

---

## Unidad V — Archivos y Seguridad

**E5.1 ●** Crea un archivo y muestra sus permisos actuales.
- Bash: `touch demo.txt && ls -l demo.txt`
- PowerShell: `New-Item demo.txt; Get-Acl demo.txt | Format-List`

**E5.2 ●●** Modifica los permisos para que solo el propietario pueda leer y escribir.
- Bash: `chmod 600 demo.txt`
- PowerShell: usar `icacls demo.txt /inheritance:r /grant:r "$env:USERNAME:(R,W)"`

**E5.3 ●●●** Escribe un script que recorra un directorio y reporte todos los archivos con permisos de escritura abiertos a "otros" (Linux) o a "Everyone" (Windows).
- Bash: combinar `find . -perm -o+w`
- PowerShell: combinar `Get-ChildItem -Recurse | Get-Acl` filtrando reglas que incluyan `Everyone`

---

## Unidad VI — E/S y Recursos

**E6.1 ●** Muestra el uso de CPU promedio del sistema.
- Bash: `mpstat 2 3` (o `top -bn1 | head -5` si `mpstat` no está disponible)
- PowerShell: `Get-Counter '\Processor(_Total)\% Processor Time'`

**E6.2 ●●** Muestra actividad de lectura/escritura en disco.
- Bash: `iostat -x 2 3`
- PowerShell: `Get-Counter '\PhysicalDisk(_Total)\Disk Reads/sec', '\PhysicalDisk(_Total)\Disk Writes/sec'`

**E6.3 ●●●** Construye un script que consolide en una sola tabla: % CPU, memoria disponible y top 3 procesos por consumo, ejecutándose cada 5 segundos durante 1 minuto.

---

## Unidad VII — Virtualización y Contenedores

**E7.1 ●** Verifica que Docker esté disponible y ejecuta un contenedor de prueba.
- Bash: `docker run hello-world`
- PowerShell: `docker run hello-world`

**E7.2 ●●** Lista los procesos vistos desde dentro de un contenedor y compáralos con los vistos desde el host.
- Bash: `docker run -it ubuntu ps aux` luego, en otra terminal del host, `ps aux | grep docker`
- PowerShell: equivalente con `docker run -it ubuntu ps aux` desde PowerShell con Docker Desktop

---

## Clave de Corrección (uso docente)

Cada ejercicio se evalúa de forma binaria (correcto/incorrecto) o por checklist de pasos completados, según el formato de la actividad en que se use (laboratorio formativo, quiz, o insumo de proyecto). No forman parte directa del 100% de la nota salvo que el docente los incorpore explícitamente a un instrumento de evaluación formal.
