# =============================================================================
# generar_documentos.ps1 -- Markdown -> Word -> PDF de los dos entregables.
# Administrador Simplificado de Recursos (Proyecto 4) -- Fabrizio Espinoza Arce
#
# Lo llama la opcion 6 de "EJECUTAR (Todo).bat". Hace, en orden:
#   1. verificar_documento.py sobre el IEEE (citas, tablas, figuras, anos);
#   2. md2docx.js: IEEE a dos columnas y QA a una columna;
#   3. Word exporta los dos .docx a PDF.
# Requiere Python, Node.js con el paquete docx (node_modules del proyecto) y Word.
# =============================================================================
$ErrorActionPreference = "Stop"
$Raiz = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
Set-Location $Raiz
$Docs = Join-Path $Raiz "docs"

Write-Host "[1/3] Verificando la coherencia del documento IEEE ..."
& python "scripts\verificar_documento.py" "docs\documento_ieee_recursos.md" | Select-Object -Last 3
if ($LASTEXITCODE -ne 0) { throw "El documento IEEE tiene problemas: revise el detalle de arriba." }

Write-Host "[2/3] Generando los .docx ..."
& node "scripts\docs\md2docx.js" "docs\documento_ieee_recursos.md" "docs\documento_ieee_recursos.docx" ieee
if ($LASTEXITCODE -ne 0) { throw "Fallo la generacion del IEEE." }
& node "scripts\docs\md2docx.js" "docs\QA_Proyecto4.md" "docs\QA_Proyecto4.docx" qa
if ($LASTEXITCODE -ne 0) { throw "Fallo la generacion del QA." }

Write-Host "[3/3] Exportando a PDF con Word ..."
# La exportacion va por exportar_pdf.py (pywin32) y no por COM desde
# PowerShell: llamando a Word desde este script, la exportacion se quedaba
# colgada indefinidamente en ExportAsFixedFormat. Es el mismo exportador que
# usa el Portafolio, que si funciona en este equipo.
# Si el PDF esta abierto en un visor, Word no puede sobrescribirlo: cierrelo.
foreach ($nombre in "documento_ieee_recursos", "QA_Proyecto4") {
    & python "scripts\docs\exportar_pdf.py" (Join-Path $Docs "$nombre.docx") (Join-Path $Docs "$nombre.pdf")
    if ($LASTEXITCODE -ne 0) { throw "Fallo la exportacion de $nombre a PDF." }
}
Write-Host ""
Write-Host "Listo: docs\documento_ieee_recursos.pdf y docs\QA_Proyecto4.pdf"
