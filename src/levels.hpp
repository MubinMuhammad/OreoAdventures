#ifndef GAME_LEVELS_HPP_
#define GAME_LEVELS_HPP_

#include "texture.hpp"
#include "../cstmEngine/types.hpp"
#include "../cstmEngine/batch.hpp"
#include <vector>

namespace game {
  std::vector<std::vector<game::BlockType>> levelGet2DGrid(
    const char *level_path, 
    int &outLevelPoints
  );

  void levelRenderTile(
    cstmEngine::Batch &batch,
    std::vector<cstmEngine::vec2> textureGrid,
    int tileSize,
    cstmEngine::vec2 coord,
    game::BlockType bt
  );
}

#endif
