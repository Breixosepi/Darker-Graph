# Directorio donde se construirán los ejecutables de prueba
TEST_BIN_DIR = test/bin

# Direcciones
DSG = external/DeSiGNAR-2.0.0
BUILD_DIR = build
LIB_SRC_DIR = lib/src

MAIN_SRC = main.cpp
MAIN_TARGET = DarkerGraph  # El ejecutable estará en la raíz

# Directorio de archivos fuente de prueba
TEST_SRC_DIR = test/src
TEST_SOURCES = $(wildcard $(addsuffix /*.cpp, $(TEST_SRC_DIR)))
TEST_TARGETS = $(patsubst $(TEST_SRC_DIR)/%.cpp, $(TEST_BIN_DIR)/%, $(TEST_SOURCES))

# Archivos fuente de lib
LIB_SOURCES = $(wildcard $(addsuffix /*.cpp, $(LIB_SRC_DIR)))
LIB_OBJECTS = $(LIB_SOURCES:$(LIB_SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Directorios de archivos de encabezado
INC_DIRS = lib/include $(DSG)/include

# Directorio de la biblioteca DeSiGNAR (después de la compilación)
DESIGNAR_LIB_DIR = $(DSG)/build
DESIGNAR_LIB_NAME = Designar

# Flags del compilador
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Iexternal/DeSiGNAR-2.0.0/include -Ilib/include -Wcast-align -Wno-sign-compare -Wno-write-strings -Wno-parentheses -Wfloat-equal -pedantic

# Flags del enlazador
LDFLAGS = -L$(DESIGNAR_LIB_DIR)

# Compilador a usar
CXX = g++

# Regla principal para construir todos los ejecutables
all: $(MAIN_TARGET) $(TEST_TARGETS)

# Regla para construir el ejecutable del archivo fuente principal (en la raíz)
$(MAIN_TARGET): $(BUILD_DIR)/$(notdir $(MAIN_SRC:.cpp=.o)) $(LIB_OBJECTS) $(DESIGNAR_LIB_DIR)/lib$(DESIGNAR_LIB_NAME).a
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(LDFLAGS) -o $@ $(BUILD_DIR)/$(notdir $(MAIN_SRC:.cpp=.o)) $(LIB_OBJECTS) -l$(DESIGNAR_LIB_NAME)

# Regla para construir el objeto del archivo fuente principal
$(BUILD_DIR)/$(notdir $(MAIN_SRC:.cpp=.o)): $(MAIN_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para construir los objetos de la librería
$(BUILD_DIR)/%.o: $(LIB_SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_BIN_DIR)/%: $(TEST_SRC_DIR)/%.cpp $(DESIGNAR_LIB_DIR)/lib$(DESIGNAR_LIB_NAME).a
	@mkdir -p $(TEST_BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/$(basename $(notdir $<)).o
	$(CXX) $(LDFLAGS) -o $@ $(BUILD_DIR)/$(basename $(notdir $@)).o -l$(DESIGNAR_LIB_NAME)

# Regla para construir la biblioteca DeSIGNAR (si es necesario)
$(DESIGNAR_LIB_DIR)/lib$(DESIGNAR_LIB_NAME).a: $(DSG)/CMakeLists.txt
	@echo "Construyendo la biblioteca DeSIGNAR..."
	@mkdir -p $(DSG)/build
	cd $(DSG)/build && cmake ..
	$(MAKE) -C $(DSG)/build

# Regla para limpiar todos los ejecutables
clean:
	rm -rf $(MAIN_TARGET) $(BUILD_DIR)
	find $(TEST_BIN_DIR) -type f -not -name ".empty" -delete

.PHONY: all clean