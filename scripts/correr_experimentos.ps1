# =============================================================================
# correr_experimentos.ps1 -- Reproduce TODA la evidencia experimental.
# Administrador Simplificado de Recursos (Proyecto 4)
# Universidad Invenio, TIIT2007 Sistemas Operativos
# Autor: Fabrizio Espinoza Arce
#
# POR QUE POWERSHELL Y NO BASH: en el Proyecto 1 el script de experimentos se
# escribio en Bash y no pudo ejecutarse nunca en la maquina de desarrollo. Esta
# es la ruta que realmente corre.
#
# Uso:
#   powershell -ExecutionPolicy Bypass -File scripts\correr_experimentos.ps1
#
# Deja en results\:
#   pruebas_windows.txt        suite completa en Windows
#   rendimiento_archivos.txt   RE-1: tiempos con 10, 100 y 1000 archivos
#   prueba_carga.txt           RE-2: los mismos tiempos con el sistema cargado
#   autoconsumo.txt            RE-3: consumo de la propia herramienta
#   vistas_menu.txt            las tres funciones del menu, RF-1, RF-2 y RF-3
#   errores_manejados.txt      RNF-1: los tres casos de error, provocados
#   pureza_plataforma.txt      evidencia de que windows.h vive en un solo sitio
#   csv\                       datos crudos para recalcular cualquier resumen
# =============================================================================

$ErrorActionPreference = "Stop"

$Raiz = Split-Path -Parent $PSScriptRoot
Set-Location $Raiz

Write-Host "=============================================================="
Write-Host " Experimentos -- Administrador de Recursos (Proyecto 4)"
Write-Host "=============================================================="
Write-Host ""

