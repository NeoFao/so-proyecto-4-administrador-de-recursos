# =============================================================================
# empaquetar_entrega.ps1 -- Deja la entrega lista en "Entregables Proyecto 4".
# Administrador Simplificado de Recursos (Proyecto 4) -- Fabrizio Espinoza Arce
#
# Lo llama la opcion 9 de "EJECUTAR (Todo).bat". Genera:
#   <Entregables>\Proyecto 4 - Administrador de Recursos - Fabrizio Espinoza.zip
#   <Entregables>\Documento IEEE - Proyecto 4 - Fabrizio Espinoza.pdf
#   <Entregables>\Autoevaluacion ISO-IEC 25010 y QA - Proyecto 4.pdf
#
# El ZIP lleva el proyecto sin lo que se regenera o no es entregable: build\,
# node_modules\, la guia del curso, y las carpetas de trabajo de las demos.
# Si la entrega anterior existe, se guarda en _version_anterior_<fecha>\.
# =============================================================================
$ErrorActionPreference = "Stop"
Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem

$Raiz = Split-Path -Parent $PSScriptRoot
$NombreProyecto = Split-Path -Leaf $Raiz
$Entregables = Join-Path (Split-Path -Parent $Raiz) "Entregables Proyecto 4"
if (-not (Test-Path $Entregables)) {
    throw "No existe la carpeta '$Entregables'. Esta opcion solo sirve en el equipo del autor."
}

$zipDestino = Join-Path $Entregables "Proyecto 4 - Administrador de Recursos - Fabrizio Espinoza.zip"
$pdfIeee = Join-Path $Entregables "Documento IEEE - Proyecto 4 - Fabrizio Espinoza.pdf"
$pdfQa = Join-Path $Entregables "Autoevaluacion ISO-IEC 25010 y QA - Proyecto 4.pdf"

# Respaldo de la entrega anterior (una carpeta por dia).
if (Test-Path $zipDestino) {
    $respaldo = Join-Path $Entregables ("_version_anterior_" + (Get-Date -Format "yyyy-MM-dd_HHmm"))
    New-Item -ItemType Directory -Force -Path $respaldo | Out-Null
    foreach ($f in $zipDestino, $pdfIeee, $pdfQa) {
        if (Test-Path $f) { Move-Item -Force $f $respaldo }
    }
    Write-Host "Entrega anterior guardada en: $respaldo"
}

$excluir = @("build", "node_modules", "Documentacion Guia", "trabajo", "demo", "demo_bench",
             "demo_protegido", "__pycache__", ".vs")
$archivos = Get-ChildItem -Path $Raiz -Recurse -File | Where-Object {
    $relativo = $_.FullName.Substring($Raiz.Length + 1)
    -not ($relativo.Split('\') | Where-Object { $excluir -contains $_ })
}

$zip = [System.IO.Compression.ZipFile]::Open($zipDestino, "Create")
try {
    foreach ($a in $archivos) {
        $dentro = $NombreProyecto + "/" + $a.FullName.Substring($Raiz.Length + 1).Replace('\', '/')
        [System.IO.Compression.ZipFileExtensions]::CreateEntryFromFile(
            $zip, $a.FullName, $dentro, [System.IO.Compression.CompressionLevel]::Optimal) | Out-Null
    }
} finally {
    $zip.Dispose()
}

Copy-Item -Force (Join-Path $Raiz "docs\documento_ieee_recursos.pdf") $pdfIeee
Copy-Item -Force (Join-Path $Raiz "docs\QA_Proyecto4.pdf") $pdfQa

Write-Host ""
Write-Host ("ZIP con {0} archivos: {1}" -f $archivos.Count, $zipDestino)
Write-Host "PDF copiados junto al ZIP."
