#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "../gameEngine/physics.hpp"
#include "../cstmEngine/types.hpp"
#include "../cstmEngine/batch.hpp"
#include <vector>

namespace game {
  enum playerViewSide {
    SIDE_LEFT, SIDE_RIGHT
  };

  struct Player {
    gameEngine::Physics m_phy;
    playerViewSide m_viewSide = SIDE_RIGHT;
    bool m_onGround = false;

    void setSize(cstmEngine::vec2 size);
    void render(cstmEngine::Batch &batch, std::vector<cstmEngine::vec2> &textureGrid);
  };
}

#endif
