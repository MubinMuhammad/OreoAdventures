#include "player.hpp"
#include "texture.hpp"

// set size is nothing but to set the size of the
// player. It is just handy to have. Instead of
// writing both statements you just write a function.
void game::Player::setSize(cstmEngine::vec2 size) {
  m_phy.m_size.x = size.x;
  m_phy.m_size.y = size.y;
}

// the render function is used to render the player
// on screen.
void game::Player::render(
  cstmEngine::Batch &batch,
  std::vector<cstmEngine::vec2> &textureGrid,
  gameEngine::Time &nextFrameTime,
  float nextFrameInterval
) {
  // the if statement checks if works like a stopwatch that
  // is begin ran after a certain interval. The interval is
  // the nextFrameInterval.
  // 
  // Here last time is the constant for a moment but the current
  // time goes up and up until the difference between them is
  // greater than or equal to nextFrameInterval, we do...
  if (nextFrameTime.m_crnt - nextFrameTime.m_last >= nextFrameInterval) {
    // now we set the last time to the crnt time.
    nextFrameTime.m_last = nextFrameTime.m_crnt;
    // this statement is changing the tile texture of the
    // player by adding frameIdx to SQR_PLAYER with mod 3.
    // where as the SQR_PLAYER1 is an enum and PLAYER1,
    // PLAYER2 and PLAYER3 are in a same sequence, they
    // have a incremental value of 1. so we can add the frameIdx % 3
    // to swap between textures.
    //
    // Q. why mod 3?
    // -------------
    // A. mod 3 because the frameIdx can increament more than
    // 3 but we have a only 3 player textues so we limit frameIdx
    // to 0 to 2 so that we can add it to SQR_PLAYER1 to get the
    // next frame. Hence, the name frameIdx.
    bt = (game::BlockType)(game::SQR_PLAYER1 + (frameIdx % 3));
    frameIdx++;
  }

  // read the src/texture.hpp file to understand what's happening
  // read the top section.
  cstmEngine::vec2 texCoords[4] = {
    textureGrid[bt * 4 + (m_viewSide == SIDE_RIGHT ? 0 : 3)],
    textureGrid[bt * 4 + (m_viewSide == SIDE_RIGHT ? 1 : 2)],
    textureGrid[bt * 4 + (m_viewSide == SIDE_RIGHT ? 2 : 1)],
    textureGrid[bt * 4 + (m_viewSide == SIDE_RIGHT ? 3 : 0)]
  };

  // adding the vertices to the batch so that
  // later we can call glDrawElements to draw the player.
  batch.drawQuad(m_phy.m_size, m_phy.m_pos, texCoords);
}
