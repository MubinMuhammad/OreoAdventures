#ifndef GAME_ENGINE_PHYSICS_HPP_
#define GAME_ENGINE_PHYSICS_HPP_

#include "../cstmEngine/types.hpp"

namespace gameEngine {
  const float GRAVITY = 9.80665f;
  struct Physics {
    float m_mass;
    cstmEngine::vec2 m_size;
    cstmEngine::vec2 m_pos;
    cstmEngine::vec2 m_velocity;
    cstmEngine::vec2 m_accel;
    cstmEngine::vec2 m_friction;

    void update(
      float deltaTime,
      cstmEngine::vec2 force,
      cstmEngine::vec2 frictionCoefficient
    );

    bool checkCollision(
      cstmEngine::vec2 objCoords,
      cstmEngine::vec2 objSizeFactors,
      int squareSize
    );
  };
}

#endif
