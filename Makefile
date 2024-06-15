COMPILER := g++
INCLUDE_DIR := -IcstmEngine/ -IcstmEngine/glad/include
LIBS := -lglfw -lGL -L./glm/ -lglm
SRC_FILES := main.cpp cstmEngine/glad/src/glad.c
EXE := MarioAdventures

all:
	$(COMPILER) $(SRC_FILES) -o $(EXE) $(INCLUDE_DIR) $(LIBS)

run:
	./MarioAdventures
