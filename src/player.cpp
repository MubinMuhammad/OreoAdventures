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

void game::Player::resolveCollision(
  cstmEngine::vec2 objCoords,
  cstmEngine::vec2 objSizeFactors,
  int squareSize
) {
  float playerTop    = m_phy.m_pos.y + (m_phy.m_size.y / 2);
  float playerRight  = m_phy.m_pos.x + (m_phy.m_size.x / 2);
  float playerBottom = m_phy.m_pos.y - (m_phy.m_size.y / 2);
  float playerLeft   = m_phy.m_pos.x - (m_phy.m_size.x / 2);

  float objTop    = objCoords.y + ((objSizeFactors.x * squareSize) / 2);
  float objRight  = objCoords.x + ((objSizeFactors.x * squareSize) / 2);
  float objBottom = objCoords.y - ((objSizeFactors.y * squareSize) / 2);
  float objLeft   = objCoords.x - ((objSizeFactors.x * squareSize) / 2);

  // player touching the ceiling
  if (playerTop >= objBottom) {
    m_phy.m_pos.y = std::min(objBottom, playerTop) - (m_phy.m_size.y) / 2;
    m_phy.m_velocity.y = 0.0f;
  }
  // player touching a right object
  if (playerRight >= objLeft) {
    m_phy.m_pos.x = std::min(objLeft, playerRight) - (m_phy.m_size.y) / 2;
    m_phy.m_velocity.x = 0.0f;
  }
  // player touching the ground
  if (playerBottom <= objTop) {
    m_phy.m_pos.y = std::max(objTop, playerBottom) - (m_phy.m_size.y) / 2;
    m_phy.m_velocity.y = 0.0f;
  }
  // player touching a left object
  if (playerLeft <= objRight) {
    m_phy.m_pos.x = std::max(objRight, playerLeft) - (m_phy.m_size.y) / 2;
    m_phy.m_velocity.x = 0.0f;
  }
}
