#ifndef GAME_ENGINE_PHYSICS_HPP_
#define GAME_ENGINE_PHYSICS_HPP_

#include "../cstmEngine/types.hpp"

namespace gameEngine {
  const float GRAVITY = 9.80665f;
  struct Physics {
    float m_mass;
    cstmEngine::vec2 m_size;
    cstmEngine::vec2 m_pos;
    cstmEngine::vec2 m_tmpPos = {0.0f, 0.0f};
    cstmEngine::vec2 m_velocity = {0.0f, 0.0f};
    cstmEngine::vec2 m_accel;
    cstmEngine::vec2 m_friction;

    void update(float deltaTime, cstmEngine::vec2 force, cstmEngine::vec2 frictionCoefficient,
                cstmEngine::vec2 halfWindowSize, float tileSize, cstmEngine::vec2 initialPos);

    void resetPosition(cstmEngine::vec2 halfWindowSize, float tileSize,
                       cstmEngine::vec2 newPos);

    bool checkCollision(cstmEngine::vec2 objCoords, cstmEngine::vec2 objSizeFactors,
                        int squareSize);
  };
}

#endif
