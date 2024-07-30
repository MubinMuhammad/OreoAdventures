#ifndef GAME_ENGINE_TEXTURE_HPP_
#define GAME_ENGINE_TEXTURE_HPP_

#include "../cstmEngine/types.hpp"
#include <vector>

namespace gameEngine {
  void textureCropAtlas(cstmEngine::vec2 *out_coord1, cstmEngine::vec2 *out_coord2,
                        cstmEngine::vec2 *out_coord3, cstmEngine::vec2 *out_coord4,
                        const cstmEngine::vec2 &atlas_wh, const int tex_factor,
                        const cstmEngine::vec2 &tex_wh, const cstmEngine::vec2 &pos_xy);

  void textureGetCoords(std::vector<cstmEngine::vec2> &texGrid, int idx,
                        cstmEngine::vec2 *outTexCoords);
}

#endif
