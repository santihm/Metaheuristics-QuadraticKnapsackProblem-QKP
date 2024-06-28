# Directorios
SRC_DIR = src
BIN_DIR = bin

# Archivos fuente
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# Nombre del ejecutable
EXECUTABLE = QKP

# Compilador y opciones
CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2

# Regla principal
all: $(BIN_DIR)/$(EXECUTABLE)

# Compilación y enlazado
$(BIN_DIR)/$(EXECUTABLE): $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Limpieza

