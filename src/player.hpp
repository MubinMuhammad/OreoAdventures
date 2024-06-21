#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "../gameEngine/physics.hpp"
#include "../cstmEngine/types.hpp"
#include "../cstmEngine/batch.hpp"
#include "texture.hpp"
#include <vector>

namespace game {
  enum playerViewSide {
    SIDE_LEFT, SIDE_RIGHT
  };

  struct Player {
    gameEngine::Physics m_phy;
    cstmEngine::vec2 m_size = {30, 30};
    playerViewSide m_viewSide = SIDE_RIGHT;
    bool m_onGround = false;

    void render(cstmEngine::Batch &batch, std::vector<cstmEngine::vec2> &textureGrid);
  };
}

#endif
