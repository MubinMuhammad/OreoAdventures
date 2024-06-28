#include "texture.hpp"
#include "../gameEngine/texture.hpp"

static void nextTextureCoords(
  cstmEngine::ivec2 &coords,
  int &k, std::vector<cstmEngine::vec2> &v,
  cstmEngine::vec2 sizeFactors,
  cstmEngine::vec2 atlas_wh,
  int texFactor
) {
  gameEngine::textureCropAtlas(
    &v[k], &v[k + 1], &v[k + 2], &v[k + 3],
    atlas_wh, texFactor, sizeFactors,
    {
      (float)(coords.x % 8),
      (float)(coords.y)
    }
  );
  k += 4;
}

void game::textureGetCoordsIdxs(
  std::vector<cstmEngine::vec2> &v,
  std::vector<cstmEngine::vec2> &out_quad_size,
  cstmEngine::vec2 atlas_wh,
  int tex_factor
) {
  v.resize(30 * 4);
  out_quad_size.resize(30);
  int k = 0;
  cstmEngine::ivec2 texCoord;

  // indexing rendering size factors
  out_quad_size[0] = {1, 1};
  out_quad_size[1] = {2, 1};

  for (int i = 2; i < 17; i++)
    out_quad_size[i] = {1, 1};

  for (int i = 17; i < 19; i++)
    out_quad_size[i] = {2, 1};

  for (int i = 19; i < 26; i++)
    out_quad_size[i] = {1, 1};

  for (int i = 26; i < 28; i++)
    out_quad_size[i] = {2, 2};

  out_quad_size[28] = {2, 4};
  out_quad_size[29] = {2, 3};

  // Getting texture coordinates
  nextTextureCoords(texCoord, k, v, {1, 1}, atlas_wh, tex_factor);
  texCoord.x++;
  nextTextureCoords(texCoord, k, v, {2, 1}, atlas_wh, tex_factor);
  texCoord.x += 2;

  for (int i = 2; i < 17; i++) {
    nextTextureCoords(texCoord, k, v, {1, 1}, atlas_wh, tex_factor);
    texCoord.x++;
    if (texCoord.x % 8 == 0) texCoord.y++;
  }

  for (int i = 17; i < 19; i++) {
    nextTextureCoords(texCoord, k, v, {2, 1}, atlas_wh, tex_factor);
    texCoord.x += 2;
  }

  for (int i = 19; i < 24; i++) {
    nextTextureCoords(texCoord, k, v, {1, 1}, atlas_wh, tex_factor);
    texCoord.x++;
    if (texCoord.x % 8 == 0) texCoord.y++;
  }
  texCoord.x += 3;

  nextTextureCoords(texCoord, k, v, {1, 1}, atlas_wh, tex_factor);
  texCoord.x++;
  nextTextureCoords(texCoord, k, v, {1, 1}, atlas_wh, tex_factor);
  texCoord.x++;

  if (texCoord.x % 8 == 0) texCoord.y++;
  texCoord.y += 2;

  nextTextureCoords(texCoord, k, v, {2, 2}, atlas_wh, tex_factor);
  texCoord.x += 2;
  nextTextureCoords(texCoord, k, v, {2, 2}, atlas_wh, tex_factor);
  texCoord.x += 2;

  texCoord.y -= 2;

  nextTextureCoords(texCoord, k, v, {2, 4}, atlas_wh, tex_factor);
  texCoord.x += 2;
  texCoord.y--;

  nextTextureCoords(texCoord, k, v, {2, 3}, atlas_wh, tex_factor);
}
