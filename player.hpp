#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "cstmEngine/buffer.hpp"

struct Player {
  float x;
  float y;
  float w;
  float h;
  float mass;
  cstmEngine::vec2 velocity;
  cstmEngine::vec2 accel;
  cstmEngine::vec2 force;
  cstmEngine::vec2 friction;
  cstmEngine::vec2 tex_coords[4];
  bool on_ground = false;
};

#endif
