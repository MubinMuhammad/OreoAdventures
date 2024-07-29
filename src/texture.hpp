#ifndef _GAME_TEXTURE_H_
#define _GAME_TEXTURE_H_

#include "../cstmEngine/types.hpp"
#include <vector>

namespace game {
  // these are the blockTypes and have to be
  // indexed by the one that comes first going
  // from left to right and top to bottom.
  enum BlockType {
    TILE_CLOUD1 = 0, TILE_CLOUD2,
    TILE_CLOUD3, TILE_CLOUD4,
    TILE_GRASS, TILE_GRASS_SIDE,
    TILE_DIRT,TILE_BUSH1, TILE_BUSH2,
    TILE_CACTUS, TILE_SAND,
    TILE_WATER, TILE_WATER_TOP,
    TILE_BRICK, TILE_NITROGEN_BOX,
    TILE_BOX, TILE_QUESTION_BLOCK,
    TILE_FENCE, TILE_GRASS_TOP1,
    TILE_GRASS_TOP2, TILE_SLAB,
    TILE_COIN, TILE_WOODPILE,
    TILE_BOAT, TILE_PLAYER1,
    TILE_PLAYER2, TILE_PLAYER3,
    TILE_SUN, TILE_TREE1, TILE_TREE2,
    TILE_TREE3, TILE_TREE4, TILE_DOOR,
    _TILE_EMPTY
  };

  // gives us a vector v, full of texture coordinates of each
  // sprite in given textureAtlas.
  void textureGetCoordsIdxs(std::vector<cstmEngine::vec2> &v,
                            std::vector<cstmEngine::vec2> &quadSize,
                            cstmEngine::vec2 atlasWH, int texFactor);
}

#endif