# --- Paso 0: entorno de compilacion ------------------------------------------
function Import-EntornoVisualStudio {
    param([string]$RutaVcvars)
    $salida = & cmd.exe /c "call `"$RutaVcvars`" >nul 2>&1 && set"
    foreach ($linea in $salida) {
        if ($linea -match '^([^=]+)=(.*)$') {
            [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2], "Process")
        }
    }
}

Write-Host "[0/8] Preparando el entorno de compilacion ..."
if (-not (Get-Command cl.exe -ErrorAction SilentlyContinue)) {
    $vcvars = $null
    foreach ($ed in @("Community", "Professional", "Enterprise", "BuildTools")) {
        foreach ($ver in @("18", "17", "16")) {
            $ruta = "${env:ProgramFiles}\Microsoft Visual Studio\$ver\$ed\VC\Auxiliary\Build\vcvars64.bat"
            if ((-not $vcvars) -and (Test-Path $ruta)) { $vcvars = $ruta }
        }
    }
    if (-not $vcvars) { throw "No se encontro cl.exe ni vcvars64.bat." }
    Write-Host "      Configurando entorno: $vcvars"
    Import-EntornoVisualStudio -RutaVcvars $vcvars
}

# Los .bat deben tener finales CRLF o cmd.exe no resuelve `call :etiqueta`.
# Trampa descubierta en el Proyecto 3; se normaliza para que no reaparezca.
if (Test-Path "$Raiz\scripts\normalizar_bat_crlf.py") {
    python "$Raiz\scripts\normalizar_bat_crlf.py" | Out-Null
}

Write-Host "      Compilando ..."
& cmd.exe /c "`"$Raiz\build.bat`"" | Out-Null
if ($LASTEXITCODE -ne 0) { throw "La compilacion fallo." }

$exe = Join-Path $Raiz "build\recursos.exe"
if (-not (Test-Path $exe)) { throw "No se genero $exe" }

# --- Directorios --------------------------------------------------------------
if (Test-Path "results\csv") { Remove-Item -Recurse -Force "results\csv" }
foreach ($d in @("results", "results\csv")) {
    New-Item -ItemType Directory -Force -Path $d | Out-Null
}
# Area de trabajo de los experimentos, FUERA del proyecto: crear y borrar miles
# de archivos dentro del arbol entregable dejaria basura si algo falla.
$AreaTrabajo = Join-Path $env:TEMP "p4_experimentos"
if (Test-Path $AreaTrabajo) { Remove-Item -Recurse -Force $AreaTrabajo }
New-Item -ItemType Directory -Force -Path $AreaTrabajo | Out-Null

# --- Paso 1: suite de pruebas -------------------------------------------------
# Va ANTES que los experimentos: no se generan resultados sobre un binario que
# no pasa sus propias pruebas.
Write-Host "[1/8] Ejecutando la suite de pruebas ..."
& cmd.exe /c "`"$Raiz\build.bat`" tests" | Out-File -Encoding utf8 "results\pruebas_windows.txt"
if ($LASTEXITCODE -ne 0) {
    throw "La suite no paso. No se generan resultados sobre un binario que falla."
}
$conteo = (Select-String -Path "results\pruebas_windows.txt" -Pattern "pruebas superadas").Line.Trim()
Write-Host "      $conteo"

# --- Paso 2: pureza de la capa de plataforma ---------------------------------
#
# LA SALIDA VACIA ES LA EVIDENCIA. Si <windows.h> apareciera fuera de
# src\plataforma\, la separacion que sostiene la respuesta sobre portabilidad
# seria falsa.
Write-Host "[2/8] Verificando que windows.h vive en un solo directorio ..."
$patron = '^\s*#\s*include\s*<windows\.h>'
$fuera = Select-String -Path "src\core\*.h", "src\core\*.cpp", "src\io\*.h", "src\io\*.cpp", "src\main.cpp" `
    -Pattern $patron -ErrorAction SilentlyContinue

$pureza = @(
    "Verificacion de la separacion de plataforma",
    "===========================================",
    "",
    "La respuesta a la pregunta de defensa 'que reescribirias para portar a",
    "Linux' es: solo los ficheros de src/plataforma/. Esta comprobacion es lo",
    "que convierte esa respuesta en un hecho y no en una estimacion.",
    "",
    "Se busca la directiva #include <windows.h> FUERA de src/plataforma/.",
    "LA SALIDA VACIA ES LA EVIDENCIA.",
    "",
    ("Ficheros examinados: " + (Get-ChildItem "src\core", "src\io" -File).Count + " + src/main.cpp"),
    ("Coincidencias fuera de plataforma/: " + ($fuera | Measure-Object).Count),
    ""
)
if ($fuera) {
    $pureza += "FALLA -- windows.h aparece fuera de la capa de plataforma:"
    $pureza += ($fuera | ForEach-Object { "  " + $_.ToString() })
} else {
    $pureza += "RESULTADO: salida vacia. Todo lo especifico de Windows esta"
    $pureza += "confinado en src/plataforma/SistemaWindows.cpp."
}
$pureza += ""
$pureza += "Ficheros especificos de plataforma (los unicos a reescribir):"
$pureza += (Get-ChildItem "src\plataforma" -File | ForEach-Object { "  src/plataforma/" + $_.Name })
$pureza | Out-File -Encoding utf8 "results\pureza_plataforma.txt"
Write-Host ("      Coincidencias fuera de plataforma/: " + ($fuera | Measure-Object).Count)

# --- Paso 3: las tres vistas del menu (RF-1, RF-2, RF-3) ---------------------
Write-Host "[3/8] Capturando las tres funciones del menu ..."
$dirVistas = Join-Path $AreaTrabajo "vistas"
New-Item -ItemType Directory -Force -Path $dirVistas | Out-Null

# Se conduce el menu por tuberia, exactamente como lo haria un usuario pero sin
# teclear. Es tambien la prueba de que la entrada se maneja con robustez.
$guion = @(
    "1", "2", "informe.txt", "contenido de prueba", # crear
    "1",                                             # listar
    "3", "informe.txt",                              # metadatos
    "0",                                             # volver
    "2",                                             # procesos (primera pagina)
    "0",                                             # salir del paginado
    "3",                                             # memoria
    "4",                                             # consumo propio (RE-3)
    "0"                                              # salir
) -join "`n"

# -InputObject con Out-String evita el BOM que PowerShell antepone al escribir
# a una tuberia; el programa lo tolera igualmente, pero asi la evidencia queda
# limpia.
$guion | & $exe --dir $dirVistas 2>&1 | Out-File -Encoding utf8 "results\vistas_menu.txt"

# --- Paso 4: RE-1, rendimiento con 10 / 100 / 1000 archivos ------------------
Write-Host "[4/8] RE-1: rendimiento con 10, 100 y 1000 archivos ..."
$dirBench = Join-Path $AreaTrabajo "bench"

"Resultado experimental 1 -- Rendimiento de las operaciones de archivo" |
    Out-File -Encoding utf8 "results\rendimiento_archivos.txt"
@(
    "=====================================================================",
    "",
    "Cantidades 10 / 100 / 1000 archivos de 1 KB: son las que fija el",
    "Dataset 4 del curso. Cada medicion se repite 5 veces y se reporta",
    "media +- desviacion estandar muestral.",
    ""
) | Out-File -Encoding utf8 -Append "results\rendimiento_archivos.txt"

# PASADA DE CALENTAMIENTO, DESCARTADA.
#
# Sin esto, la medicion normal corre en frio y la de bajo carga corre despues,
# con la cache del sistema de ficheros ya caliente. El sesgo favorece a la
# segunda y llega a invertir el resultado: en una corrida previa las
# operaciones "bajo carga" salieron un 20 % MAS RAPIDAS que las normales, que
# es obviamente falso. Calentando primero, las dos condiciones parten iguales.
Write-Host "      (calentamiento, se descarta) ..."
& $exe --benchmark --cantidad 100 --repeticiones 2 --dir $dirBench | Out-Null

foreach ($n in 10, 100, 1000) {
    Write-Host "      $n archivos ..."
    & $exe --benchmark --cantidad $n --repeticiones 5 --dir $dirBench `
           --csv "results\csv\rendimiento_$n.csv" |
        Out-File -Encoding utf8 -Append "results\rendimiento_archivos.txt"
}

# --- Paso 5: RE-3, consumo de la propia herramienta ---------------------------
Write-Host "[5/8] RE-3: consumo de la propia herramienta ..."
& $exe --autoconsumo --dir $dirVistas | Out-File -Encoding utf8 "results\autoconsumo.txt"

# --- Paso 6: RE-2, comportamiento bajo carga ---------------------------------
#
# La carga se genera desde este mismo script, sin software adicional, siguiendo
# el Laboratorio 4 (memoria) y el 6 (CPU): unos trabajos en segundo plano que
# reservan memoria y ocupan procesador. Se comparan los mismos tiempos del
# paso 4 con el sistema cargado.
Write-Host "[6/8] RE-2: los mismos tiempos con el sistema bajo carga ..."

$trabajos = @()
try {
    $nucleos = [Environment]::ProcessorCount
    $so = Get-CimInstance Win32_OperatingSystem
    $totalMb = [math]::Round($so.TotalVisibleMemorySize / 1024, 0)
    # FreePhysicalMemory es la memoria DISPONIBLE (libre + reutilizable), la
    # misma magnitud que la herramienta reporta con GlobalMemoryStatusEx.
    $antesMb = [math]::Round($so.FreePhysicalMemory / 1024, 0)

    # CUANTA MEMORIA OCUPAR.
    #
    # "Carga elevada" no puede ser una cifra fija: 1,5 GB es mucho en una
    # maquina de 4 GB y casi nada en una de 16 GB con la mitad libre. Se
    # calcula lo necesario para llevar la ocupacion del sistema al 85 %,
    # acotado entre 1,5 GB y 8 GB para no provocar intercambio masivo a disco.
    $objetivoMb = [math]::Round($antesMb - 0.15 * $totalMb, 0)
    $objetivoMb = [math]::Max(1536, [math]::Min(8192, $objetivoMb))

    # --- Carga de memoria (Laboratorio 4), PRIMERO y en solitario -----------
    #
    # DEFECTO DEF-007 (reapertura de DEF-002). La version anterior arrancaba
    # la carga de memoria A LA VEZ que la de CPU y esperaba 20 s a ciegas. Con
    # doce bucles de CPU compitiendo, el trabajo de memoria --que tocaba cada
    # pagina desde un bucle interpretado de PowerShell-- no llegaba a
    # terminar: la evidencia registro 91 MB ocupados en vez de 1,5 GB, y el
    # documento de calidad daba el defecto por cerrado.
    #
    # Ahora: (1) las paginas se tocan desde un metodo compilado de C#, que es
    # cientos de veces mas rapido; (2) el trabajo AVISA cuando termino de
    # reservar, y no se sigue hasta recibir el aviso; (3) solo entonces se
    # arranca la carga de CPU; (4) si la memoria ocupada no llega a la mitad
    # del objetivo, la evidencia lo dice en vez de publicarse como valida.
    Write-Host "      Carga de memoria: reservando $objetivoMb MB (objetivo: 85 % de ocupacion) ..."
    $trabajoMemoria = Start-Job -ArgumentList $objetivoMb -ScriptBlock {
        param($mb)
        Add-Type -TypeDefinition @'
public static class Paginas {
    // Escribe un byte por pagina de 4 KB: obliga al sistema a CONFIRMAR la
    // pagina. Un arreglo reservado y no tocado casi no ocupa memoria fisica.
    public static void Tocar(byte[] b) {
        for (int i = 0; i < b.Length; i += 4096) { b[i] = 1; }
    }
}
'@
        $bloques = New-Object System.Collections.ArrayList
        $bloqueMb = 256
        for ($reservado = 0; $reservado -lt $mb; $reservado += $bloqueMb) {
            $b = New-Object byte[] ($bloqueMb * 1MB)
            [Paginas]::Tocar($b)
            [void]$bloques.Add($b)
        }
        Write-Output "LISTO"
        Start-Sleep -Seconds 400
        $bloques = $null
    }
    $trabajos += $trabajoMemoria

    $limite = (Get-Date).AddSeconds(120)
    while (-not ((Receive-Job $trabajoMemoria -Keep) -contains "LISTO")) {
        if ($trabajoMemoria.State -ne "Running") {
            throw "El trabajo de carga de memoria termino antes de tiempo: $($trabajoMemoria.State)"
        }
        if ((Get-Date) -gt $limite) { throw "La carga de memoria no termino de reservar en 120 s." }
        Start-Sleep -Milliseconds 500
    }
    Start-Sleep -Seconds 2
    $conMemoriaMb = [math]::Round((Get-CimInstance Win32_OperatingSystem).FreePhysicalMemory / 1024, 0)
    $consumidoMb = $antesMb - $conMemoriaMb
    Write-Host "      Carga de memoria aplicada: $consumidoMb MB ocupados"

    # --- Carga de CPU (Laboratorio 6) ---------------------------------------
    # Misma leccion que con la memoria: una espera fija no garantiza nada.
    # Arrancar cada trabajo lanza un powershell.exe nuevo y tarda segundos; en
    # una corrida con 10 s de espera fija la CPU estaba al 9 % cuando empezo
    # a medirse. Cada trabajo avisa al entrar en su bucle, y no se mide hasta
    # que avisaron todos y el uso de CPU lo confirma.
    Write-Host "      Carga de CPU: $nucleos trabajos en bucle, uno por procesador logico ..."
    $trabajosCpu = @()
    for ($i = 0; $i -lt $nucleos; $i++) {
        $trabajosCpu += Start-Job -ScriptBlock {
            Write-Output "GIRANDO"
            $fin = (Get-Date).AddSeconds(420)
            while ((Get-Date) -lt $fin) { $null = [Math]::Sqrt((Get-Random)) }
        }
    }
    $trabajos += $trabajosCpu

    $limite = (Get-Date).AddSeconds(180)
    while (@($trabajosCpu | Where-Object { (Receive-Job $_ -Keep) -contains "GIRANDO" }).Count -lt $nucleos) {
        if ((Get-Date) -gt $limite) { throw "Los trabajos de CPU no arrancaron en 180 s." }
        Start-Sleep -Milliseconds 500
    }
    Start-Sleep -Seconds 3

    # Media de varias muestras: LoadPercentage es una foto de un instante.
    $muestras = 1..5 | ForEach-Object {
        (Get-CimInstance Win32_Processor | Measure-Object -Property LoadPercentage -Average).Average
        Start-Sleep -Milliseconds 400
    }
    $cpuPct = [math]::Round(($muestras | Measure-Object -Average).Average, 0)
    Write-Host "      Uso de CPU confirmado: $cpuPct %"

    & $exe --memoria --dir $dirVistas | Out-File -Encoding utf8 "results\prueba_carga.txt"

    $comprobacion = @(
        "",
        "COMPROBACION DE QUE LA CARGA EXISTIO",
        ("  Memoria fisica total                 : " + $totalMb + " MB"),
        ("  Disponible antes de la carga         : " + $antesMb + " MB"),
        ("  Disponible con la carga de memoria   : " + $conMemoriaMb + " MB"),
        ("  Memoria ocupada por la carga         : " + $consumidoMb + " MB (objetivo " + $objetivoMb + " MB)"),
        ("  Trabajos de CPU en ejecucion         : " + $nucleos + " (uno por procesador logico)"),
        ("  Uso de CPU al empezar a medir        : " + $cpuPct + " % (media de 5 muestras)"),
        ""
    )
    if ($cpuPct -lt 80) {
        $comprobacion += "  AVISO: el uso de CPU no llego al 80 %. Los resultados de abajo"
        $comprobacion += "  no deben presentarse como medidos bajo carga de CPU elevada."
        $comprobacion += ""
        Write-Host "      AVISO: la carga de CPU no se aplico por completo." -ForegroundColor Yellow
    }
    if ($consumidoMb -lt 0.5 * $objetivoMb) {
        $comprobacion += "  AVISO: la carga de memoria NO llego a la mitad del objetivo."
        $comprobacion += "  Los resultados de abajo no deben presentarse como medidos bajo"
        $comprobacion += "  carga de memoria elevada."
        $comprobacion += ""
        Write-Host "      AVISO: la carga de memoria no se aplico por completo." -ForegroundColor Yellow
    }
    $comprobacion | Out-File -Encoding utf8 -Append "results\prueba_carga.txt"

    @(
        "",
        "Resultado experimental 2 -- Comportamiento bajo carga",
        "=====================================================================",
        "",
        "Carga generada por scripts/correr_experimentos.ps1 segun los",
        "Laboratorios 4 y 6: un trabajo que reserva y escribe $objetivoMb MB de",
        "memoria, y $nucleos trabajos de CPU en bucle. Se repiten las mismas",
        "mediciones del RE-1 para poder compararlas linea a linea.",
        ""
    ) | Out-File -Encoding utf8 -Append "results\prueba_carga.txt"

    foreach ($n in 10, 100, 1000) {
        Write-Host "      $n archivos bajo carga ..."
        & $exe --benchmark --cantidad $n --repeticiones 5 --dir $dirBench `
               --csv "results\csv\carga_$n.csv" |
            Out-File -Encoding utf8 -Append "results\prueba_carga.txt"
    }

    & $exe --autoconsumo --dir $dirVistas |
        Out-File -Encoding utf8 -Append "results\prueba_carga.txt"

    # La carga debe seguir aplicada al TERMINAR de medir, no solo al empezar:
    # si algun trabajo hubiera muerto a mitad, las ultimas filas se habrian
    # medido sin carga.
    $vivos = @($trabajos | Where-Object { $_.State -eq "Running" }).Count
    $alFinalMb = [math]::Round((Get-CimInstance Win32_OperatingSystem).FreePhysicalMemory / 1024, 0)
    $cpuFinal = [math]::Round((Get-CimInstance Win32_Processor |
        Measure-Object -Property LoadPercentage -Average).Average, 0)
    @(
        "",
        "COMPROBACION AL TERMINAR LAS MEDICIONES",
        ("  Trabajos de carga aun en ejecucion   : " + $vivos + " de " + $trabajos.Count),
        ("  Memoria disponible                   : " + $alFinalMb + " MB"),
        ("  Uso de CPU                           : " + $cpuFinal + " %"),
        ""
    ) | Out-File -Encoding utf8 -Append "results\prueba_carga.txt"
    if ($vivos -ne $trabajos.Count) {
        Write-Host "      AVISO: algun trabajo de carga termino antes que las mediciones." -ForegroundColor Yellow
    }

} finally {
    # Los trabajos se detienen SIEMPRE, incluso si algo falla en medio: dejar
    # ocho bucles de CPU corriendo seria bastante peor que no tener la medida.
    Write-Host "      Retirando la carga ..."
    $trabajos | Stop-Job -ErrorAction SilentlyContinue
    $trabajos | Remove-Job -Force -ErrorAction SilentlyContinue
}

