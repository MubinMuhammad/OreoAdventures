#ifndef GAME_ENGINE_FONT_HPP_
#define GAME_ENGINE_FONT_HPP_

#include "../cstmEngine/types.hpp"
#include "../cstmEngine/batch.hpp"

#include <vector>
#include <string>
#include <cstdint>

namespace game {
  struct Font {
    std::vector<cstmEngine::vec2> fontAtlasCoords;
    
    void create(cstmEngine::vec2 fontAtlasSize);
    void render(
      cstmEngine::Batch &batch,
      std::string &str,
      cstmEngine::vec2 textPos,
      uint32_t fontSize
    );
  };
}

#endif
