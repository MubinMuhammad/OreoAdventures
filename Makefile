COMPILER := g++ -std=c++17
INCLUDE_DIR := -IcstmEngine/ -IcstmEngine/glad/include
LIBS := -lglfw -lGL -L./glm/ -lglm
SRC_FILES := main.cpp cstmEngine/glad/src/glad.c texture.cpp
EXE := MarioAdventures

all:
	$(COMPILER) rle.cpp -o rle
	./rle ./levels/level1.txt ./levels/level2.txt ./levels/level3.txt
	$(COMPILER) $(SRC_FILES) -o $(EXE) $(INCLUDE_DIR) $(LIBS)

run:
	./MarioAdventures

clean:
	rm -fr ./MarioAdventures ./rle
