#include "levels.hpp"
#include "texture.hpp"

#include <iostream>
#include <string>

void game::Level::loadLevel(std::string levelRle, int levelPoints) {
  m_levelRle = levelRle;
  m_levelPoints = levelPoints;
}

void game::Level::renderLevel(
  cstmEngine::Batch &batch,
  std::vector<cstmEngine::vec2> &textureGrid,
  std::vector<cstmEngine::vec2> &quadSizes,
  int tileSize,
  cstmEngine::vec2 windowSize,
  uint32_t gameSeed
) {
  srand(gameSeed);

  cstmEngine::vec2 tilePos = {
    -windowSize.x / 2.0f + tileSize / 2.0f,
    -windowSize.y / 2.0f + tileSize / 2.0f
  };

  cstmEngine::vec2 offset = {0, 0};

  int rleNum = 0;
  std::string rleNumStr = "";
  game::BlockType bt;

  for (char c : m_levelRle) {
    if (c == '\n') {
      tilePos.x = -windowSize.x / 2.0f + tileSize / 2.0f;
      tilePos.y += tileSize;
      continue;
    }

    if (!(c >= '0' && c <= '9')) {
      rleNum = std::stoi(rleNumStr);
      rleNumStr = "";

      switch (c) {
        case '|':
          continue;
        case ' ':
          tilePos.x += rleNum * tileSize;
          continue;
        case 'G':
          bt = SQR_GRASS;
          break;
        case 'd':
          bt = SQR_DIRT;
          break;
        case 'T':
          bt = (game::BlockType)(SQR_TREE1/* + rand() % 4*/);
          offset = {0.5, 0.5};
          break;
        case 'B':
          bt = (game::BlockType)(SQR_BUSH1 + rand() % 3);
          offset = {0.5, 0.0};
          break;
        case '?':
          bt = SQR_QUESTION_BLOCK;
          break;
        case 'C':
          bt = SQR_COIN;
          break;
        case 'D':
          bt = SQR_DOOR;
          break;
        case 'W':
          bt = SQR_WOODPILE;
          break;
        case 'w':
          bt = SQR_WATER;
          break;
        case 'S':
          bt = SQR_SLAB;
          break;
        case 's':
          bt = SQR_SAND;
          break;
        case '_':
          bt = SQR_SLAB;
          break;
        case 'U':
          bt = SQR_SLAB;
          break;
        case 'b':
          bt = SQR_BOX;
          break;
        case 'f':
          bt = SQR_FENCE;
          break;
        default:
          bt = SQR_QUESTION_BLOCK;
          break;
      }

      for (int i = 0; i < rleNum; i++) {
        renderTile(
          batch,
          textureGrid,
          {
            quadSizes[bt].x * tileSize,
            quadSizes[bt].y * tileSize
          },
          {
            tilePos.x + (offset.x * tileSize),
            tilePos.y + (offset.y * tileSize)
          },
          bt
        );
        tilePos.x += tileSize;
      }

      offset = {0, 0};
      continue;
    }

    rleNumStr += c;
  }
}

void game::Level::renderTile(
  cstmEngine::Batch &batch,
  std::vector<cstmEngine::vec2> &textureGrid,
  cstmEngine::vec2 tileSize,
  cstmEngine::vec2 coord,
  game::BlockType bt
) {
  cstmEngine::vec2 texCoords[4];
  game::textureGetCoords(textureGrid, bt, texCoords);

  batch.drawQuadT(
    {tileSize.x, tileSize.y},
    {coord.x, coord.y},
    texCoords
  );
}
