#include "player.hpp"
#include "texture.hpp"

void game::Player::setSize(cstmEngine::vec2 size) {
  m_phy.m_size.x = size.x;
  m_phy.m_size.y = size.y;
}

void game::Player::render(
  cstmEngine::Batch &batch,
  std::vector<cstmEngine::vec2> &textureGrid,
  gameEngine::Time &nextFrameTime,
  float nextFrameInterval
) {
  if (nextFrameTime.m_crnt - nextFrameTime.m_last >= nextFrameInterval) {
    nextFrameTime.m_last = nextFrameTime.m_crnt;
    bt = (game::BlockType)(game::SQR_PLAYER1 + (frameIdx % 3));
    frameIdx++;
  }

  cstmEngine::vec2 texCoords[4] = {
    textureGrid[bt * 4 + (m_viewSide == SIDE_RIGHT ? 0 : 3)],
    textureGrid[bt * 4 + (m_viewSide == SIDE_RIGHT ? 1 : 2)],
    textureGrid[bt * 4 + (m_viewSide == SIDE_RIGHT ? 2 : 1)],
    textureGrid[bt * 4 + (m_viewSide == SIDE_RIGHT ? 3 : 0)]
  };

  batch.drawQuad(m_phy.m_size, m_phy.m_pos, texCoords);
}
