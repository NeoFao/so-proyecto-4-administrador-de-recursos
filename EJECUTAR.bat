@echo off
REM ===========================================================================
REM EJECUTAR.bat -- Demostracion completa del Proyecto 4.
REM Administrador Simplificado de Recursos -- Fabrizio Espinoza Arce
REM TIIT2007 Sistemas Operativos, Universidad Invenio
REM
REM Via de entrada recomendada para el evaluador: un doble clic compila,
REM ejecuta la suite de pruebas y recorre UNO POR UNO los cuatro requisitos
REM funcionales, los dos no funcionales y los tres resultados experimentales,
REM diciendo en cada paso cual se esta demostrando.
REM
REM CONFIGURA SOLO el entorno de Visual Studio: funciona con doble clic desde
REM el explorador, sin abrir la "Developer Command Prompt".
REM ===========================================================================
setlocal enabledelayedexpansion
cd /d "%~dp0"

echo.
echo ================================================================
echo   ADMINISTRADOR SIMPLIFICADO DE RECURSOS - Proyecto 4
echo   Fabrizio Espinoza Arce - TIIT2007 Sistemas Operativos
echo ================================================================
echo.
echo   Este script demuestra los 4 requisitos funcionales, los 2 no
echo   funcionales y los 3 resultados experimentales, uno por uno.
echo.

REM --- Paso 1: localizar un compilador ---------------------------------------
where cl.exe >nul 2>&1
if not errorlevel 1 (
    echo [1/8] cl.exe ya esta en el PATH.
    goto :compilar
)

echo [1/8] Buscando Visual Studio ...
set "VCVARS="
for %%D in (18 17 16) do (
    for %%C in (Community Professional Enterprise BuildTools) do (
        if not defined VCVARS (
            if exist "%ProgramFiles%\Microsoft Visual Studio\%%D\%%C\VC\Auxiliary\Build\vcvars64.bat" (
                set "VCVARS=%ProgramFiles%\Microsoft Visual Studio\%%D\%%C\VC\Auxiliary\Build\vcvars64.bat"
            )
        )
    )
)

if defined VCVARS (
    echo       Encontrado: !VCVARS!
    REM vcvars64.bat imprime un aviso sobre vswhere.exe que es inofensivo.
    call "!VCVARS!" >nul 2>&1
    where cl.exe >nul 2>&1
    if not errorlevel 1 goto :compilar
)

echo.
echo ERROR: no se encontro ningun compilador de C++.
echo   Instale Visual Studio con "Desarrollo para el escritorio con C++".
echo.
pause
exit /b 1

:compilar
echo [2/8] Compilando ...
call "%~dp0build.bat"
if errorlevel 1 ( pause & exit /b 1 )

echo.
echo [3/8] Ejecutando la suite de pruebas ...
echo       (26 pruebas: archivos, frontera del directorio, contratos de
echo        error, memoria, procesos y utilidades)
echo.
call "%~dp0build.bat" tests
if errorlevel 1 ( pause & exit /b 1 )

echo.
echo ================================================================
echo   DEMOSTRACION 1 de 5
echo   Requisitos 1 y 4: gestion de archivos desde el MENU
echo ================================================================
echo.
echo   RF-4  Menu de consola que navega entre las tres funciones
echo   RF-1  Crear, listar, ver metadatos y eliminar archivos dentro
echo         de un directorio de trabajo controlado
echo.
echo   Se conduce el menu automaticamente: crear un archivo, listarlo,
echo   ver sus metadatos y eliminarlo.
echo.
pause
echo.
(
echo 1
echo 2
echo informe.txt
echo contenido de demostracion
echo 1
echo 3
echo informe.txt
echo 4
echo informe.txt
echo s
echo 1
echo 0
echo 0
) | build\recursos.exe --dir demo

echo.
echo ================================================================
echo   DEMOSTRACION 2 de 5
echo   Requisito 2: monitoreo de procesos
echo ================================================================
echo.
echo   RF-2  Listar procesos activos con PID, nombre y consumo
echo         aproximado de recursos. Se usa el comando del sistema,
echo         que es una de las dos vias que permite el enunciado.
echo.
pause
echo.
build\recursos.exe --procesos --dir demo

