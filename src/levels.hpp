#ifndef GAME_LEVELS_HPP_
#define GAME_LEVELS_HPP_

#include "../cstmEngine/types.hpp"
#include "../cstmEngine/batch.hpp"
#include "texture.hpp"
#include "player.hpp"

#include <string>
#include <vector>

namespace game {
  struct PlayerLevelState {
    uint32_t score = 0;
    int crntLevel = 0;
    bool doorMsg = false;
  };

  struct Level {
    std::string m_levelRle;
    int m_levelPoints;
    uint64_t coinState = ~((uint64_t)0);
    bool levelPassed = false;

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
