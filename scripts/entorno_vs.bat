@echo off
REM ===========================================================================
REM entorno_vs.bat -- Deja cl.exe (Visual Studio) listo en la consola actual.
REM
REM Lo usan los menus "EJECUTAR (...).bat". Se llama con CALL y, a proposito,
REM SIN setlocal: las variables de Visual Studio deben quedar en la consola que
REM lo llamo. Devuelve errorlevel 1 si no encuentra ningun compilador.
REM ===========================================================================
where cl.exe >nul 2>&1
if not errorlevel 1 exit /b 0

set "ENTORNO_VCVARS="
for %%D in (18 17 16) do (
    for %%C in (Community Professional Enterprise BuildTools) do (
        if not defined ENTORNO_VCVARS (
            if exist "%ProgramFiles%\Microsoft Visual Studio\%%D\%%C\VC\Auxiliary\Build\vcvars64.bat" (
                set "ENTORNO_VCVARS=%ProgramFiles%\Microsoft Visual Studio\%%D\%%C\VC\Auxiliary\Build\vcvars64.bat"
            )
        )
    )
)
if not defined ENTORNO_VCVARS (
    echo ERROR: no se encontro Visual Studio con "Desarrollo para el escritorio con C++".
    exit /b 1
)
call "%ENTORNO_VCVARS%" >nul 2>&1
where cl.exe >nul 2>&1
if errorlevel 1 (
    echo ERROR: se encontro Visual Studio pero cl.exe no quedo disponible.
    exit /b 1
)
exit /b 0