echo.
echo ================================================================
echo   DEMOSTRACION 3 de 5
echo   Requisito 3: monitoreo de memoria
echo ================================================================
echo.
echo   RF-3  Memoria total, usada y disponible del sistema anfitrion.
echo.
pause
echo.
build\recursos.exe --memoria --dir demo

echo.
echo ================================================================
echo   DEMOSTRACION 4 de 5
echo   Requisito no funcional 1: manejo robusto de errores
echo ================================================================
echo.
echo   RNF-1  Se provocan los errores de verdad y se muestra que la
echo          herramienta NO se cae: informa y vuelve al menu.
echo.
echo     a^) ARCHIVO INEXISTENTE: pedir metadatos de un archivo que no existe
echo     b^) intentar escribir FUERA del directorio controlado
echo     c^) intentar eliminar el propio directorio de trabajo (".")
echo     d^) PERMISOS INSUFICIENTES: crear un archivo en una carpeta a la
echo        que se le quito el permiso de escritura con icacls
echo.
echo     El tercer caso del enunciado, COMANDO NO DISPONIBLE, es la
echo     prueba 5.4 de la suite que acaba de pasar arriba.
echo.
pause
echo.
(
echo 1
echo 3
echo no_existe.txt
echo 2
echo ../../intruso.txt
echo x
echo 4
echo .
echo s
echo 0
echo 0
) | build\recursos.exe --dir demo

echo.
echo   --- d^) permisos insuficientes ---
if not exist demo_protegido mkdir demo_protegido
REM Se deniega escribir al grupo "Todos" por su SID, que no cambia con el
REM idioma de Windows, y se restaura justo despues.
icacls demo_protegido /deny *S-1-1-0:(W,AD) >nul
(
echo 1
echo 2
echo nuevo.txt
echo x
echo 0
echo 0
) | build\recursos.exe --dir demo_protegido
icacls demo_protegido /remove:d *S-1-1-0 >nul

echo.
echo ================================================================
echo   DEMOSTRACION 5 de 5
echo   Resultados experimentales RE-1 y RE-3
echo ================================================================
echo.
echo   RE-1  Tiempo de las operaciones de archivo con 10, 100 y 1000
echo         archivos, que son las cantidades del Dataset 4 del curso.
echo         Media y desviacion sobre 5 repeticiones.
echo   RE-3  Consumo de recursos de la propia herramienta.
echo.
echo   (RE-2, el comportamiento bajo carga, tarda unos dos minutos y
echo    esta en results\prueba_carga.txt)
echo.
pause
echo.
build\recursos.exe --benchmark --cantidad 10   --repeticiones 5 --dir demo_bench
build\recursos.exe --benchmark --cantidad 100  --repeticiones 5 --dir demo_bench
build\recursos.exe --benchmark --cantidad 1000 --repeticiones 5 --dir demo_bench
build\recursos.exe --autoconsumo --dir demo

echo.
echo ================================================================
echo   RESUMEN
echo ================================================================
echo.
echo   Los 4 requisitos funcionales, el manejo de errores y los
echo   resultados experimentales quedan demostrados arriba.
echo.
echo   DONDE ESTA TODO LO DEMAS
echo.
echo     docs\documento_ieee_recursos.pdf   Documento IEEE
echo     docs\QA_Proyecto4.pdf              Autoevaluacion ISO/IEC 25010
echo     ENTREGABLES.md                     Indice: que requisito esta donde
echo     results\                           Evidencia experimental completa
echo     results\compatibilidad_linux.txt   La MISMA herramienta en Linux
echo.
echo   PARA REGENERAR TODA LA EVIDENCIA:
echo     powershell -ExecutionPolicy Bypass -File scripts\correr_experimentos.ps1
echo     powershell -ExecutionPolicy Bypass -File scripts\verificar_linux.ps1
echo.
echo   OTRAS OPCIONES:
echo     build\recursos.exe --help
echo     build\recursos.exe            (menu interactivo)
echo.
echo ================================================================
echo.
pause
