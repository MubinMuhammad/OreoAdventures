#ifndef GAME_LEVELS_HPP_
#define GAME_LEVELS_HPP_

#include "texture.hpp"
#include "../cstmEngine/types.hpp"
#include "../cstmEngine/batch.hpp"
#include <string>
#include <vector>

namespace game {
  struct Level {
    std::string m_levelRle;
    int m_levelPoints;

    void loadLevel(std::string levelRle, int levelPoints);

    void renderLevel(
      cstmEngine::Batch &batch,
      std::vector<cstmEngine::vec2> &textureGrid,
      std::vector<cstmEngine::vec2> &quadSizes,
      int tileSize,
      cstmEngine::vec2 windowSize,
      uint32_t gameSeed
    );

    private:
      void renderTile(
        cstmEngine::Batch &batch,
        std::vector<cstmEngine::vec2> &textureGrid,
        cstmEngine::vec2 tileSize,
        cstmEngine::vec2 coord,
        game::BlockType bt
      );
  };
}

#endif
