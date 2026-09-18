@echo off
REM ===========================================================================
REM EJECUTAR (Todo).bat -- Panel del Administrador de Recursos (Proyecto 4).
REM Fabrizio Espinoza Arce -- TIIT2007 Sistemas Operativos, Universidad Invenio
REM
REM Doble clic y elegir un numero: no hace falta recordar ningun comando.
REM Solo tareas de la herramienta: usarla, demostrarla, probarla y ver sus
REM resultados. Lo que tiene que ver con la ENTREGA --regenerar los documentos,
REM empaquetar el ZIP-- vive en scripts\ y no en este panel.
REM ===========================================================================
setlocal
cd /d "%~dp0"
title Proyecto 4 - Administrador de Recursos

:menu
cls
echo.
echo  ================================================================
echo    ADMINISTRADOR DE RECURSOS - Proyecto 4
echo  ================================================================
echo.
echo     1  Abrir el Administrador de Recursos (menu interactivo)
echo     2  Demostracion guiada de todos los requisitos
echo     3  Correr las 30 pruebas
echo     4  Ver los resultados experimentales (carpeta results)
echo.
echo     0  Salir
echo.
choice /c 12340 /n /m "  Elija una opcion: "
set "OPCION=%ERRORLEVEL%"
echo.

if "%OPCION%"=="1" goto :abrir
if "%OPCION%"=="2" goto :demostracion
if "%OPCION%"=="3" goto :pruebas
if "%OPCION%"=="4" goto :resultados
exit /b 0

:abrir
REM Se abre en una ventana propia: al salir del programa se vuelve aqui.
start "Administrador de Recursos" /wait cmd /c ""%~dp0EJECUTAR.bat""
goto :menu

:demostracion
start "Demostracion - Proyecto 4" /wait cmd /c ""%~dp0EJECUTAR (Demostraciones).bat""
goto :menu

:pruebas
call "%~dp0scripts\entorno_vs.bat" || goto :fin_con_pausa
call "%~dp0build.bat" tests
goto :fin_con_pausa

:resultados
start "" explorer "%~dp0results"
goto :menu

:fin_con_pausa
echo.
echo  Pulse una tecla para volver al menu.
pause >nul
goto :menu
