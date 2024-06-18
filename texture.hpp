#ifndef _GAME_TEXTURE_H_
#define _GAME_TEXTURE_H_

#include "cstmEngine/buffer.hpp"

void textureCropAtlas(
  cstmEngine::Vector2 *out_tex_coords,
  const cstmEngine::Vector2 &atlas_wh,
  const int tex_factor,
  const cstmEngine::Vector2 &tex_wh,
  const cstmEngine::Vector2 &pos_xy
) {
  out_tex_coords[0] = {(pos_xy.x * tex_factor)              / atlas_wh.x, (atlas_wh.y - pos_xy.y * tex_factor)              / atlas_wh.y};
  out_tex_coords[1] = {(pos_xy.x * tex_factor)              / atlas_wh.x, (atlas_wh.y - (pos_xy.y + tex_wh.y) * tex_factor) / atlas_wh.y};
  out_tex_coords[2] = {((pos_xy.x + tex_wh.x) * tex_factor) / atlas_wh.x, (atlas_wh.y - (pos_xy.y + tex_wh.y) * tex_factor) / atlas_wh.y};
  out_tex_coords[3] = {((pos_xy.x + tex_wh.x) * tex_factor) / atlas_wh.x, (atlas_wh.y - pos_xy.y * tex_factor)              / atlas_wh.y};
}

#endif
