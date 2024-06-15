#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "cstmEngine/buffer.hpp"

struct Player {
  float x, y, w, h;
  cstmEngine::Vector2 tex_coords[4];
  float velocity, accel, friction;
};

#endif
