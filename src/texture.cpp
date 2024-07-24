#include "texture.hpp"
#include "../gameEngine/texture.hpp"

// this function stores the texture coordinates
// in the vector &v.
static void nextTextureCoords(
  cstmEngine::ivec2 &coords,
  int &k, std::vector<cstmEngine::vec2> &v,
  cstmEngine::vec2 sizeFactors,
  cstmEngine::vec2 atlas_wh,
  int texFactor
) {
  // as for texture coordinates we have 4 of them
  // they are top-left, bottom-left, bottom-right
  // top-right.
  //
  // therefore, textureCropAtlas() takes the 4
  // output coordinates of a part of the texture.
  // they are more likely to be a sprite of the
  // game.
  gameEngine::textureCropAtlas(
    &v[k], &v[k + 1], &v[k + 2], &v[k + 3],
    atlas_wh, texFactor, sizeFactors,
    {(float)(coords.x % (int)(atlas_wh.x / texFactor)), (float)(coords.y)}
  );

  // here k is increased by 4 becuase k is the
  // index on here to put output coordinates.
  //
  // here is a diagram to explain it better:
  // imagine this is the coordinate vector v.
  // here k is being like this:
  //
  //     k       k       k       k       k
  //     ↓       ↓       ↓       ↓       ↓
  //    ----------------------------------- ... and so on...
  //    |1|1|1|1|2|2|2|2|3|3|3|3|4|4|4|4|5|
  //    -----------------------------------
  //
  // this is the reason we do k + 1, k + 2, k + 3
  // so that we can access the elements forward
  // to k. and fill up the vector with coordinates
  // of all the sprites we have in our have.
  k += 4;
}

void game::textureGetCoordsIdxs(
  std::vector<cstmEngine::vec2> &v,
  std::vector<cstmEngine::vec2> &out_quad_size,
  cstmEngine::vec2 atlas_wh,
  int tex_factor
) {
  v.resize((4 + 3 + 2 + 13 + 2 + 3 + 2 + 4) * 4);
  out_quad_size.resize(4 + 3 + 2 + 13 + 2 + 3 + 2 + 4);
  int k = 0;
  cstmEngine::ivec2 texCoord = {0, 4};

  // these are the size factors used in game texture atlas
  // (not the font atlas) these then can be multiplied by
  // there renderSize to get the actual rendering size on the screen.

  // indexing rendering size factors
  for (int i = 0; i < 3; i++) out_quad_size[i] = {(float)i + 1, 1};
  out_quad_size[3] = {2, 1};
  for (int i = 4; i < 7; i++) out_quad_size[i] = {1, 1};
  for (int i = 7; i < 9; i++) out_quad_size[i] = {2, 1};
  for (int i = 9; i < 22; i++) out_quad_size[i] = {1, 1};
  for (int i = 22; i < 24; i++) out_quad_size[i] = {2, 1};
  for (int i = 24; i < 27; i++) out_quad_size[i] = {1, 1};
  for (int i = 27; i < 29; i++) out_quad_size[i] = {2, 2};
  for (int i = 29; i < 33; i++) out_quad_size[i] = {1, 2};

  // this is a boring way to set each
  // texture map to match the texture map in the atlas.
  // we have to painstakingly go through each sprite
  // in the atlas and call nextTextureCoords() to get
  // the coordinates needed.
  //
  // here,
  //
  // the texCoord.x is the topRight X coordinate
  // of the sprite in factors.
  //
  // the texCoord.y is the topRight Y coordinate
  // of the sprite in factors.

  // Getting texture coordinates

  for (int i = 0; i < 3; i++) {
    nextTextureCoords(texCoord, k, v, {(float)i + 1, 1}, atlas_wh, tex_factor);
    texCoord.x += i + 1;
  }

  nextTextureCoords(texCoord, k, v, {2, 1}, atlas_wh, tex_factor);
  texCoord.x += 2;

  for (int i = 4; i < 7; i++) {
    nextTextureCoords(texCoord, k, v, {1, 1}, atlas_wh, tex_factor);
    texCoord.x++;
  }

  for (int i = 7; i < 9; i++) {
    nextTextureCoords(texCoord, k, v, {2, 1}, atlas_wh, tex_factor);
    texCoord.x += 2;
  }

  for (int i = 9; i < 22; i++) {
    nextTextureCoords(texCoord, k, v, {1, 1}, atlas_wh, tex_factor);
    texCoord.x++;
    if (texCoord.x % (int)(atlas_wh.x / tex_factor) == 0) texCoord.y++;
  }

  for (int i = 22; i < 24; i++) {
    nextTextureCoords(texCoord, k, v, {2, 1}, atlas_wh, tex_factor);
    texCoord.x += 2;
    if (texCoord.x % (int)(atlas_wh.x / tex_factor) == 0) texCoord.y++;
  }

  for (int i = 24; i < 27; i++) {
    nextTextureCoords(texCoord, k, v, {1, 1}, atlas_wh, tex_factor);
    texCoord.x += 1;
    if (texCoord.x % (int)(atlas_wh.x / tex_factor) == 0) texCoord.y++;
  }

  for (int i = 27; i < 29; i++) {
    nextTextureCoords(texCoord, k, v, {2, 2}, atlas_wh, tex_factor);
    texCoord.x += 2;
  }

  for (int i = 29; i < 33; i++) {
    nextTextureCoords(texCoord, k, v, {1, 2}, atlas_wh, tex_factor);
    texCoord.x += 1;
  }
}
