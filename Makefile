# ==============================================================================
# Makefile -- Compilacion con g++ para Linux.
# Administrador Simplificado de Recursos (Proyecto 4)
# Universidad Invenio, TIIT2007 Sistemas Operativos
# Autor: Fabrizio Espinoza Arce
#
# ESTE MAKEFILE SI SE EJECUTA. A diferencia de los Proyectos 2 y 3 --donde la
# rama de Linux se entregaba escrita pero nunca probada, y habia que declararlo
# como limitacion-- aqui se compila y se corre de verdad con g++ en una
# distribucion Linux de WSL2 (Ubuntu). Lo lanza scripts/verificar_linux.ps1 y
# la evidencia queda en results/compatibilidad_linux.txt. Como alternativa,
# Dockerfile.linux hace lo mismo en un contenedor Debian.
#
# Objetivos:
#   make / make all   -> compila build/recursos
#   make tests        -> compila build/test_recursos y EJECUTA la suite
#   make run          -> compila y ejecuta (ARGS="..." para pasar opciones)
#   make clean        -> borra build/
#   make help         -> lista estos objetivos
#
# SELECCION DE PLATAFORMA: se compila src/plataforma/SistemaLinux.cpp y NO
# SistemaWindows.cpp. La eleccion es explicita aqui, igual que build.bat elige
# la contraria, en vez de envolver los ficheros enteros en un #ifdef que
# dejaria una unidad de traduccion vacia.
# ==============================================================================

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
INCLUDES := -Isrc

MKDIR_P ?= mkdir -p
RM_RF   ?= rm -rf

BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj

APP_BIN   := $(BUILD_DIR)/recursos
TESTS_BIN := $(BUILD_DIR)/test_recursos

# Fuentes comunes a las dos plataformas.
CORE_SRCS := $(wildcard src/core/*.cpp)
IO_SRCS   := $(wildcard src/io/*.cpp)

# Fuentes de plataforma: Archivos es portable, Sistema es especifico.
PLAT_SRCS := src/plataforma/Archivos.cpp src/plataforma/SistemaLinux.cpp

CORE_OBJS := $(patsubst src/core/%.cpp,$(OBJ_DIR)/core/%.o,$(CORE_SRCS))
IO_OBJS   := $(patsubst src/io/%.cpp,$(OBJ_DIR)/io/%.o,$(IO_SRCS))
PLAT_OBJS := $(patsubst src/plataforma/%.cpp,$(OBJ_DIR)/plataforma/%.o,$(PLAT_SRCS))
MAIN_OBJ  := $(OBJ_DIR)/main.o
TEST_OBJ  := $(OBJ_DIR)/tests/test_recursos.o

DEPS := $(CORE_OBJS:.o=.d) $(IO_OBJS:.o=.d) $(PLAT_OBJS:.o=.d) $(MAIN_OBJ:.o=.d) $(TEST_OBJ:.o=.d)
-include $(DEPS)

.PHONY: all tests run clean help
.DEFAULT_GOAL := all

all: $(APP_BIN)

$(APP_BIN): $(CORE_OBJS) $(IO_OBJS) $(PLAT_OBJS) $(MAIN_OBJ) | $(BUILD_DIR)
	@echo "=== Enlazando $@ ==="
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "OK: $@ generado."

tests: $(TESTS_BIN)
	@echo "=== Ejecutando $(TESTS_BIN) ==="
	$(TESTS_BIN)

# La suite enlaza el nucleo, la capa de plataforma y los modulos de io/
# probables sin capturar la salida estandar. NO enlaza src/main.cpp, que
# traeria un segundo main. Si esta lista se desincroniza de la de build.bat,
# el enlazado falla con undefined reference.
$(TESTS_BIN): $(CORE_OBJS) $(IO_OBJS) $(PLAT_OBJS) $(TEST_OBJ) | $(BUILD_DIR)
	@echo "=== Enlazando $@ ==="
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "OK: $@ generado."

ARGS ?=
run: $(APP_BIN)
	$(APP_BIN) $(ARGS)

clean:
	$(RM_RF) $(BUILD_DIR)

help:
	@echo "Objetivos disponibles:"
	@echo "  make          compila build/recursos -- objetivo por defecto"
	@echo "  make tests    compila build/test_recursos y ejecuta la suite"
	@echo "  make run      compila y ejecuta. ARGS=\"...\" pasa opciones"
	@echo "  make clean    borra el directorio build/"

$(OBJ_DIR)/core/%.o: src/core/%.cpp | $(OBJ_DIR)/core
	@echo "Compilando $< ..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(OBJ_DIR)/io/%.o: src/io/%.cpp | $(OBJ_DIR)/io
	@echo "Compilando $< ..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(OBJ_DIR)/plataforma/%.o: src/plataforma/%.cpp | $(OBJ_DIR)/plataforma
	@echo "Compilando $< ..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(OBJ_DIR)/main.o: src/main.cpp | $(OBJ_DIR)
	@echo "Compilando $< ..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(OBJ_DIR)/tests/test_recursos.o: tests/test_recursos.cpp | $(OBJ_DIR)/tests
	@echo "Compilando $< ..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

$(BUILD_DIR) $(OBJ_DIR) $(OBJ_DIR)/core $(OBJ_DIR)/io $(OBJ_DIR)/plataforma $(OBJ_DIR)/tests:
	$(MKDIR_P) $@
