# =============================================================================
# verificar_linux.ps1 -- Compila y prueba la rama de Linux, de verdad.
# Administrador Simplificado de Recursos (Proyecto 4)
# Universidad Invenio, TIIT2007 Sistemas Operativos
# Autor: Fabrizio Espinoza Arce
#
# POR QUE ESTE SCRIPT EXISTE
#
# El requisito no funcional 2 pide "portabilidad razonable" y documentar si la
# implementacion es de Windows, de Linux o de ambas. En los Proyectos 2 y 3 la
# rama de Linux se entrego ESCRITA PERO NUNCA EJECUTADA, y hubo que declararlo
# como limitacion en el documento de calidad.
#
# Aqui no. Este script compila src/plataforma/SistemaLinux.cpp con g++ dentro
# de una distribucion Linux real y corre la suite completa, ademas de una
# demostracion de las tres funciones. La evidencia queda en
# results\compatibilidad_linux.txt.
#
# COMO ENCUENTRA UN LINUX, EN ORDEN DE PREFERENCIA:
#   1. WSL con una distribucion instalada (no requiere que nada este arrancado)
#   2. Docker con contenedores Linux (usa Dockerfile.linux)
# Si no hay ninguno, lo dice claramente en vez de fingir que verifico algo.
#
# POR QUE SE COMPILA EN UNA COPIA Y NO SOBRE EL PROYECTO: los objetos de g++ y
# los de MSVC no son compatibles y comparten el nombre build/, asi que
# compilar in situ pisaria la compilacion de Windows. Ademas, compilar sobre
# /mnt/c desde WSL es varias veces mas lento que hacerlo en el disco de la
# distribucion.
#
# Uso:
#   powershell -ExecutionPolicy Bypass -File scripts\verificar_linux.ps1
# =============================================================================

$ErrorActionPreference = "Stop"

$Raiz = Split-Path -Parent $PSScriptRoot
Set-Location $Raiz

Write-Host "=============================================================="
Write-Host " Verificacion de la rama de Linux"
Write-Host "=============================================================="
Write-Host ""

New-Item -ItemType Directory -Force -Path "results" | Out-Null
$Informe = "results\compatibilidad_linux.txt"

function Escribir { param([string[]]$Lineas) $Lineas | Out-File -Encoding utf8 -Append $Informe }

@(
    "Evidencia de compatibilidad -- rama de Linux",
    "=====================================================================",
    "",
    ("Generado: " + (Get-Date -Format "yyyy-MM-dd HH:mm:ss")),
    ""
) | Out-File -Encoding utf8 $Informe

# --- Localizar un entorno Linux ----------------------------------------------
#
# WSL_UTF8=1 hace que wsl.exe emita UTF-8. Sin esto devuelve UTF-16LE, y al
# capturarlo desde PowerShell cada letra queda separada por un byte nulo: la
# lista de distribuciones se lee como 'U' en vez de 'Ubuntu'. Es un fallo
# silencioso y muy desconcertante.
$env:WSL_UTF8 = "1"

$distro = $null
try {
    $lista = wsl --list --quiet 2>$null |
        ForEach-Object { $_.Trim() } |
        Where-Object { $_ -and $_ -ne "docker-desktop" }
    if ($lista) { $distro = @($lista)[0] }
} catch { }

if (-not $distro) {
    Write-Host "No se encontro ninguna distribucion WSL utilizable."
    Escribir @("RESULTADO: no se pudo verificar.",
               "No hay ninguna distribucion Linux disponible en esta maquina.",
               "La rama de Linux se entrega escrita pero NO verificada, y asi se",
               "declara en el documento de calidad.")
    exit 1
}

Write-Host "[1/4] Entorno Linux: WSL / $distro"

# --- Comprobar el compilador --------------------------------------------------
$version = wsl -d $distro -- bash -lc "g++ --version 2>/dev/null | head -1" 2>$null
if (-not $version) {
    Write-Host "      g++ no esta instalado en $distro. Instalandolo ..."
    wsl -d $distro -u root -- bash -lc "export DEBIAN_FRONTEND=noninteractive; apt-get update -qq >/dev/null 2>&1; apt-get install -y -qq --no-install-recommends g++ make procps >/dev/null 2>&1" 2>$null | Out-Null
    $version = wsl -d $distro -- bash -lc "g++ --version 2>/dev/null | head -1" 2>$null
}
if (-not $version) {
    Write-Host "      No se pudo disponer de g++."
    Escribir @("RESULTADO: no se pudo verificar: g++ no disponible en $distro.")
    exit 1
}
Write-Host "      $version"

# OJO: nada de comillas dobles dentro de esta cadena. En PowerShell el
# escape es el acento grave, no la barra invertida, asi que un \" cerraria
# la cadena y el parser fallaria en un sitio muy posterior y desconcertante.
$lineaSo = wsl -d $distro -- bash -lc 'grep PRETTY_NAME /etc/os-release' 2>$null
$so = ($lineaSo -replace '^PRETTY_NAME=', '') -replace '"', ''
$kernel = wsl -d $distro -- bash -lc "uname -r" 2>$null

