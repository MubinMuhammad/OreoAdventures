#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "../cstmEngine/batch.hpp"
#include "../cstmEngine/types.hpp"
#include "../gameEngine/physics.hpp"
#include "../gameEngine/time.hpp"
#include "texture.hpp"
#include <vector>

namespace game {
  enum playerViewSide {
    SIDE_LEFT, SIDE_RIGHT
  };

  struct Player {
    gameEngine::Physics m_phy;
    game::BlockType bt = SQR_PLAYER1;
    playerViewSide m_viewSide = SIDE_RIGHT;
    bool m_onGround = false;
    uint32_t frameIdx = 0;

    void setSize(cstmEngine::vec2 size);
    void render(
      cstmEngine::Batch &batch,
      std::vector<cstmEngine::vec2> &textureGrid,
      gameEngine::Time &nextFrameTime,
      float nextFrameInterval
    );
  };
}

#endif