# --- Paso 7: RNF-1, los tres casos de error, provocados ----------------------
Write-Host "[7/8] RNF-1: provocando los tres casos de error ..."
$dirErr = Join-Path $AreaTrabajo "errores"
New-Item -ItemType Directory -Force -Path $dirErr | Out-Null

$errTxt = "results\errores_manejados.txt"
function Anotar { param([string[]]$Lineas) $Lineas | Out-File -Encoding utf8 -Append $errTxt }

@(
    "Requisito no funcional 1 -- Manejo robusto de errores",
    "=====================================================================",
    "",
    "El enunciado nombra tres casos: archivo inexistente, permisos",
    "insuficientes y comando no disponible. Los tres se provocan de verdad",
    "y se muestra la respuesta real de la herramienta. Ninguno termina el",
    "programa de forma anormal: todos vuelven al menu con un mensaje que",
    "dice que paso. Al final se anaden tres casos adicionales.",
    ""
) | Out-File -Encoding utf8 $errTxt

# Caso 1: archivo inexistente
Anotar "--- CASO 1 (enunciado): archivo inexistente ---"
("1`n3`nno_existe.txt`n0`n0`n") | & $exe --dir $dirErr 2>&1 |
    Select-String -Pattern "ERROR al" -Context 0,1 | Out-File -Encoding utf8 -Append $errTxt

