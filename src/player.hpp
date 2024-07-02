#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "../cstmEngine/batch.hpp"
#include "../cstmEngine/types.hpp"
#include "../gameEngine/physics.hpp"
#include "../gameEngine/time.hpp"
#include "texture.hpp"
#include <vector>

namespace game {
  // the player side enum that is used to create a mirror of player
  // the both sides when needed.
  enum playerViewSide {
    SIDE_LEFT, SIDE_RIGHT
  };

  struct Player {
    // physics componenet includes many variables like
    // position, velocity, acceleration, force, friction etc.
    // to determine and calculate physics for the player.
    gameEngine::Physics m_phy;
    // at the begining the player should be the first
    // texture.
    game::BlockType m_bt = SQR_PLAYER1;
    // at the begining the player will look forward.
    playerViewSide m_viewSide = SIDE_RIGHT;
    // the current frame idx this is used to render the correct 
    // frame after an interval of the player.
    uint32_t m_frameIdx = 0;

    // an abbreviation for setting the size
    // of the player.
    void setSize(cstmEngine::vec2 size);

    // draws the player on screen.
    void render(
      cstmEngine::Batch &batch,
      std::vector<cstmEngine::vec2> &textureGrid,
      gameEngine::Time &nextFrameTime,
      float nextFrameInterval
    );
  };
}

#endif
