@echo off
REM ============================================================================
REM build.bat -- Compilacion con MSVC (cl.exe) para Windows.
REM Administrador Simplificado de Recursos (Proyecto 4)
REM Universidad Invenio, TIIT2007 Sistemas Operativos
REM Autor: Fabrizio Espinoza Arce
REM
REM Uso:
REM   build.bat            compila build\recursos.exe
REM   build.bat tests      compila build\test_recursos.exe y ejecuta la suite
REM   build.bat sinopt     compila con /Od (una de las configuraciones
REM                        verificadas: comprueba que el resultado no depende
REM                        del optimizador)
REM
REM Requiere una consola con cl.exe en el PATH. Si no la tiene, use
REM EJECUTAR.bat, que configura el entorno de Visual Studio por su cuenta.
REM
REM SELECCION DE PLATAFORMA: se compila src\plataforma\SistemaWindows.cpp y NO
REM SistemaLinux.cpp. La eleccion es explicita aqui, y no con un #ifdef que
REM envuelva el fichero entero, para que ninguna unidad de traduccion quede
REM vacia: MSVC avisa con LNK4221 sobre objetos sin simbolos publicos, y este
REM proyecto exige cero advertencias.
REM ============================================================================

setlocal
cd /d "%~dp0"

where cl.exe >nul 2>&1
if errorlevel 1 (
    echo.
    echo ERROR: no se encontro cl.exe en el PATH.
    echo.
    echo   Opcion 1: ejecute EJECUTAR.bat, que configura Visual Studio solo.
    echo   Opcion 2: abra "x64 Native Tools Command Prompt for VS" y repita.
    echo.
    exit /b 1
)

REM cl.exe NO crea el directorio de /Fo: si no existe, falla con un C1083 que
REM senala al .cpp y no al directorio, y despista. Se crean los tres aqui.
if not exist build          mkdir build
if not exist build\obj      mkdir build\obj
if not exist build\obj_test mkdir build\obj_test
if not exist build\obj_od   mkdir build\obj_od

REM /std:c++17  estandar del proyecto (hace falta para <filesystem>)
REM /W4         advertencias altas -- cero advertencias es condicion de entrega
REM /EHsc       modelo estandar de excepciones
REM /O2         optimizacion
REM /nologo     sin banner
REM /Isrc       encabezados del proyecto
set FLAGS=/std:c++17 /W4 /EHsc /O2 /nologo /Isrc
set FUENTES=src\core\*.cpp src\io\*.cpp src\plataforma\Archivos.cpp src\plataforma\SistemaWindows.cpp

set ARG1=%~1
if /I "%ARG1%"=="tests"  goto :build_tests
if /I "%ARG1%"=="sinopt" goto :build_sinopt

echo Compilando build\recursos.exe ...
cl.exe %FLAGS% %FUENTES% src\main.cpp /Fobuild\obj\ /Febuild\recursos.exe
if errorlevel 1 (
    echo.
    echo ERROR: la compilacion fallo. Revise los mensajes de arriba.
    exit /b 1
)
echo.
echo OK: build\recursos.exe generado.
exit /b 0

:build_sinopt
echo Compilando build\recursos_od.exe SIN optimizar (/Od) ...
cl.exe /std:c++17 /W4 /EHsc /Od /nologo /Isrc %FUENTES% src\main.cpp /Fobuild\obj_od\ /Febuild\recursos_od.exe
if errorlevel 1 (
    echo.
    echo ERROR: la compilacion sin optimizar fallo.
    exit /b 1
)
echo.
echo OK: build\recursos_od.exe generado.
exit /b 0

:build_tests
echo Compilando build\test_recursos.exe ...
echo Enlaza el nucleo, la capa de plataforma y los modulos de src\io\ probables
echo sin capturar la salida estandar; NO enlaza src\main.cpp, que traeria un
echo segundo main. Si esta lista se desincroniza de la del Makefile, el
echo enlazado falla con LNK2019.
cl.exe %FLAGS% tests\test_recursos.cpp %FUENTES% /Fobuild\obj_test\ /Febuild\test_recursos.exe
if errorlevel 1 (
    echo.
    echo ERROR: la compilacion de la suite de pruebas fallo.
    exit /b 1
)
echo.
echo OK: build\test_recursos.exe generado.
echo.
build\test_recursos.exe
set TEST_EXIT=%ERRORLEVEL%
echo.
if %TEST_EXIT% EQU 0 (
    echo OK: la suite completa paso.
) else (
    echo FALLA: revise arriba que pruebas no pasaron.
)
exit /b %TEST_EXIT%
