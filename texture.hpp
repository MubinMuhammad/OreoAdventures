#ifndef _GAME_TEXTURE_H_
#define _GAME_TEXTURE_H_

#include "cstmEngine/buffer.hpp"
#include <vector>

enum textureIdx {
  GAME_CLOUD1 = 0, GAME_CLOUD2,
  GAME_CLOUD3, GAME_CLOUD4,
  GAME_COIN, GAME_GRASS,
  GAME_DIRT, GAME_HEART1,
  GAME_HEART2, GAME_HEART3,
  GAME_QUESTION_BLOCK, GAME_FENCE,
  GAME_WOODPILE, GAME_CACTUS,
  GAME_SAND, GAME_DOOR,
  GAME_PIPE, GAME_PIPE_HEAD,
  GAME_SLAB, GAME_PLAYER1,
  GAME_PLAYER2, GAME_WATER,
  GAME_BRICK, GAME_BUSH1,
  GAME_BUSH2, GAME_BUSH3,
  GAME_NITROGEN_BOX, GAME_BOX,
  GAME_BANGLADESH_FLAG, GAME_UNKNOWN,
  GAME_TREE1 = 41, GAME_TREE2,
  GAME_TREE3, GAME_TREE4,
};

void getTexCoordsFromAtlas(
  std::vector<cstmEngine::vec2> &v,
  std::vector<cstmEngine::vec2> &quad_size,
  cstmEngine::vec2 atlas_wh,
  int tex_factor
);

void getRenderingWHFactors(
  cstmEngine::vec2 &out_wh, 
  std::vector<cstmEngine::vec2> &texture_grid,
  cstmEngine::vec2 atlas_wh,
  textureIdx tex_type,
  int tex_factor
);

#endif
