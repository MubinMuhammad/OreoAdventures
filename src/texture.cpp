#include "texture.hpp"
#include <iostream>

static void textureCropAtlas(
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
  textureCropAtlas(
    &v[k], &v[k + 1], &v[k + 2], &v[k + 3],
    atlas_wh, tex_factor, {1, 1}, {(float)(tex_x % 8), (float)tex_y}
  );

  k += 4;
  tex_x++;
  textureCropAtlas(
    &v[k], &v[k + 1], &v[k + 2], &v[k + 3],
    atlas_wh, tex_factor, {2, 1}, {(float)(tex_x % 8), (float)tex_y}
  );
  tex_x += 2;

  for (int i = 2; i < 23; i++) {
    k += 4;
    textureCropAtlas(
      &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
      atlas_wh, tex_factor, {1, 1}, {(float)(tex_x % 8), (float)tex_y}
    );
    tex_x++;
    if (tex_x % 8 == 0) tex_y++;
  }

  for (int i = 23; i < 26; i++) {
    k += 4;
    textureCropAtlas(
      &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
      atlas_wh, tex_factor, {2, 1}, {(float)(tex_x % 8), (float)tex_y}
    );
    tex_x += 2;
    if (tex_x % 8 == 0) tex_y++;
  }

  k += 4;
  textureCropAtlas(
    &v[k], &v[k + 1], &v[k + 2], &v[k + 3],
    atlas_wh, tex_factor, {1, 1}, {(float)(tex_x % 8), (float)tex_y}
  );
  tex_x++;
  if (tex_x % 8 == 0) tex_y++;

  k += 4;
  textureCropAtlas(
    &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
    atlas_wh, tex_factor, {1, 1}, {(float)(tex_x % 8), (float)tex_y}
  );
  tex_x++;
  if (tex_x % 8 == 0) tex_y++;

  k += 4;
  textureCropAtlas(
    &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
    atlas_wh, tex_factor, {2, 2}, {(float)(tex_x % 8), (float)tex_y}
  );
  tex_x += 2;
  if (tex_x % 8 == 0) tex_y++;
  printf("%d\n", tex_x);

  for (int i = 29; i < 35; i++) {
    k += 4;
    textureCropAtlas(
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
    textureCropAtlas(
      &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
      atlas_wh, tex_factor, {1, 1}, {(float)(tex_x % 8), (float)tex_y}
    );
    tex_x++;
  }

  if (tex_x % 8 == 0) tex_y++;

  for (int i = 41; i < 45; i++) {
    k += 4;
    textureCropAtlas(
      &v[k], &v[k + 1], &v[k + 2], &v[k + 3], 
      atlas_wh, tex_factor, {2, 2}, {(float)(tex_x % 8), (float)tex_y}
    );
    tex_x += 2;
    if (tex_x % 8 == 0) tex_y++;
  }

  std::cout
    << v[game::SQR_TREE1 * 4 + 0].x << ", " << v[game::SQR_TREE1 * 4 + 0].y << '\n'
    << v[game::SQR_TREE1 * 4 + 1].x << ", " << v[game::SQR_TREE1 * 4 + 1].y << '\n'
    << v[game::SQR_TREE1 * 4 + 2].x << ", " << v[game::SQR_TREE1 * 4 + 2].y << '\n'
    << v[game::SQR_TREE1 * 4 + 3].x << ", " << v[game::SQR_TREE1 * 4 + 3].y << "\n\n";
}

void game::textureGetCoords(
  std::vector<cstmEngine::vec2> &texGrid,
  game::BlockType bkType,
  cstmEngine::vec2 *outTexCoords
) {
  outTexCoords[0] = texGrid[bkType * 4 + 0];
  outTexCoords[1] = texGrid[bkType * 4 + 1];
  outTexCoords[2] = texGrid[bkType * 4 + 2];
  outTexCoords[3] = texGrid[bkType * 4 + 3];
}
