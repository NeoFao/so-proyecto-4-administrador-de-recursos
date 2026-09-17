#!/usr/bin/env bash
# =============================================================================
# verificar_linux_interno.sh -- Lo que se ejecuta DENTRO del contenedor de
# Dockerfile.linux. Es la alternativa a WSL de scripts/verificar_linux.ps1.
#
# Administrador Simplificado de Recursos (Proyecto 4)
# Universidad Invenio, TIIT2007 Sistemas Operativos
# Autor: Fabrizio Espinoza Arce
#
# Uso (desde la raiz del proyecto, en una maquina con Docker):
#   docker build -f Dockerfile.linux -t p4-linux .
#   docker run --rm -v "$PWD:/p4" p4-linux
#
# Compila en una copia (/tmp/p4) y no sobre /p4: los objetos de g++ y los de
# MSVC comparten el nombre build/, y compilar in situ pisaria los de Windows.
#
# NOTA: un contenedor corre como root, y root ignora los bits de permiso. Por
# eso la prueba 3.4 (permisos insuficientes) se declara omitida aqui; en WSL,
# que corre como usuario normal, si se ejecuta.
# =============================================================================
set -euo pipefail

rm -rf /tmp/p4
mkdir -p /tmp/p4
cp -r /p4/src /p4/tests /p4/Makefile /tmp/p4/
cd /tmp/p4

echo "=== Entorno ==="
grep PRETTY_NAME /etc/os-release || true
uname -r
g++ --version | head -1
echo

echo "=== Compilacion (se exige cero advertencias) ==="
salida="$(make 2>&1)"
echo "$salida"
if echo "$salida" | grep -q "warning:"; then
    echo "FALLA: hay advertencias de compilacion"
    exit 1
fi
echo

echo "=== Suite de pruebas ==="
make tests
echo

echo "=== Demostracion de las tres funciones ==="
./build/recursos --procesos --dir /tmp/p4/demo
./build/recursos --memoria --dir /tmp/p4/demo
printf '1\n2\ninforme.txt\ncontenido de prueba\n1\n3\ninforme.txt\n0\n0\n' |
    ./build/recursos --dir /tmp/p4/demo
