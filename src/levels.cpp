#include "levels.hpp"
#include "playState.hpp"
#include "texture.hpp"
#include "utils.hpp"
#include "../gameEngine/texture.hpp"

#include <GLFW/glfw3.h>
#include <cmath>

#include <cstdlib>
#include <climits>
#include <string>
#include <vector>
#include <fstream>

// this function is used to get the
// level length.
//
// before we take a look at how
// the code below works, we first
// need to take a look at how the
// level RLE works.
//
// Imagine the following string:
// 1|8d9G2s5 3s11G22d6G6 16G1d\n1|8G30 13G8d1G28 1D
//
// At first, it make look like a bunch of gibberish.
// But, if you look closely you will see a it matches
// some part of the levels/level1.txt. Because, it's
// a part of the level1. It's just a compressed form
// of the first to rows of the of the level counting
// from the bottom.
//
// You may ask why we use the '|' symbol at the begining
// of each row. Well the simple answer is that sometimes
// the editor you're using may add tabs instead of
// spaces which can have \t format specifier to
// indicate a tab. And it will cause problems when
// trying to render the level. So, to avoid it we add |
// character so the editor doesn't add the tab.
// Maybe, editors like VScode will still at tabs anyway
// but it works fine with NEOVIM.
//
// The '\n' in after a section of the RLE indicates
// that we jump to the upper row. and then it begins
// to RLE that row. and puts a section for that.
//
// To see how, understand the `src/rle.cpp` file.
static uint32_t levelGetLength(const std::string &levelRle) {
  // the out will contain the result which is the
  // length of the level.
  int out = -INT_MAX;

  // here ns is the part where the RLE
  // string has the number and not the chracter.
  // for example 98d here ns will contain 98.
  std::string ns = "";

  // the following loop takes RLE string and moves
  // through each character.
  for (int i = 0, n = 0; i < levelRle.size(); i++) {
    // if following character is '\n'
    // that means reading the current row
    // has finished.
    if (levelRle[i] == '\n') {
      // therefore, set out to be the maximum.
      // of all Ns.
      out = std::max(n, out);
      // setting n to 0 is really important.
      // Otherwise n will accumulate after each
      // iteration and will give us wrong length.
      n = 0;

      // then we continue to the next character
      // and void going running the statements
      // below.
      continue;
    }

    // this state makes sure the currenet character
    // it's checking is in integer set and not an alphabet.
    if (levelRle[i] >= '0' && levelRle[i] <= '9') {
      // if so we append it to the ns variable
      ns.push_back(levelRle[i]);
      // the continue method will go for the
      // next character in the RLE string and
      // not run anything under this if statement
      // if the character is between 0 and 9
      continue;
    }

    // now the current character must not
    // be in between 0 and 9 because we had a
    // condition for it above. Therefore,
    // we get the number version of ns with
    // std::stoi() and add it to n.
    n += std::stoi(ns);

    // after that we will empty ns.
    ns = "";
  }

  // the typecast here is not really necessary.
  // but it's here to to convert out to uint32_t
  // as it was an int (in theory).
  //
  // It's here because length of the level
  // can't be negative.
  return (uint32_t)out;
}

static std::string getRle(const std::string &raw) {
  if (raw.empty()) return "";

  int count = 1;
  char crntChar = raw[0];
  std::string rle = "";

  for (int i = 1; i < raw.size(); i++) {
    if (raw[i] != crntChar) {
      rle.append(std::to_string(count) + crntChar);
      count = 1;
      crntChar = raw[i];
      continue;
    }

    count++;
  }

  rle.append(std::to_string(count) + crntChar);

  return rle;
}

