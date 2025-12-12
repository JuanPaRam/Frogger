# Makefile para Frogger con MSYS2 MINGW64

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Directorios
SRC_DIR = src
BIN_DIR = bin

# Nombre del ejecutable
TARGET = $(BIN_DIR)/frogger.exe

# Archivos fuente
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Archivos objeto
OBJECTS = $(SOURCES:.cpp=.o)

# Regla principal
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Compilación exitosa: $(TARGET)"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)
	@echo "Archivos limpiados"

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run