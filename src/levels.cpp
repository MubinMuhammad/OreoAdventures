#include "levels.hpp"
#include "texture.hpp"

#include <fstream>
#include <string>
#include <ctime>

std::vector<std::vector<game::BlockType>>
game::levelGet2DGrid(const char *level_path) {
  srand((unsigned int)time(NULL));

  std::ifstream ifile(level_path);
  std::string line;
  std::vector<std::vector<game::BlockType>> output;

  while (std::getline(ifile, line)) {
    std::vector<game::BlockType> tmp;

    for (char c : line) {
      switch (c) {
        case 'G': tmp.push_back(game::SQR_GRASS); break;
        case 'd': tmp.push_back(game::SQR_DIRT); break;
        case 'T': tmp.push_back(
          static_cast<game::BlockType>
          (41 + rand() % 4)
        ); break;
        case 'B': tmp.push_back(
          static_cast<game::BlockType>
          (game::SQR_BUSH1 + rand() % 3)
        ); break;
        case '?': tmp.push_back(game::SQR_QUESTION_BLOCK); break;
        case 'C': tmp.push_back(game::SQR_COIN); break;
        case 'D': tmp.push_back(game::SQR_DOOR); break;
        case 'W': tmp.push_back(game::SQR_WOODPILE); break;
        case 'w': tmp.push_back(game::SQR_WATER); break;
        case 'S': tmp.push_back(game::SQR_SLAB); break;
        case 's': tmp.push_back(game::SQR_SAND); break;
        case '_': tmp.push_back(game::SQR_SLAB); break;
        case 'U': tmp.push_back(game::SQR_SLAB); break;
        case 'b': tmp.push_back(game::SQR_BOX); break;
        case 'f': tmp.push_back(game::SQR_FENCE); break;
        default: tmp.push_back(game::_SQR_EMPTY); break;
      }
    }

    output.push_back(tmp);
  }

  return output;
}
