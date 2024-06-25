#include "levels.hpp"
#include "texture.hpp"
#include "../gameEngine/texture.hpp"

#include <cstdlib>
#include <string>

void game::Level::loadLevel(std::string levelRle, int levelPoints) {
  m_levelRle = levelRle;
  m_levelPoints = levelPoints;
}

void game::Level::renderLevel(
  cstmEngine::Batch &batch,
  game::Player player,
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
          bt = (game::BlockType)(SQR_TREE1 + rand() % 4);
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
        /*if (*/
        /*  player.m_phy.checkCollision(*/
        /*    {tilePos.x + (offset.x * tileSize),*/
        /*     tilePos.y + (offset.y * tileSize)},*/
        /*    {quadSizes[bt].x, quadSizes[bt].y},*/
        /*    tileSize*/
        /*  )*/
        /*) {*/
        /*  player.resolveCollision(*/
        /*    {tilePos.x + (offset.x * tileSize),*/
        /*     tilePos.y + (offset.y * tileSize)},*/
        /*    {quadSizes[bt].x, quadSizes[bt].y},*/
        /*    tileSize*/
        /*  );*/
        /*}*/

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
  gameEngine::textureGetCoords(textureGrid, bt, texCoords);

  batch.drawQuadT(
    {tileSize.x, tileSize.y},
    {coord.x, coord.y},
    texCoords
  );
}

void game::renderCloud(
  cstmEngine::Batch &batch,
  std::vector<cstmEngine::vec2> &textureGrid,
  std::vector<cstmEngine::vec2> &quadSizes,
  int tileSize,
  int cloudHeight,
  uint32_t variability,
  uint32_t seed
) {
  srand(seed);

  for (int i = -4; i <= 128; i += (rand() % 6) + 1) {
    int y = rand() % ((variability * 2) + (cloudHeight - variability));

    game::BlockType bt = (game::BlockType)(SQR_CLOUD1 + rand() % 4);

    cstmEngine::vec2 texCoords[4];
    gameEngine::textureGetCoords(textureGrid, bt, texCoords);

    cstmEngine::vec2 cloudSize = {
      quadSizes[bt].x * tileSize * 2,
      quadSizes[bt].y * tileSize * 2
    };

    batch.drawQuadT(cloudSize, {(float)i * tileSize, (float)y * tileSize}, texCoords);
  }
}
