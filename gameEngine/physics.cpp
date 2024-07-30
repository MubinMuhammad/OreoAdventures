#include "physics.hpp"

void gameEngine::Physics::update(float deltaTime, cstmEngine::vec2 force,
                                 cstmEngine::vec2 frictionCoefficient,
                                 cstmEngine::vec2 halfWindowSize, float tileSize,
                                 cstmEngine::vec2 initialPos) {
  m_friction.x = frictionCoefficient.x * GRAVITY * m_mass;
  m_friction.y = frictionCoefficient.y * GRAVITY * m_mass;

  m_pos.x = -halfWindowSize.x + tileSize / 2.0f + initialPos.x;
  m_pos.y = -halfWindowSize.y + tileSize / 2.0f + initialPos.y;

  m_accel.x = force.x / m_mass;
  m_accel.y = force.y / m_mass;

  m_velocity.x += m_accel.x;
  m_velocity.y += m_accel.y;

  if (m_velocity.x > 0.0f) m_velocity.x -= m_friction.x / m_mass;
  else if (m_velocity.x < 0.0f) m_velocity.x += m_friction.x / m_mass;

  if (m_velocity.y > 0.0f) m_velocity.y -= m_friction.y / m_mass;
  else if (m_velocity.y < 0.0f) m_velocity.y += m_friction.y / m_mass;

  m_tmpPos.x += m_velocity.x * deltaTime;
  m_tmpPos.y += m_velocity.y * deltaTime;

  m_pos.x += m_tmpPos.x;
  m_pos.y += m_tmpPos.y;
}

void gameEngine::Physics::resetPosition(cstmEngine::vec2 halfWindowSize, float tileSize,
                                        cstmEngine::vec2 newPos) {
  m_pos.x = -halfWindowSize.x + tileSize / 2.0f + newPos.x;
  m_pos.y = -halfWindowSize.y + tileSize / 2.0f + newPos.y;

  m_tmpPos = {0.0f, 0.0f};
  m_velocity = {0.0f, 0.0f};
  m_accel = {0.0f, 0.0f};
}

bool gameEngine::Physics::checkCollision(cstmEngine::vec2 objCoords, cstmEngine::vec2 objSizeFactors,
                                         int squareSize) {
  return
    m_pos.x + (m_size.x / 2) >= objCoords.x - ((objSizeFactors.x * squareSize) / 2) &&
    m_pos.x - (m_size.x / 2) <= objCoords.x + ((objSizeFactors.x * squareSize) / 2) &&
    m_pos.y + (m_size.y / 2) >= objCoords.y - ((objSizeFactors.y * squareSize) / 2) &&
    m_pos.y - (m_size.y / 2) <= objCoords.y + ((objSizeFactors.y * squareSize) / 2);
}