std::vector<game::Level> game::levelRead(std::vector<std::string> levelPaths) {
  std::vector<game::Level> levels(levelPaths.size());
  int crntLevelIdx = 0;

  for (std::string crntLevelPath : levelPaths) {
    std::ifstream crntLevelMap(crntLevelPath);

    if (!crntLevelMap.is_open()) continue;

    std::string line = "";
    std::getline(crntLevelMap, line);

    std::vector<std::string> tokens = game::tokenize(line, ',');

    levels[crntLevelIdx].m_points = std::stoi(tokens[0]);
    levels[crntLevelIdx].m_time = std::stoi(tokens[1]);

    while (std::getline(crntLevelMap, line)) {
      if (line.empty()) continue;
      std::string crntRowRle = getRle(line);
      crntRowRle.push_back('\n');
      levels[crntLevelIdx].m_rle.insert(0, crntRowRle);
    }
    std::cout << levels[crntLevelIdx].m_rle << std::endl;

    levels[crntLevelIdx].m_length = levelGetLength(levels[crntLevelIdx].m_rle);
    crntLevelIdx++;
  }

  return levels;
}

// renders the level in the game.

void game::Level::renderLevel(cstmEngine::Batch &batch, game::Player &player,
                              gamePlayState &playState, std::vector<cstmEngine::vec2> &textureGrid,
                              std::vector<cstmEngine::vec2> &quadSizes, int tileSize,
                              cstmEngine::vec2 windowSize, uint32_t gameSeed) {
  // a random seed is used here so that tree and bush
  // types are random.
  srand(gameSeed);

  // the tile position for the level is currently
  // set to bottom left. As the rle also starts
  // from the bottom left.
  cstmEngine::vec2 tilePos = {
    -windowSize.x / 2.0f + tileSize / 2.0f,
    -windowSize.y / 2.0f + tileSize / 2.0f
  };

  // tile offset is sometimes needed when we are
  // going to calculate the positon for trees
  // and bushes as there side lengths are more
  // than 1 (considering the factor of it).
  cstmEngine::vec2 tileOffset = {0, 0};

  // coin idx is used for cointing the coins
  // that are in a level and indexing them
  // by which one comes first.
  int coinIdx = 0;

  // rleNum is like n.
  int rleNum = 0;
  // rleNumStr is like ns.
  std::string rleNumStr = "";
  // bt is the block type that needs to be placed in a
  // certain position.
  game::BlockType bt;

  // door msg is set to false.
  // If the player touches the the door
  // it will check for the points and if
  // that check if not passed then it will
  // be true and the player will be texted with
  // the warining about the not having enough
  // coins.
  player.levelState.m_doorMsg = false;

  // here is a for each loop to go through is character
  // of the RLE string.
  for (char c : m_rle) {
    // if the character is '\n'
    // that means reading the 
    // current row has finished.
    if (c == '\n') {
      // we re-set the x coordinate of the tile
      // to be again to the left.
      tilePos.x = -windowSize.x / 2.0f + tileSize / 2.0f;
      // we increase the y level by the tile size.
      tilePos.y += tileSize;
      continue;
    }

    if (!(c >= '0' && c <= '9')) {
      // here rleNumStr will be converted
      // into and integer and put into the
      // rleNum variable.
      rleNum = std::stoi(rleNumStr);
      // we set the rleNumStr variable to be ""
      // so that it doesn't accumulate more digits
      // and potentially cause a integer overflow.
      // (that did happend)
      rleNumStr = "";

      // we add a switch on the current character
      switch (c) {
        // if the character is a '|' we just
        // ignore it as it's not part of the level.
        case '|':
          continue;
        // if we find space it just means we just
        // have to increase x by tileSize cuple of times
        // and that is going to happend for rleNum times
        // as the rle contains the the repetition of a character
        // in a row.
        case ' ':
          tilePos.x += rleNum * tileSize;
          continue;
        // if the character is a 'G' we say that the block type
        // is a grass. Others are self-explanitory
        case 'G':
          bt = TILE_GRASS;
          break;
        case 'd':
          bt = TILE_DIRT;
          break;
        case 'T':
          // if we find a tree though, we need to select a random three
          // from the three trees we have here we do use the rand() function
          // to generate a random number. Because the random number might be
          // greater than the number of trees we have we do a mod on it to
          // make it in range of 0 and 2. Now add it to TILE_TREE1 as it's
          // an enum and enum is just a value and if you take look at
          // src/texture.hpp you will see TILE_TREE1, TILE_TREE2, TILE_TREE3
          // and TILE_TREE4 are one after another, therefore the enum value
          // increase by one.
          bt = (game::BlockType)(TILE_TREE1 + rand() % 4);
          // we deal the with x coordinate of the tile as if we don't
          // we will see the position gets half of it way to x coordinate.
          // So, I thought of this solution (Try to think by yourself).
          tileOffset = {0.5f * ((int)quadSizes[bt].x - 1), 0.5f * ((int)quadSizes[bt].y - 1)};
          break;
        case 'B':
          // same thing we did with tree but now with bushes.
          bt = (game::BlockType)(TILE_BUSH1 + rand() % 2);
          // fixing tile issue.
          tileOffset = {0.5f, 0.0f};
          break;
        case '?':
          bt = TILE_QUESTION_BLOCK;
          break;
        case 'C':
          bt = TILE_COIN;
          break;
        case 'D':
          bt = TILE_DOOR;
          tileOffset = {0.5f * ((int)quadSizes[bt].x - 1), 0.5f * ((int)quadSizes[bt].y - 1)};
          break;
        case 'W':
          bt = TILE_WOODPILE;
          break;
        case 'w':
          bt = TILE_WATER;
          break;
        case 's':
          bt = TILE_SAND;
          break;
        case '_':
          bt = TILE_SLAB;
          break;
        case 'b':
          bt = TILE_BOX;
          break;
        case 'f':
          bt = TILE_FENCE;
          break;
        case 'N':
          bt = TILE_NITROGEN_BOX;
          break;
        case 'a':
          bt = TILE_BRICK;
          break;
        default:
          bt = TILE_QUESTION_BLOCK;
          break;
      }

      game::BlockType btCpy = bt;

      for (int i = 0; i < rleNum; i++) {
        // if the the block type is a COIN
        // and the at coinIdx-th bit is 0
        // we know that player has taken the
        // coin already. So, we set the block
        // type to _TILE_EMPTY which will not
        // be rendered as any texture.
        if (bt == TILE_COIN) {
          if (!(m_coinState & (1 << coinIdx))) {
            bt = _TILE_EMPTY;
          }
          tileOffset.y = fabs(sin(glfwGetTime()));
        }

        // if a collision did happend with the player
        // and the it was a coin. Then...
        if (player.m_phy.checkCollision({tilePos.x + (tileOffset.x * tileSize),
                                         tilePos.y + (tileOffset.y * tileSize)},
                                        {quadSizes[bt].x, quadSizes[bt].y}, tileSize) &&
            bt == TILE_COIN) {
          // we set the coinIdx-th bit to 0.
          m_coinState &= ~(1 << coinIdx);
          // we also add a point so the player can
          // can see he is getting something.
          player.levelState.m_points += 10;
        }

        // if a collision did happend with the player
        // and the it was a door. Then...
        if (player.m_phy.checkCollision({tilePos.x + (tileOffset.x * tileSize),
                                         tilePos.y + (tileOffset.y * tileSize)},
                                        {quadSizes[bt].x, quadSizes[bt].y}, tileSize) &&
            bt == TILE_DOOR) {
          // __builtin_popcountll returns how many bits are on
          // in a 64bit unsigned integer. By doing a (64 - that) we now
          // know how many bit's are all in m_coinState, we multiply
          // that by 10 to get player's score and check if it less than
          // the points needed to get him to the next level.
          // 
          // if so, print the doorMsg otherwise set m_passed to true.
          if (m_points > ((64 - __builtin_popcountll(m_coinState)) * 10)) player.levelState.m_doorMsg = true;
          else if (player.levelState.m_passed == false) player.levelState.m_passed = true;
        }

        if (player.m_phy.checkCollision({tilePos.x + (tileOffset.x * tileSize),
                                         tilePos.y + (tileOffset.y * tileSize)},
                                        {quadSizes[bt].x, quadSizes[bt].y}, tileSize) &&
            bt == TILE_NITROGEN_BOX) {
          playState = GAME_ENDSCREEN;
        }

        // now, if the bt is not _TILE_EMPTY we shall render that tile!
        if (bt != _TILE_EMPTY) {
          renderTile(batch, textureGrid, {quadSizes[bt].x * tileSize, quadSizes[bt].y * tileSize},
                    {tilePos.x + (tileOffset.x * tileSize), tilePos.y + (tileOffset.y * tileSize)}, bt);
        }

        // we increase the tileX by tileSize each tile
        // we render a single tile.
        tilePos.x += tileSize;
        // this is just because in the next iteration we
        // have to still go through and if bt
        // is a coin and check coinIdx-th bit is on or off.
        if (bt == _TILE_EMPTY) bt = btCpy;
        // increase the coinIdx if it is a coin.
        if (bt == TILE_COIN) coinIdx++;
      }

      // set tile offset to 0 each time,
      // NOTE the level renders each frame so,
      // this function runs multiple times.
      tileOffset = {0, 0};
      continue;
    }

    // add next digit to rleNumStr.
    rleNumStr += c;
  }
}

