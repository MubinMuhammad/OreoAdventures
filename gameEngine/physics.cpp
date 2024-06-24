#include "physics.hpp"
#include <cmath>

void gameEngine::Physics::update(
      float deltaTime,
      cstmEngine::vec2 force,
      cstmEngine::vec2 frictionCoefficient
) {
  m_friction.x = frictionCoefficient.x * GRAVITY * m_mass;
  m_friction.y = frictionCoefficient.y * GRAVITY * m_mass;

  m_accel.x = (force.x / m_mass);
  m_accel.y = (force.y / m_mass);

  m_velocity.x += m_accel.x;
  m_velocity.y += m_accel.y;
  m_velocity.y -= GRAVITY;

  if (fabs(m_velocity.x) <= 0.1f) m_velocity.x = 0.0f;

  if (m_velocity.x > 0.0f) m_velocity.x -= m_friction.x / m_mass;
  else if (m_velocity.x < 0.0f) m_velocity.x += m_friction.x / m_mass;

  if (fabs(m_velocity.y) <= 0.1f) m_velocity.y = 0.0f;

  if (m_velocity.y > 0.0f) m_velocity.y -= m_friction.y / m_mass;
  else if (m_velocity.y < 0.0f) m_velocity.y += m_friction.y / m_mass;

  m_pos.x += m_velocity.x * deltaTime;
  m_pos.y += m_velocity.y * deltaTime;
}

bool gameEngine::Physics::checkCollision(
  cstmEngine::vec2 objCoords,
  cstmEngine::vec2 objSizeFactors,
  int squareSize
) {
  float objMaxX = m_pos.x + (m_size.x / 2);
  float objMaxY = m_pos.y + (m_size.y / 2);
  float objMinX = m_pos.x - (m_size.x / 2);
  float objMinY = m_pos.y - (m_size.y / 2);

  float givenObjMaxX = objCoords.x + ((objSizeFactors.x * squareSize) / 2);
  float givenObjMaxY = objCoords.y + ((objSizeFactors.x * squareSize) / 2);
  float givenObjMinX = objCoords.x - ((objSizeFactors.x * squareSize) / 2);
  float givenObjMinY = objCoords.y - ((objSizeFactors.y * squareSize) / 2);

  return
    objMinX < givenObjMaxX && objMaxX > givenObjMinX &&
    objMinY < givenObjMaxY && objMaxY > givenObjMinY; 
}
