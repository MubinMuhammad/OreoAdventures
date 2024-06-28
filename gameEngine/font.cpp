#include "font.hpp"
#include "texture.hpp"

static std::string charMap =
"ABCDEFGHIJKLMNOP\n"
"QRSTUVWXYZ012345\n"
"6789~!@#$%^&*()-\n"
"+=[]{}|;:'\"<>?/,";

static void charToUpper(char &c) {
  if (c >= 'a' && c <= 'z') {
    c -= 'a' - 'A';
  }
}

static void getCharIdx(char c, uint32_t &outIdx) {
  outIdx = 0;
  charToUpper(c);

  for (char i : charMap) {
    if (i == '\n') continue;
    if (i == c) break;
    outIdx++;
  }
}

void gameEngine::Font::create(cstmEngine::vec2 fontAtlasSize) {
  fontAtlasCoords.resize(16 * 4 * 4);
  int x = 0, y = 0;

  for (int i = 0, k = 0; i < charMap.size(); i++) {
    gameEngine::textureCropAtlas(
      &fontAtlasCoords[k], &fontAtlasCoords[k + 1],
      &fontAtlasCoords[k + 2], &fontAtlasCoords[k + 3],
      fontAtlasSize, 8, {1, 1}, {(float)x, (float)y}
    );

    x++;
    if (charMap[i] == '\n') {
      y++;
      x = 0;
      continue;
    }
    k += 4;
  }
}

void gameEngine::Font::render(
  cstmEngine::Batch &batch,
  std::string str,
  cstmEngine::vec2 textPos,
  uint32_t fontSize
) {
  for (char c : str) {
    if (c == ' ') {
      textPos.x += fontSize;
      continue;
    }

    uint32_t k;
    getCharIdx(c, k);

    cstmEngine::vec2 charTexCoords[4];
    gameEngine::textureGetCoords(fontAtlasCoords, k, charTexCoords);

    batch.drawQuadT(
      {(float)fontSize, (float)fontSize},
      textPos, charTexCoords
    );

    textPos.x += fontSize;
  }
}

void gameEngine::Font::renderCentered(
  cstmEngine::Batch &batch,
  std::string str,
  cstmEngine::vec2 textPos,
  uint32_t fontSize
) {
  int halfTextLength = (str.size() * fontSize) / 2;
  textPos.x -= halfTextLength;

  for (char c : str) {
    if (c == ' ') {
      textPos.x += fontSize;
      continue;
    }

    uint32_t k;
    getCharIdx(c, k);

    cstmEngine::vec2 charTexCoords[4];
    gameEngine::textureGetCoords(fontAtlasCoords, k, charTexCoords);

    batch.drawQuadT(
      {(float)fontSize, (float)fontSize},
      textPos, charTexCoords
    );

    textPos.x += fontSize;
  }
}
