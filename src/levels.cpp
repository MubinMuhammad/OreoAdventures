#include "levels.hpp"
#include "texture.hpp"
#include "../gameEngine/texture.hpp"

#include <cstdlib>
#include <climits>
#include <string>

uint32_t levelGetLength(const std::string &levelRle) {
  int out = -INT_MAX;

  std::string ns = "";
  for (int i = 0, n = 0; i < levelRle.size(); i++) {
    if (levelRle[i] == '\n') {
      out = std::max(n, out);
      n = 0;
      continue;
    }

    if (levelRle[i] >= '0' && levelRle[i] <= '9') {
      ns.push_back(levelRle[i]);
      continue;
    }

    n += std::stoi(ns);
    ns = "";
  }

  return (uint32_t)out;
}

void game::Level::loadLevel(std::string levelRle, int levelPoints) {
  m_levelRle = levelRle;
  m_levelPoints = levelPoints;
  m_levelLength = levelGetLength(levelRle);
}

void game::Level::renderLevel(
  cstmEngine::Batch &batch,
  game::Player player,
  PlayerLevelState &playerState,
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

  cstmEngine::vec2 tileOffset = {0, 0};

  int coinIdx = 0;

  int rleNum = 0;
  std::string rleNumStr = "";
  game::BlockType bt;

  playerState.doorMsg = false;

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
          bt = (game::BlockType)(SQR_TREE1 + rand() % 3);
          tileOffset = {0.5f * ((int)quadSizes[bt].x - 1), 0.5f * ((int)quadSizes[bt].y - 1)};
          break;
        case 'B':
          bt = (game::BlockType)(SQR_BUSH1 + rand() % 2);
          tileOffset = {0.5f, 0.0f};
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
        case 's':
          bt = SQR_SAND;
          break;
        case '_':
          bt = SQR_SLAB;
          break;
        case 'b':
          bt = SQR_BOX;
          break;
        case 'f':
          bt = SQR_FENCE;
          break;
        case 'F':
          bt = SQR_BANGLADESH_FLAG;
          break;
        default:
          bt = SQR_QUESTION_BLOCK;
          break;
      }

      game::BlockType btCpy = bt;

      for (int i = 0; i < rleNum; i++) {
        if (bt == SQR_COIN) {
          if (!(coinState & (1 << coinIdx))) {
            bt = _SQR_EMPTY;
          }
        }

        if (
          player.m_phy.checkCollision(
            {tilePos.x + (tileOffset.x * tileSize),
             tilePos.y + (tileOffset.y * tileSize)},
            {quadSizes[bt].x, quadSizes[bt].y},
            tileSize
          ) && 
          bt == SQR_COIN
        ) {
          coinState &= ~(1 << coinIdx);
          playerState.points += 10;
        }

        if (
          player.m_phy.checkCollision(
            {tilePos.x + (tileOffset.x * tileSize),
             tilePos.y + (tileOffset.y * tileSize)},
            {quadSizes[bt].x, quadSizes[bt].y},
            tileSize
          ) && 
          bt == SQR_DOOR
        ) {
          if ((m_levelPoints - ((64 - __builtin_popcountll(coinState)) * 10)) > 0)
            playerState.doorMsg = true;
          else if (levelPassed == false) {
            playerState.crntLevel++;
            levelPassed = true;
          }
        }


        if (bt != _SQR_EMPTY || (coinState & (1 << coinIdx))) {
          renderTile(
            batch,
            textureGrid,
            { quadSizes[bt].x * tileSize, quadSizes[bt].y * tileSize },
            { tilePos.x + (tileOffset.x * tileSize), tilePos.y + (tileOffset.y * tileSize) },
            bt
          );
        }

        tilePos.x += tileSize;
        if (bt == _SQR_EMPTY) bt = btCpy;
        if (bt == SQR_COIN) coinIdx++;
      }

      tileOffset = {0, 0};
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

  batch.drawQuad(
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
  uint32_t seed,
  uint32_t levelLength
) {
  srand(seed);

  for (int i = -4; i <= levelLength; i += (rand() % 6) + 3) {
    int y = rand() % ((variability * 2) + (cloudHeight - variability));

    game::BlockType bt = (game::BlockType)(SQR_CLOUD1 + rand() % 4);

    cstmEngine::vec2 texCoords[4];
    gameEngine::textureGetCoords(textureGrid, bt, texCoords);

    cstmEngine::vec2 cloudSize = {
      quadSizes[bt].x * tileSize * 2,
      quadSizes[bt].y * tileSize * 2
    };

    batch.drawQuad(cloudSize, {(float)i * tileSize, (float)y * tileSize}, texCoords);
  }
}
