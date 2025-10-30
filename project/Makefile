CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -g
INCLUDES = -Iinclude
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
OUT = bin/proyecto_os

all: build run

build: $(OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(OUT)

run:
	@echo "Ejecuta el programa con ./bin/proyecto_os [opciones]"

clean:
	rm -rf src/*.o bin

.PHONY: all build run clean