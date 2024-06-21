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
  /*m_velocity.y -= GRAVITY;*/

  if (m_velocity.x > 0.0f) m_velocity.x -= m_friction.x;
  else if (m_velocity.x < 0.0f) m_velocity.x += m_friction.x;

  if (fabs(m_velocity.x) < 0.1f) m_velocity.x = 0.0f;

  if (m_velocity.y > 0.0f) m_velocity.y -= m_friction.y;
  else if (m_velocity.y < 0.0f) m_velocity.y += m_friction.y;

  if (fabs(m_velocity.y) < 0.1f) m_velocity.y = 0.0f;

  m_pos.x += m_velocity.x * deltaTime;
  m_pos.y += m_velocity.y * deltaTime;
}
