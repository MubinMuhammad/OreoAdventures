#include "texture.hpp"
#include "../gameEngine/texture.hpp"

void game::textureGetCoordsIdxs(
  std::vector<cstmEngine::vec2> &v,
  std::vector<cstmEngine::vec2> &out_quad_size,
  cstmEngine::vec2 atlas_wh,
  int tex_factor
) {
  v.resize(45 * 4);
  out_quad_size.resize(45);
  int k = 0;
  int tex_x = 0;
  int tex_y = 0;

  // indexing rendering size factors
  out_quad_size[0] = {1, 1};
  out_quad_size[1] = {2, 1};

  for (int i = 2; i < 23; i++)
    out_quad_size[i] = {1, 1};

  for (int i = 23; i < 26; i++)
    out_quad_size[i] = {2, 1};

  out_quad_size[26] = {1, 1};
  out_quad_size[27] = {1, 1};
  out_quad_size[28] = {2, 2};

  for (int i = 29; i < 41; i++) {
    out_quad_size[i] = {1, 1};
  }

  for (int i = 41; i < 45; i++) {
    out_quad_size[i] = {2, 2};
  }

  // Getting texture coordinates
  gameEngine::textureCropAtlas(
    &v[k], &v[k + 1], &v[k + 2], &v[k + 3],
    atlas_wh, tex_factor, {1, 1}, {(float)(tex_x % 8), (float)tex_y}
  );

  k += 4;
  tex_x++;
  gameEngine::textureCropAtlas(
    &v[k], &v[k + 1], &v[k + 2], &v[k + 3],
    atlas_wh, tex_factor, {2, 1}, {(float)(tex_x % 8), (float)tex_y}
  );
  tex_x += 2;

  for (int i = 2; i < 23; i++) {
    k += 4;
    gameEngine::textureCropAtlas(
      &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
      atlas_wh, tex_factor, {1, 1}, {(float)(tex_x % 8), (float)tex_y}
    );
    tex_x++;
    if (tex_x % 8 == 0) tex_y++;
  }

  for (int i = 23; i < 26; i++) {
    k += 4;
    gameEngine::textureCropAtlas(
      &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
      atlas_wh, tex_factor, {2, 1}, {(float)(tex_x % 8), (float)tex_y}
    );
    tex_x += 2;
    if (tex_x % 8 == 0) tex_y++;
  }

  k += 4;
  gameEngine::textureCropAtlas(
    &v[k], &v[k + 1], &v[k + 2], &v[k + 3],
    atlas_wh, tex_factor, {1, 1}, {(float)(tex_x % 8), (float)tex_y}
  );
  tex_x++;
  if (tex_x % 8 == 0) tex_y++;

  k += 4;
  gameEngine::textureCropAtlas(
    &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
    atlas_wh, tex_factor, {1, 1}, {(float)(tex_x % 8), (float)tex_y}
  );
  tex_x++;
  if (tex_x % 8 == 0) tex_y++;

  k += 4;
  gameEngine::textureCropAtlas(
    &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
    atlas_wh, tex_factor, {2, 2}, {(float)(tex_x % 8), (float)tex_y}
  );
  tex_x += 2;
  if (tex_x % 8 == 0) tex_y++;

  for (int i = 29; i < 35; i++) {
    k += 4;
    gameEngine::textureCropAtlas(
      &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
      atlas_wh, tex_factor, {1, 1}, {(float)(tex_x % 8), (float)tex_y}
    );
    tex_x++;
  }

  if (tex_x % 8 == 0) {
    tex_y++;
    tex_x += 2;
  }

  for (int i = 35; i < 41; i++) {
    k += 4;
    gameEngine::textureCropAtlas(
      &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
      atlas_wh, tex_factor, {1, 1}, {(float)(tex_x % 8), (float)tex_y}
    );
    tex_x++;
  }

  if (tex_x % 8 == 0) tex_y++;

  for (int i = 41; i < 45; i++) {
    k += 4;
    gameEngine::textureCropAtlas(
      &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
      atlas_wh, tex_factor, {2, 2}, {(float)(tex_x % 8), (float)tex_y}
    );
    tex_x += 2;
    if (tex_x % 8 == 0) tex_y++;
  }
}
