#ifndef _GAME_TEXTURE_H_
#define _GAME_TEXTURE_H_

#include "../cstmEngine/types.hpp"
#include <vector>

namespace game {
  enum BlockType {
    SQR_CLOUD1 = 0, SQR_CLOUD2,
    SQR_CLOUD3, SQR_CLOUD4,
    SQR_COIN, SQR_GRASS,
    SQR_DIRT, SQR_HEART1,
    SQR_HEART2, SQR_HEART3,
    SQR_QUESTION_BLOCK, SQR_FENCE,
    SQR_WOODPILE, SQR_CACTUS,
    SQR_SAND, SQR_DOOR,
    SQR_PIPE, SQR_PIPE_HEAD,
    SQR_SLAB, SQR_PLAYER1,
    SQR_PLAYER2, SQR_WATER,
    SQR_BRICK, SQR_BUSH1,
    SQR_BUSH2, SQR_BUSH3,
    SQR_NITROGEN_BOX, SQR_BOX,
    SQR_BANGLADESH_FLAG, SQR_UNKNOWN,
    SQR_TREE1 = 41, SQR_TREE2,
    SQR_TREE3, SQR_TREE4, _SQR_EMPTY
  };

  void textureGetCoordsIdxs(
    std::vector<cstmEngine::vec2> &v,
    std::vector<cstmEngine::vec2> &quadSize,
    cstmEngine::vec2 atlasWH,
    int texFactor
  );
}

#endif
