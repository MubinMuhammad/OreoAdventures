#ifndef GAME_LEVELS_HPP_
#define GAME_LEVELS_HPP_

#include "../cstmEngine/types.hpp"
#include "../cstmEngine/batch.hpp"
#include "texture.hpp"
#include "player.hpp"
#include "playState.hpp"

#include <string>
#include <vector>

namespace game {
  // A struct that can be changed by the renderLevel
  // function. It's used to get vital information like
  // - how many points the player has.
  // - what level player is in.
  // - should the door msg be shown when he comes to the door
  struct Level {
    // containing a copy of the level RLE
    std::string m_rle;
    // this point is different than the
    // PlayerLevelState.m_points as it contains
    // the how many points the player will need
    // to go to the next level.
    uint16_t m_points;
    // the length of the level this is basically the
    // width of the level useful when generating clouds.
    uint32_t m_length;
    // the coinstate is a considered as a 64 true or false
    // gates if the i-th bit is 0 then it means the coin
    // has been taken by the player and we should not be
    // rendering it anymore. Otherwise, we should render
    // and wait for the player to take it.
    uint64_t m_coinState = ~((uint64_t)0);
    // the will be true if the player should go to the
    // next level and false otherwise.
    bool m_passed = false;


    // renderLevel draws the level with the RIE
    // it maps the correct tiles and checks player
    // conditions.
    void renderLevel(
      cstmEngine::Batch &batch,
      game::Player &player, gamePlayState &playState,
      std::vector<cstmEngine::vec2> &textureGrid,
      std::vector<cstmEngine::vec2> &quadSizes,
      int tileSize,
      cstmEngine::vec2 windowSize,
      uint32_t gameSeed
    );

    private:
    // this function is used to render a single
    // tile it's private because it's only being
    // used by the renderLevel()
      void renderTile(
        cstmEngine::Batch &batch,
        std::vector<cstmEngine::vec2> &textureGrid,
        cstmEngine::vec2 tileSize,
        cstmEngine::vec2 coord,
        game::BlockType bt
      );
  };

  std::vector<Level> levelRead(std::vector<std::string> levelPaths);

  // render cloud renders clouds with a given
  // height and variablilty, the game seed is
  // going to be used along side with rand()
  // function in C to generate random numbers
  // with the given seed.
  void renderCloud(
    cstmEngine::Batch &batch,
    std::vector<cstmEngine::vec2> &textureGrid,
    std::vector<cstmEngine::vec2> &quadSizes,
    int tileSize,
    int cloudHeight,
    uint32_t variability,
    uint32_t seed,
    uint32_t levelLength
  );
}

#endif
