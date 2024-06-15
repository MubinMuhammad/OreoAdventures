#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "cstmEngine/buffer.hpp"

struct Player {
  float x;
  float y;
  float w;
  float h;
  float mass;
  cstmEngine::Vector2 velocity;
  cstmEngine::Vector2 accel;
  cstmEngine::Vector2 force;
  cstmEngine::Vector2 friction;
  cstmEngine::Vector2 tex_coords[4];
  bool on_ground = false;
};

#endif
