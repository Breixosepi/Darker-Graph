# Directorio donde se construirán los ejecutables de prueba
TEST_BIN_DIR = test/bin
BUILD_DIR = build  # Directorio temporal de objetos (opcional ahora para los tests)

# Archivo fuente principal y su ejecutable (en la carpeta principal)
MAIN_SRC = main.cpp
MAIN_TARGET = DarkerGraph  # El ejecutable estará en la raíz

# Directorio de archivos fuente de prueba
TEST_SRC_DIR = test/src
TEST_SOURCES = $(wildcard $(addsuffix /*.cpp, $(TEST_SRC_DIR)))
TEST_TARGETS = $(patsubst $(TEST_SRC_DIR)/%.cpp, $(TEST_BIN_DIR)/%, $(TEST_SOURCES:.cpp=))

# Directorios de archivos de encabezado
INC_DIRS = lib/include external/DeSIGNAR-2.0.0/include

# Directorio de la biblioteca DeSIGNAR (después de la compilación)
DESIGNAR_LIB_DIR = external/DeSIGNAR-2.0.0/lib

# Nombre de la biblioteca DeSIGNAR
DESIGNAR_LIB_NAME = Designar

# Flags del compilador
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I$(INC_DIRS)

# Flags del enlazador
LDFLAGS = -L$(DESIGNAR_LIB_DIR)

# Compilador a usar
CXX = g++

# Regla principal para construir todos los ejecutables
all: $(MAIN_TARGET) $(TEST_TARGETS)

# Regla para construir el ejecutable del archivo fuente principal (en la raíz)
$(MAIN_TARGET): $(MAIN_SRC) $(DESIGNAR_LIB_DIR)/lib$(DESIGNAR_LIB_NAME).a
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< -l$(DESIGNAR_LIB_NAME)

# Regla para construir los ejecutables de los archivos fuente de prueba (en test/bin/)
$(TEST_BIN_DIR)/%: $(TEST_SRC_DIR)/%.cpp $(DESIGNAR_LIB_DIR)/lib$(DESIGNAR_LIB_NAME).a
	@mkdir -p $(TEST_BIN_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< -l$(DESIGNAR_LIB_NAME)

# Regla para construir la biblioteca DeSIGNAR (si es necesario)
$(DESIGNAR_LIB_DIR)/lib$(DESIGNAR_LIB_NAME).a: external/DeSIGNAR-2.0.0/CMakeLists.txt
	@echo "Construyendo la biblioteca DeSIGNAR..."
	@mkdir -p external/DeSIGNAR-2.0.0/build
	cd external/DeSIGNAR-2.0.0/build && cmake ..
	$(MAKE) -C external/DeSIGNAR-2.0.0/build

# Regla para limpiar todos los ejecutables y el directorio de construcción
clean:
	rm -rf $(TEST_BIN_DIR) $(MAIN_TARGET)

.PHONY: all clean