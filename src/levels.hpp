#ifndef GAME_LEVELS_HPP_
#define GAME_LEVELS_HPP_

#include "texture.hpp"
#include <vector>

namespace game {
  std::vector<std::vector<game::BlockType>> levelGet2DGrid(const char *level_path);
}


#endif