// Render tile function renders a single tile
// on the screen. This function is called multiple
// times in renderLevel function to render multiple
// tile and create a level.
void game::Level::renderTile(cstmEngine::Batch &batch, std::vector<cstmEngine::vec2> &textureGrid,
                             cstmEngine::vec2 tileSize, cstmEngine::vec2 coord, game::BlockType bt) {
  // texCoords is a has 4 coordinates, these are top-left, bottom-left,
  // bottom-right and top-right. these coordinates are later used with
  // OpenGL to draw a part of the texture atlas.
  cstmEngine::vec2 texCoords[4];
  // this function get the needed texture coordiates from
  // the atlas. It outputs the coordinates depending what
  // blockType(bt) you pass in.
  gameEngine::textureGetCoords(textureGrid, bt, texCoords);

  // The batch then draws the generates correct vertices to draw
  // a tile.
  batch.drawQuad({tileSize.x, tileSize.y}, {coord.x, coord.y}, texCoords);
}

// render cloud function is used for generating cloud at
// a certain level height.
void game::renderCloud(cstmEngine::Batch &batch, std::vector<cstmEngine::vec2> &textureGrid,
                       std::vector<cstmEngine::vec2> &quadSizes, int tileSize,
                       int cloudHeight, uint32_t variability, uint32_t seed,
                       uint32_t levelLength) {
  // Here, we set the seed that's used create randomness
  // in the positions and type of the clouds.
  srand(seed);

  for (int i = 0; i <= levelLength; i += 5) {
    // this is a simple equation to generate random heights
    // for the clouds. The rand() will generate the random
    // numbers between 0 and RAND_MAX. but for our porposes
    // we will mod it by variability to get random numbers
    // between 0 and varibility - 1. Then to add the height
    // to it we add cloudHeight / 2 to it.
    int y = ((rand() % variability) + cloudHeight);

    // the block type is got in the same way the trees get
    // the random generation.
    game::BlockType bt = (game::BlockType)(TILE_CLOUD1 + rand() % 4);

    // this is same as we have done in renderTile.
    cstmEngine::vec2 texCoords[4];
    gameEngine::textureGetCoords(textureGrid, bt, texCoords);

    // here we get the size of the cloud by multiplying
    // there factors with tileSize and then doubling the size
    // to make it larger compared to everything else.
    cstmEngine::vec2 cloudSize = {
      quadSizes[bt].x * tileSize,
      quadSizes[bt].y * tileSize
    };

    // this just generates required vertices to render a
    // cloud.
    batch.drawQuad(cloudSize, {(float)i * tileSize, (float)y * tileSize}, texCoords);
  }
}

//
// I did abbreviate some parts here and
// there, so if you'd like to know how these all
// things work, you would have to google them or
// learn more by doing some research.
//