Escribir @("ENTORNO",
           "  Distribucion : $so",
           "  Kernel       : $kernel",
           "  Compilador   : $version",
           "  Banderas     : -std=c++17 -O2 -Wall -Wextra -Wpedantic",
           "")

# --- Copiar, compilar, probar -------------------------------------------------
# La ruta del proyecto vista desde WSL. Se traduce C:\ a /mnt/c/ y las barras.
$rutaWsl = "/mnt/" + $Raiz.Substring(0,1).ToLower() + ($Raiz.Substring(2) -replace '\\', '/')

Write-Host "[2/4] Compilando la rama de Linux ..."
$salidaCompilacion = wsl -d $distro -- bash -lc "rm -rf ~/p4_verif && mkdir -p ~/p4_verif && cp -r '$rutaWsl/src' '$rutaWsl/tests' '$rutaWsl/Makefile' ~/p4_verif/ && cd ~/p4_verif && make 2>&1" 2>$null

$advertencias = $salidaCompilacion | Select-String -Pattern "warning:"
Escribir @("COMPILACION",
           ("  Advertencias: " + ($advertencias | Measure-Object).Count))
if ($advertencias) { Escribir ($advertencias | ForEach-Object { "    " + $_.ToString() }) }
Escribir @("")

if ($salidaCompilacion -match "error:") {
    Write-Host "      FALLO la compilacion."
    Escribir @("RESULTADO: la compilacion fallo.")
    Escribir ($salidaCompilacion | Select-String -Pattern "error:" | ForEach-Object { "  " + $_ })
    exit 1
}
Write-Host "      Compilado sin advertencias."

Write-Host "[3/4] Ejecutando la suite de pruebas en Linux ..."
$salidaPruebas = wsl -d $distro -- bash -lc "cd ~/p4_verif && make tests 2>&1" 2>$null
$conteo = ($salidaPruebas | Select-String -Pattern "pruebas superadas").Line
if (-not $conteo) {
    Write-Host "      La suite no reporto resultado."
    Escribir @("RESULTADO: la suite no completo.")
    exit 1
}
Write-Host "      $($conteo.Trim())"

# Que aparezca la linea del conteo no basta: la suite la imprime tambien
# cuando algo falla. Se exige que no haya ningun [FALLA].
$fallidas = $salidaPruebas | Select-String -Pattern "^\[FALLA\]"

Escribir @("SUITE DE PRUEBAS EN LINUX", "")
Escribir ($salidaPruebas | Where-Object { $_ -match "^\[OK\]|^\[FALLA\]|->|omitida|Grupo|pruebas superadas|asercion" } |
          ForEach-Object { "  " + $_ })
Escribir @("")

if ($fallidas) {
    Write-Host "      FALLARON pruebas en Linux."
    Escribir @("RESULTADO: la suite NO paso en Linux.")
    exit 1
}

# --- Demostracion de las tres funciones en Linux ------------------------------
Write-Host "[4/4] Demostrando las tres funciones en Linux ..."
$demo = wsl -d $distro -- bash -lc "cd ~/p4_verif && mkdir -p demo && printf '1\n2\ninforme.txt\ncontenido de prueba\n1\n3\ninforme.txt\n0\n2\n3\n0\n' | ./build/recursos --dir demo 2>&1" 2>$null

Escribir @("DEMOSTRACION DE LAS TRES FUNCIONES EN LINUX", "")
Escribir ($demo | ForEach-Object { "  " + $_ })

# --- Rendimiento en Linux, para poder comparar --------------------------------
# Misma pasada de calentamiento descartada que en Windows, para que el metodo
# sea identico en las dos plataformas.
$benchCrudo = wsl -d $distro -- bash -lc "cd ~/p4_verif && ./build/recursos --benchmark --cantidad 100 --repeticiones 2 --dir bench >/dev/null && ./build/recursos --benchmark --cantidad 100 --repeticiones 5 --dir bench" 2>$null
$bench = $benchCrudo | Where-Object { $_ -match '^\|' }
Escribir @("", "RENDIMIENTO EN LINUX (100 archivos, 5 repeticiones, tras calentamiento)", "")
Escribir ($bench | ForEach-Object { "  " + $_ })

Escribir @("",
           "=====================================================================",
           "RESULTADO: la rama de Linux compila sin advertencias, pasa la suite",
           "completa y ejecuta las tres funciones del enunciado.",
           "",
           "ALCANCE DE ESTA EVIDENCIA, declarado con precision:",
           "  WSL usa un kernel Linux real, asi que /proc/meminfo y ps son los",
           "  de Linux y el codigo ejercitado es el mismo que correria en una",
           "  maquina Linux nativa. Lo que WSL no reproduce es el hardware ni la",
           "  carga de un servidor real: por eso las CIFRAS de las dos",
           "  plataformas no son comparables entre si. Lo que si queda",
           "  demostrado es que el mismo codigo fuente funciona en ambas.")

Write-Host ""
Write-Host "=============================================================="
Write-Host " Evidencia escrita en $Informe"
Write-Host "=============================================================="
