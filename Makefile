# Nombre del ejecutable final de tu proyecto
TARGET = DarkerGraph

# Directorio donde se construirán los objetos de tu proyecto
BUILD_DIR = build

# Directorios de código fuente de tu proyecto
SRC_DIRS = lib/src

# Directorios de archivos de encabezado de tu proyecto
INC_DIRS = lib/include external/DeSIGNAR-2.0.0/include

# Directorio de la biblioteca DeSIGNAR (después de la compilación)
DESIGNAR_LIB_DIR = external/DeSIGNAR-2.0.0/lib

# Nombre de la biblioteca DeSIGNAR (estática, según el README)
DESIGNAR_LIB_NAME = Designar

# Flags del compilador para tu proyecto (añade las que necesites)
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I$(INC_DIRS)

# Flags del enlazador para tu proyecto (añade las que necesites)
LDFLAGS = -L$(DESIGNAR_LIB_DIR) -l$(DESIGNAR_LIB_NAME)

# Compilador a usar
CXX = g++

# Obtener la lista de archivos fuente .cpp de tu proyecto
SOURCES = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))

# Obtener la lista de archivos objeto .o basados en los archivos fuente de tu proyecto
OBJECTS = $(patsubst $(SRC_DIRS)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Regla principal para construir el ejecutable de tu proyecto EN LA CARPETA PRINCIPAL
$(TARGET): $(OBJECTS) $(DESIGNAR_LIB_DIR)/lib$(DESIGNAR_LIB_NAME).a
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

# Regla para compilar archivos .cpp de tu proyecto a .o
$(BUILD_DIR)/%.o: $(SRC_DIRS)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para construir la biblioteca DeSIGNAR
$(DESIGNAR_LIB_DIR)/lib$(DESIGNAR_LIB_NAME).a: external/DeSIGNAR-2.0.0/CMakeLists.txt
	@echo "Construyendo la biblioteca DeSIGNAR..."
	@mkdir -p external/DeSIGNAR-2.0.0/build
	cd external/DeSIGNAR-2.0.0/build && cmake ..
	$(MAKE) -C external/DeSIGNAR-2.0.0/build

# Regla para limpiar los archivos de construcción de tu proyecto y DeSIGNAR
clean:
	rm -rf $(BUILD_DIR) external/DeSIGNAR-2.0.0/build $(TARGET)

.PHONY: all clean

all: $(TARGET)