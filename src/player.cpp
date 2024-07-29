#include "player.hpp"
#include "texture.hpp"

#include <GLFW/glfw3.h>
#include <ctime>
#include <algorithm>

// set size is nothing but to set the size of the
// player. It is just handy to have. Instead of
// writing both statements you just write a function.
void game::Player::init(cstmEngine::vec2 size, float mass, float frameInterval) {
  m_phy.m_size = size;
  m_phy.m_mass = mass;
  m_frameInterval = frameInterval;
}

// the render function is used to render the player
// on screen.
void game::Player::render(cstmEngine::Batch &batch, const std::vector<cstmEngine::vec2> &maps) {
  m_timer.m_crnt = glfwGetTime();
  // the if statement checks if works like a stopwatch that
  // is begin ran after a certain interval. The interval is
  // the nextFrameInterval.
  // 
  // Here last time is the constant for a moment but the current
  // time goes up and up until the difference between them is
  // greater than or equal to nextFrameInterval, we do...
  if (m_timer.m_crnt - m_timer.m_last >= m_frameInterval) {
    // now we set the last time to the crnt time.
    m_timer.m_last = m_timer.m_crnt;
    // this statement is changing the tile texture of the
    // player by adding frameIdx to TILE_PLAYER with mod 3.
    // where as the TILE_PLAYER1 is an enum and PLAYER1,
    // PLAYER2 and PLAYER3 are in the same sequence, they
    // have a incremental value of 1. so we can add the frameIdx % 3
    // to swap between textures.
    //
    // Q. why mod 3?
    // -------------
    // A. mod 3 because the frameIdx can increament more than
    // 3 but we have a only 3 player textues so we limit frameIdx
    // to 0 to 2 so that we can add it to TILE_PLAYER1 to get the
    // next frame. Hence, the name "frameIdx".
    m_bt = (game::BlockType)(game::TILE_PLAYER1 + (m_frameIdx % 3));
    m_frameIdx++;
  }

  // read the src/texture.hpp file to understand what's happening
  // read the top section.
  cstmEngine::vec2 texCoords[4] = {
    maps[m_bt * 4 + (m_viewSide == SIDE_RIGHT ? 0 : 3)],
    maps[m_bt * 4 + (m_viewSide == SIDE_RIGHT ? 1 : 2)],
    maps[m_bt * 4 + (m_viewSide == SIDE_RIGHT ? 2 : 1)],
    maps[m_bt * 4 + (m_viewSide == SIDE_RIGHT ? 3 : 0)]
  };

  // adding the vertices to the batch so that
  // later we can call glDrawElements to draw the player.
  batch.drawQuad(m_phy.m_size, m_phy.m_pos, texCoords);
}

void game::Player::updateInput(const cstmEngine::Window &window, const gameEngine::Time &timer,
                               float gameTileSize) {
    // here we just set a different forces when the player
    // presses specific keys.
    cstmEngine::vec2 playerForce;

    if (glfwGetKey(window.m_window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window.m_window, GLFW_KEY_LEFT) == GLFW_PRESS ||
        glfwGetKey(window.m_window, GLFW_KEY_H) == GLFW_PRESS) {
      playerForce.x = -1000.0f;
      m_viewSide = game::SIDE_LEFT;
    }
    else if (glfwGetKey(window.m_window, GLFW_KEY_D) == GLFW_PRESS ||
             glfwGetKey(window.m_window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
             glfwGetKey(window.m_window, GLFW_KEY_L) == GLFW_PRESS) {
      playerForce.x = 1000.0f;
      m_viewSide = game::SIDE_RIGHT;
    }
    else
      playerForce.x =  0.0f;

    if (glfwGetKey(window.m_window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window.m_window, GLFW_KEY_UP) == GLFW_PRESS ||
        glfwGetKey(window.m_window, GLFW_KEY_K) == GLFW_PRESS)
      playerForce.y = 500.0f;
    else if (glfwGetKey(window.m_window, GLFW_KEY_S) == GLFW_PRESS ||
             glfwGetKey(window.m_window, GLFW_KEY_DOWN) == GLFW_PRESS ||
             glfwGetKey(window.m_window, GLFW_KEY_J) == GLFW_PRESS)
      playerForce.y = -500.0f;
    else
      playerForce.y = 0.0f;

    // we update the physics of the player each frame
    m_phy.update(timer.m_delta, playerForce, {0.7f, 0.7f},
                 {(float)window.m_width / 2, (float)window.m_height / 2}, gameTileSize,
                 {10.0f, 200.0f});

    // set a maximum velocity player can reach in both
    // dimensions. if we don't limit the player's velocity,
    // it will get increased and increased by the
    // player's acceleration and may touch the speed of
    // light, so where's some code to stop that!
    m_phy.m_velocity.x = std::clamp(m_phy.m_velocity.x, -500.0f, 500.0f);
    m_phy.m_velocity.y = std::clamp(m_phy.m_velocity.y, -500.0f, 500.0f);
}
