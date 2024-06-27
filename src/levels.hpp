#ifndef GAME_LEVELS_HPP_
#define GAME_LEVELS_HPP_

#include "texture.hpp"
#include "../cstmEngine/types.hpp"
#include "../cstmEngine/batch.hpp"
#include "player.hpp"
#include <string>
#include <vector>

namespace game {
  struct PlayerLevelState {
    uint32_t score = 0;
  };

  struct Level {
    std::string m_levelRle;
    int m_levelPoints;
    uint64_t coinState = ~((uint64_t)0);

    void loadLevel(std::string levelRle, int levelPoints);

    void renderLevel(
      cstmEngine::Batch &batch,
      game::Player player,
      PlayerLevelState &playerState,
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

  void renderCloud(
    cstmEngine::Batch &batch,
    std::vector<cstmEngine::vec2> &textureGrid,
    std::vector<cstmEngine::vec2> &quadSizes,
    int tileSize,
    int cloudHeight,
    uint32_t variability,
    uint32_t seed
  );
}

#endif
