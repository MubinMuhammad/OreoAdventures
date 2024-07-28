COMPILER := g++ -std=c++17
INCLUDE_DIR := -IcstmEngine/ -IcstmEngine/glad/include
LIBS := -lglfw -lGL -L./glm/ -lglm
SRC_FILES := src/main.cpp \
	src/texture.cpp \
	src/player.cpp \
	src/levels.cpp \
	src/utils.cpp \
	gameEngine/physics.cpp \
	gameEngine/texture.cpp \
	gameEngine/font.cpp \
	cstmEngine/glad/src/glad.c
EXE := MarioAdventures

all:
	$(COMPILER) $(SRC_FILES) -o $(EXE) $(INCLUDE_DIR) $(LIBS)

run: all
	./MarioAdventures

clean:
	rm -fr ./MarioAdventures
