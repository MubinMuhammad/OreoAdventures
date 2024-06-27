#include "player.hpp"
#include "texture.hpp"

void game::Player::setSize(cstmEngine::vec2 size) {
  m_phy.m_size.x = size.x;
  m_phy.m_size.y = size.y;
}

void game::Player::render(
  cstmEngine::Batch &batch, 
  std::vector<cstmEngine::vec2> &textureGrid
) {
  cstmEngine::vec2 texCoords[4] = {
    textureGrid[game::SQR_PLAYER1 * 4 + (m_viewSide == SIDE_RIGHT ? 0 : 3)],
    textureGrid[game::SQR_PLAYER1 * 4 + (m_viewSide == SIDE_RIGHT ? 1 : 2)],
    textureGrid[game::SQR_PLAYER1 * 4 + (m_viewSide == SIDE_RIGHT ? 2 : 1)],
    textureGrid[game::SQR_PLAYER1 * 4 + (m_viewSide == SIDE_RIGHT ? 3 : 0)]
  };

  batch.drawQuadT(m_phy.m_size, {m_phy.m_pos}, texCoords);
}
