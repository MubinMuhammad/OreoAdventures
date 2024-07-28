#include "font.hpp"
#include "texture.hpp"
#include <cstdio>

static std::string charMap =
"ABCDEFGHIJKLMNOP\n"
"QRSTUVWXYZ012345\n"
"6789~!@#$%^&*()-\n"
"+=[],.|;:'\"<>?/\\";

static constexpr cstmEngine::vec3 fontColors[] = {
  { 21 / 255.0f,  29 / 255.0f,  40 / 255.0f}, // black
  {199 / 255.0f, 207 / 255.0f, 204 / 255.0f}, // white
  {207 / 255.0f,  87 / 255.0f,  60 / 255.0f}, // red
  {208 / 255.0f, 218 / 255.0f, 145 / 255.0f}, // green
  {115 / 255.0f, 190 / 255.0f, 211 / 255.0f}, // blue
  {232 / 255.0f, 193 / 255.0f, 112 / 255.0f}, // yellow
  {218 / 255.0f, 134 / 255.0f,  62 / 255.0f}, // orange
  {164 / 255.0f, 221 / 255.0f, 219 / 255.0f}, // cyan
  {223 / 255.0f, 132 / 255.0f, 165 / 255.0f}  // pink
};

static void charToUpper(char &c) {
  if (c >= 'a' && c <= 'z')
    c -= 'a' - 'A';
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

static int getColorStrSize(std::string s) {
  int len = 0;

  for (int i = 0; i < s.size(); i++) {
    len++;
    if (i != s.size() - 1 && s[i] == '%') {
      char c = s[i + 1];

      if (
        c == 'a' || c == 'w' || c == 'r' || c == 'g' || c == 'b' ||
        c == 'y' || c == 'o' || c == 'c' || c == 'p'
      ) i += 2;
    }
  }

  return len;
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
  cstmEngine::vec3 color = fontColors[0];

  for (int i = 0; i < str.size(); i++) {
    if (str[i] == ' ') {
      textPos.x += fontSize;
      continue;
    }

    if (i != str.size() - 1 && str[i] == '%') {
      switch (str[i + 1]) {
        case 'a': color = fontColors[0]; i++; continue;
        case 'w': color = fontColors[1]; i++; continue;
        case 'r': color = fontColors[2]; i++; continue;
        case 'g': color = fontColors[3]; i++; continue;
        case 'b': color = fontColors[4]; i++; continue;
        case 'y': color = fontColors[5]; i++; continue;
        case 'o': color = fontColors[6]; i++; continue;
        case 'c': color = fontColors[7]; i++; continue;
        case 'p': color = fontColors[8]; i++; continue;
        default: break;
      }
    }

    uint32_t k;
    getCharIdx(str[i], k);

    cstmEngine::vec2 charTexCoords[4];
    gameEngine::textureGetCoords(fontAtlasCoords, k, charTexCoords);

    batch.drawQuad(
      {(float)fontSize, (float)fontSize},
      textPos, charTexCoords, color
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
  cstmEngine::vec3 color = fontColors[0];

  int halfTextLength = (getColorStrSize(str) * fontSize) / 2;
  textPos.x -= halfTextLength;
  textPos.y -= (float)fontSize / 2;

  Font::render(batch, str, textPos, fontSize);
}
