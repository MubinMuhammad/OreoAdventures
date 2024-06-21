#ifndef GAME_ENGINE_PHYSICS_H_
#define GAME_ENGINE_PHYSICS_H_

#include "../cstmEngine/types.hpp"

namespace gameEngine {
  const float GRAVITY = 9.80665f;
  struct Physics {
    float m_mass;
    cstmEngine::vec2 m_pos;
    cstmEngine::vec2 m_velocity;
    cstmEngine::vec2 m_accel;
    cstmEngine::vec2 m_friction;

    void update(
      float deltaTime,
      cstmEngine::vec2 force,
      cstmEngine::vec2 frictionCoefficient
    );
  };
}

#endif
