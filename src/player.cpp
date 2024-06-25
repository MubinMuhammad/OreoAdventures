#include "player.hpp"
#include "texture.hpp"
#include <algorithm>
#include <iostream>

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

  float tileTop    = objCoords.y + ((objSizeFactors.y * squareSize) / 2);
  float tileRight  = objCoords.x + ((objSizeFactors.x * squareSize) / 2);
  float tileBottom = objCoords.y - ((objSizeFactors.y * squareSize) / 2);
  float tileLeft   = objCoords.x - ((objSizeFactors.x * squareSize) / 2);

  float _error = 1e-6;

  // Player touching the ceiling
  if (tileBottom <= playerTop + _error && tileBottom > playerBottom &&
      playerRight > tileLeft && playerLeft < tileRight) {
    printf("touching Ceiling\n");
  }
  // Player touching the ground
  else if (tileTop >= playerBottom - _error && tileTop < playerTop &&
           playerRight > tileLeft && playerLeft < tileRight) {
    printf("touching ground\n");
  }
  // Player touching the right side of a tile
  else if (tileLeft <= playerRight + _error && tileLeft > playerLeft &&
           playerBottom < tileTop && playerTop > tileBottom) {
    printf("touching object at right\n");
  }
  // Player touching the left side of a tile
  else if (tileRight >= playerLeft - _error && tileRight < playerRight &&
           playerBottom < tileTop && playerTop > tileBottom) {
    printf("touching object at left\n");
  }
}