# Caso 2: permisos insuficientes, REALES.
#
# Se deniega la escritura sobre el directorio de trabajo con icacls, para el
# grupo "Todos" por su SID (S-1-1-0): el nombre del grupo cambia con el idioma
# de Windows, el SID no. Se restaura en un finally para no dejar la carpeta
# bloqueada si algo falla.
Anotar @("", "--- CASO 2 (enunciado): permisos insuficientes ---",
         "    (icacls <dir> /deny *S-1-1-0:(W,AD) -- se deniega escribir en el directorio)")
$dirProtegido = Join-Path $AreaTrabajo "protegido"
New-Item -ItemType Directory -Force -Path $dirProtegido | Out-Null
try {
    & icacls $dirProtegido /deny "*S-1-1-0:(W,AD)" | Out-Null
    ("1`n2`nnuevo.txt`nx`n0`n0`n") | & $exe --dir $dirProtegido 2>&1 |
        Select-String -Pattern "ERROR al" -Context 0,1 | Out-File -Encoding utf8 -Append $errTxt
} finally {
    & icacls $dirProtegido /remove:d "*S-1-1-0" | Out-Null
}

# Caso 3: comando no disponible.
#
# No se puede provocar en el binario de produccion sin desinstalar tasklist.
# Se prueba en la suite (prueba 5.4) a traves de listarProcesosCon(), que
# acepta otro comando: se copia aqui el resultado de esa prueba.
Anotar @("", "--- CASO 3 (enunciado): comando no disponible ---",
         "    (prueba 5.4 de la suite: listarProcesosCon() con un comando inexistente)")
