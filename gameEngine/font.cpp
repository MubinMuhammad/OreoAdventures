#include "font.hpp"
#include "texture.hpp"

static std::string charMap =
"ABCDEFGHIJKLMNOP\n"
"QRSTUVWXYZ012345\n"
"6789~!@#$%^&*()-\n"
"+=[]{}|;:'\"<>?/,";

static constexpr cstmEngine::vec3 fontColors[] = {
  {199 / 255.0f, 207 / 255.0f, 204 / 255.0f}, // white
  {165 / 255.0f,  48 / 255.0f,  48 / 255.0f}, // red
  {117 / 255.0f, 167 / 255.0f,  67 / 255.0f}, // green
  { 79 / 255.0f, 143 / 255.0f, 186 / 255.0f}, // blue
  {222 / 255.0f, 158 / 255.0f,  65 / 255.0f}, // yellow
  {218 / 255.0f, 134 / 255.0f,  62 / 255.0f}, // orange
  {164 / 255.0f, 221 / 255.0f, 219 / 255.0f}, // cyan
  {198 / 255.0f,  81 / 255.0f, 151 / 255.0f}  // pink
};

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

static int getColorStrSize(std::string s) {
  int len = 0;

  for (int i = 0; i < s.size(); i++) {
    len++;
    if (i != s.size() - 1 && s[i] == '%') {
      char c = s[i + 1];

      if (
        c == 'w' || c == 'r' || c == 'g' || c == 'b' ||
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
        case 'w':
          color = fontColors[0]; break;
        case 'r':
          color = fontColors[1]; break;
        case 'g':
          color = fontColors[2]; break;
        case 'b':
          color = fontColors[3]; break;
        case 'y':
          color = fontColors[4]; break;
        case 'o':
          color = fontColors[5]; break;
        case 'c':
          color = fontColors[6]; break;
        case 'p':
          color = fontColors[7]; break;
        default:
          i -= 2; break;
      }
      i += 2;
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

  for (int i = 0; i < str.size(); i++) {
    if (str[i] == ' ') {
      textPos.x += fontSize;
      continue;
    }

    if (i != str.size() - 1 && str[i] == '%') {
      switch (str[i + 1]) {
        case 'w':
          color = fontColors[0]; break;
        case 'r':
          color = fontColors[1]; break;
        case 'g':
          color = fontColors[2]; break;
        case 'b':
          color = fontColors[3]; break;
        case 'y':
          color = fontColors[4]; break;
        case 'o':
          color = fontColors[5]; break;
        case 'c':
          color = fontColors[6]; break;
        case 'p':
          color = fontColors[7]; break;
        default:
          i -= 2; break;
      }
      i += 2;
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
