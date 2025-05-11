# Configuración básica
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Ilib/include -Iexternal/DeSiGNAR-2.0.0/include
LDFLAGS = -Lexternal/DeSiGNAR-2.0.0/build -lDesignar

# Directorios
BUILD_DIR = build
LIB_SRC_DIR = lib/src
TEST_BIN_DIR = test/bin
TEST_SRC_DIR = test/src
DSG = external/DeSiGNAR-2.0.0

# Directorio de la biblioteca DeSiGNAR (después de la compilación)
DESIGNAR_LIB_DIR = $(DSG)/build
DESIGNAR_LIB_NAME = Designar

# Archivos fuente
MAIN_SRC = main.cpp
MAIN_TARGET = DarkerGraph
LIB_SOURCES = $(wildcard $(LIB_SRC_DIR)/*.cpp)
LIB_OBJECTS = $(patsubst $(LIB_SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(LIB_SOURCES))
TEST_SOURCES = $(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_TARGETS = $(patsubst $(TEST_SRC_DIR)/%.cpp,$(TEST_BIN_DIR)/%,$(TEST_SOURCES))

# Detección de SDL2
SDL_CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
SDL_LIBS := $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf)

# Flags completas
CXXFLAGS += $(SDL_CFLAGS)
LDFLAGS += $(SDL_LIBS)

# Reglas principales
all: $(MAIN_TARGET) $(TEST_TARGETS)

$(MAIN_TARGET): $(BUILD_DIR)/$(notdir $(MAIN_SRC:.cpp=.o)) $(LIB_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(LIB_SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_BIN_DIR)/%: $(TEST_SRC_DIR)/%.cpp $(LIB_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/$(notdir $(<:.cpp=.o))
	$(CXX) -o $@ $(BUILD_DIR)/$(notdir $(<:.cpp=.o)) $(LIB_OBJECTS) $(LDFLAGS)

# Construir biblioteca DeSiGNAR
$(DESIGNAR_LIB_DIR)/lib$(DESIGNAR_LIB_NAME).a:
	@echo "Construyendo la biblioteca DeSIGNAR..."
	@mkdir -p $(DSG)/build
	cd $(DSG)/build && cmake ..
	$(MAKE) -C $(DSG)/build

clean:
	rm -rf $(MAIN_TARGET) $(BUILD_DIR)
	find $(TEST_BIN_DIR) -type f -not -name ".empty" -delete

.PHONY: all clean