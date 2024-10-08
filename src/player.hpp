#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "../cstmEngine/batch.hpp"
#include "../cstmEngine/types.hpp"
#include "../cstmEngine/window.hpp"
#include "../gameEngine/physics.hpp"
#include "../gameEngine/time.hpp"

#include "texture.hpp"

#include <vector>

namespace game {
  // the player side enum that is used to create a mirror of player
  // the both sides when needed.
  enum playerViewSide {
    SIDE_LEFT, SIDE_RIGHT,
  };

  struct PlayerLevelState {
    uint32_t m_points = 0;
    bool m_doorMsg = false;
    bool m_passed = false;
  };

  struct Player {
    // physics componenet includes many variables like
    // position, velocity, acceleration, force, friction etc.
    // to determine and calculate physics for the player.
    gameEngine::Physics m_phy;
    gameEngine::Time m_timer;
    // at the begining the player should be the first
    // texture.
    game::BlockType m_bt = TILE_PLAYER1;
    // at the begining the player will look forward.
    playerViewSide m_viewSide = SIDE_RIGHT;
    PlayerLevelState levelState;
    // the current frame idx this is used to render the correct 
    // frame after an interval of the player.
    uint32_t m_frameIdx = 0;
    float m_frameInterval;

    // an abbreviation for setting the size of the player.
    void init(cstmEngine::vec2 size, float mass, float frameInterval);

    // draws the player on screen.
    void render(cstmEngine::Batch &batch, const std::vector<cstmEngine::vec2> &maps);

    // analyze keyboard input
    void updateInput(const cstmEngine::Window &window, const gameEngine::Time &time,
                     float gameTileSize);
  };
}

#endif
