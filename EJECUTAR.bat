@echo off
REM ===========================================================================
REM EJECUTAR.bat -- Abre el Administrador de Recursos (Proyecto 4).
REM Administrador Simplificado de Recursos -- Fabrizio Espinoza Arce
REM TIIT2007 Sistemas Operativos, Universidad Invenio
REM
REM Via de entrada para USAR la herramienta: un doble clic compila y abre el
REM MENU INTERACTIVO (requisito 4), desde el que se navega a la gestion de
REM archivos, los procesos y la memoria.
REM
REM El directorio de trabajo controlado es la carpeta "trabajo" junto a este
REM script; se crea sola. Otras opciones se pasan tal cual al programa:
REM     EJECUTAR.bat --dir otra_carpeta
REM     EJECUTAR.bat --help
REM
REM Para un recorrido guiado de todos los requisitos, con la suite de pruebas
REM y los resultados experimentales, use "EJECUTAR (Demostraciones).bat".
REM
REM Configura solo el entorno de Visual Studio: funciona con doble clic desde
REM el explorador, sin abrir la "Developer Command Prompt".
REM ===========================================================================
setlocal enabledelayedexpansion
cd /d "%~dp0"
title Administrador de Recursos - Proyecto 4

REM --- Localizar el compilador ------------------------------------------------
where cl.exe >nul 2>&1
if not errorlevel 1 goto :compilar

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
    call "!VCVARS!" >nul 2>&1
    where cl.exe >nul 2>&1
    if not errorlevel 1 goto :compilar
)

REM Sin compilador: si ya hay un ejecutable, se usa; si no, no hay nada que abrir.
if exist "build\recursos.exe" (
    echo AVISO: no se encontro Visual Studio; se abre el ejecutable ya compilado.
    goto :abrir
)
echo.
echo ERROR: no se encontro ningun compilador de C++ ni un ejecutable previo.
echo   Instale Visual Studio con "Desarrollo para el escritorio con C++".
echo.
pause
exit /b 1

REM --- Compilar ---------------------------------------------------------------
REM La salida del compilador va a un registro: en pantalla solo interesa el
REM menu. Si la compilacion falla, se muestra entera para poder diagnosticar.
:compilar
echo Compilando el Administrador de Recursos ...
if not exist build mkdir build
call "%~dp0build.bat" > "build\compilacion.log" 2>&1
if errorlevel 1 (
    type "build\compilacion.log"
    echo.
    echo ERROR: la compilacion fallo. Detalle arriba y en build\compilacion.log
    pause
    exit /b 1
)
cls

REM --- Abrir el menu interactivo ----------------------------------------------
:abrir
if "%~1"=="" (
    "build\recursos.exe" --dir trabajo
) else (
    "build\recursos.exe" %*
)

echo.
pause
