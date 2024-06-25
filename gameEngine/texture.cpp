#include "texture.hpp"

void gameEngine::textureCropAtlas(
  cstmEngine::vec2 *out_coord1,
  cstmEngine::vec2 *out_coord2,
  cstmEngine::vec2 *out_coord3,
  cstmEngine::vec2 *out_coord4,
  const cstmEngine::vec2 &atlas_wh,
  const int tex_factor,
  const cstmEngine::vec2 &tex_wh,
  const cstmEngine::vec2 &pos_xy
) {
  *out_coord1 = {
    (pos_xy.x * tex_factor) / atlas_wh.x,
    (atlas_wh.y - pos_xy.y * tex_factor) / atlas_wh.y
  };

  *out_coord2 = {
    (pos_xy.x * tex_factor) / atlas_wh.x,
    (atlas_wh.y - (pos_xy.y + tex_wh.y) * tex_factor) / atlas_wh.y
  };

  *out_coord3 = {
    ((pos_xy.x + tex_wh.x) * tex_factor) / atlas_wh.x,
    (atlas_wh.y - (pos_xy.y + tex_wh.y) * tex_factor) / atlas_wh.y
  };

  *out_coord4 = {
    ((pos_xy.x + tex_wh.x) * tex_factor) / atlas_wh.x,
    (atlas_wh.y - pos_xy.y * tex_factor) / atlas_wh.y
  };
}

void gameEngine::textureGetCoords(
  std::vector<cstmEngine::vec2> &texGrid, int idx,
  cstmEngine::vec2 *outTexCoords
) {
  outTexCoords[0] = texGrid[idx * 4 + 0];
  outTexCoords[1] = texGrid[idx * 4 + 1];
  outTexCoords[2] = texGrid[idx * 4 + 2];
  outTexCoords[3] = texGrid[idx * 4 + 3];
}
