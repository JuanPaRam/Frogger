# Makefile para Frogger con MSYS2 MINGW64

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lmingw32 -lSDL2main -lSDL2

# Nombre del ejecutable
TARGET = frogger.exe

# Archivos fuente
SOURCES = main.cpp

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
	rm -f $(OBJECTS) $(TARGET)
	@echo "Archivos limpiados"

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run