Select-String -Path "results\pruebas_windows.txt" -Pattern "5\.4 " |
    ForEach-Object { "    " + $_.Line.Trim() } | Out-File -Encoding utf8 -Append $errTxt

# Casos adicionales
Anotar @("", "--- ADICIONAL A: intento de escribir fuera del directorio controlado ---")
("1`n2`n../../intruso.txt`nx`n0`n0`n") | & $exe --dir $dirErr 2>&1 |
    Select-String -Pattern "ERROR al" -Context 0,1 | Out-File -Encoding utf8 -Append $errTxt

Anotar @("", "--- ADICIONAL B: intento de eliminar el propio directorio de trabajo ---")
("1`n4`n.`ns`n0`n0`n") | & $exe --dir $dirErr 2>&1 |
    Select-String -Pattern "ERROR al" -Context 0,1 | Out-File -Encoding utf8 -Append $errTxt
Anotar ("    El directorio sigue existiendo despues del intento: " + (Test-Path $dirErr))

Anotar @("", "--- ADICIONAL C: opcion de menu invalida ---")
("abc`n99`n0`n") | & $exe --dir $dirErr 2>&1 |
    Select-String -Pattern "no es una opcion valida" | Out-File -Encoding utf8 -Append $errTxt

# --- Paso 8: limpieza ---------------------------------------------------------
Write-Host "[8/8] Limpiando el area de trabajo temporal ..."
if (Test-Path $AreaTrabajo) { Remove-Item -Recurse -Force $AreaTrabajo -ErrorAction SilentlyContinue }

Write-Host ""
Write-Host "=============================================================="
Write-Host " Resumen"
Write-Host "=============================================================="
Write-Host "  $conteo"
$lineas = Select-String -Path "results\rendimiento_archivos.txt" -Pattern "^\| crear" |
    ForEach-Object { $_.Line.Trim() }
foreach ($l in $lineas) { Write-Host "  $l" }
Write-Host ""
Write-Host "  Evidencia completa en results\"
Write-Host "  Para la evidencia de Linux:  scripts\verificar_linux.ps1"
Write-Host "=============================================================="
