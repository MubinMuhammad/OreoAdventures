COMPILER := g++ -std=c++17
INCLUDE_DIR := -IcstmEngine/ -IcstmEngine/glad/include
LIBS := -lglfw -lGL -L./glm/ -lglm
SRC_FILES := src/main.cpp \
						 src/texture.cpp \
						 src/player.cpp \
						 src/levels.cpp \
						 gameEngine/physics.cpp \
						 gameEngine/time.cpp \
						 gameEngine/texture.cpp \
						 gameEngine/font.cpp \
						 cstmEngine/glad/src/glad.c
EXE := MarioAdventures

all:
	$(COMPILER) src/rle.cpp -o rle
	./rle ./levels/level1.txt ./levels/level2.txt ./levels/level3.txt
	mv levelsRle.hpp src/
	$(COMPILER) $(SRC_FILES) -o $(EXE) $(INCLUDE_DIR) $(LIBS)

run: all
	./MarioAdventures

clean:
	rm -fr ./rle
	rm -fr ./MarioAdventures
