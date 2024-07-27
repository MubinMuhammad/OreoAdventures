// GLM Includes
#include "../glm/include/glm.hpp"
#include "../glm/include/gtc/matrix_transform.hpp"
#include "../glm/include/gtc/type_ptr.hpp"

// Graphics Engine Includes
#include "../cstmEngine/window.hpp"
#include "../cstmEngine/shader.hpp"
#include "../cstmEngine/texture.hpp"
#include "../cstmEngine/batch.hpp"

// Game Engine Includes
#include "../gameEngine/time.hpp"
#include "../gameEngine/physics.hpp"
#include "../gameEngine/font.hpp"

// Game Includes
#include "player.hpp"
#include "levels.hpp"
#include "texture.hpp"
#include "shaders.hpp"
#include "levels.hpp"
#include "playState.hpp"

// Other Includes
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <vector>
#include <string>

// we create the gameWindow as a global variable for the
// function below.
cstmEngine::Window gameWindow;

struct gameTileMap {
  std::vector<cstmEngine::vec2> maps;
  std::vector<cstmEngine::vec2> sizes;

  void load(const cstmEngine::TextureData &textureData) {
    game::textureGetCoordsIdxs(maps, sizes,
                               {(float)textureData.width, (float)textureData.height}, 8);
  }
};

struct gameCrntLevel {
  game::Level *p;
  int idx = 0;
};

struct gameState {
  const float tileSize = 60.0f;
  const uint32_t seed = time(NULL);
  gamePlayState playState = GAME_PLAY;
  cstmEngine::Batch mainBatch;
  cstmEngine::Batch uiBatch;
  cstmEngine::Shader mainShader;
  cstmEngine::Shader uiShader;
  cstmEngine::Texture textures;
  gameTileMap tileMap;
  gameEngine::Font font;
  game::Player player;
  std::vector<game::Level> levels;
  gameCrntLevel crntLevel;
  gameEngine::Time timer;
  glm::mat4 proj;
  glm::mat4 view;
};

void windowResizeCallback(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);

  gameWindow.m_width = w;
  gameWindow.m_height = h;
}

void renderGame(gameState &state, cstmEngine::vec2 halfWindowSize) {
    state.timer.update();

    state.player.updateInput(gameWindow, state.timer, state.tileSize);
    gameWindow.beginFrame(87/255.0f, 114/255.0f, 119/255.0f, 1.0f);

    state.mainBatch.beginFrame();
    state.mainShader.use();
    state.textures.use(0, "gameAtlas", &state.mainShader);

    state.proj = glm::ortho(-halfWindowSize.x, halfWindowSize.x, -halfWindowSize.y, halfWindowSize.y,
                            0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(state.mainShader.getShaderProgram(), "orthoProj"),
                       1, GL_FALSE, glm::value_ptr(state.proj));

    state.view = glm::translate(glm::mat4(1.0f),
                                glm::vec3(
                                std::clamp(-state.player.m_phy.m_pos.x,
                                           -(((float)state.crntLevel.p->m_length - 1) *state.tileSize - gameWindow.m_width), 0.0f),
                                std::clamp(-state.player.m_phy.m_pos.y, -500.0f, 0.0f), -1.0f));

    glUniformMatrix4fv(glGetUniformLocation(state.mainShader.getShaderProgram(), "view"),
                       1, GL_FALSE, glm::value_ptr(state.view));

    game::renderCloud(state.mainBatch, state.tileMap.maps, state.tileMap.sizes, state.tileSize,
                      0, 4, state.seed, state.crntLevel.p->m_length);

    state.crntLevel.p->renderLevel(state.mainBatch, state.player, state.playState,
                                   state.tileMap.maps, state.tileMap.sizes, state.tileSize,
                                   {(float)gameWindow.m_width, (float)gameWindow.m_height}, state.seed);

    state.player.render(state.mainBatch, state.tileMap.maps);
    state.mainBatch.endFrame();

    state.uiBatch.beginFrame();
    state.uiShader.use();
    state.textures.use(1, "fontAtlas", &state.uiShader);

    glUniformMatrix4fv(glGetUniformLocation(state.uiShader.getShaderProgram(), "orthoProj"),
                       1, GL_FALSE, glm::value_ptr(state.proj));

    state.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

    glUniformMatrix4fv(glGetUniformLocation(state.uiShader.getShaderProgram(), "view"),
                       1, GL_FALSE, glm::value_ptr(state.view));

    std::string uiStatusLineString = "%gPoints:" + std::to_string(state.player.levelState.m_points);
    uiStatusLineString += "%b Level:" + std::to_string(state.player.levelState.m_crntLevel + 1);
    std::string doorWarningMsg = "You need %apoints for next level!";

    if (state.player.levelState.m_doorMsg == true) {
      doorWarningMsg.insert(9, "%r" + std::to_string(state.crntLevel.p->m_points) + " ");
      state.font.renderCentered(state.uiBatch, doorWarningMsg, {0, 0}, 20);
    }

    if (state.crntLevel.p->m_passed) {
      if (state.crntLevel.idx == state.levels.size() - 1)
        state.playState = GAME_ENDSCREEN;

      state.player.levelState.m_points = 0;
      state.player.m_phy.resetPosition(halfWindowSize, state.tileSize, {20, 200});
      state.crntLevel.idx++;
      state.crntLevel.p = &state.levels[state.crntLevel.idx];
    }

    state.font.render(state.uiBatch, uiStatusLineString,
                      {-halfWindowSize.x + 9 + 10, halfWindowSize.y - 9 - 10}, 20);
    state.uiBatch.endFrame();
    gameWindow.endFrame();
}

void renderEndScreen(gameState &state, cstmEngine::vec2 halfWindowSize) {
  gameWindow.beginFrame(30/255.0f, 29/255.0f, 57/255.0f, 1.0f);
  state.uiBatch.beginFrame();

  state.proj = glm::ortho(-halfWindowSize.x, halfWindowSize.x, -halfWindowSize.y, halfWindowSize.y,
                          0.1f, 100.0f);

  glUniformMatrix4fv(glGetUniformLocation(state.mainShader.getShaderProgram(), "orthoProj"),
                     1, GL_FALSE, glm::value_ptr(state.proj));

  state.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

  glUniformMatrix4fv(glGetUniformLocation(state.uiShader.getShaderProgram(), "view"),
                     1, GL_FALSE, glm::value_ptr(state.view));

  if (glfwGetKey(gameWindow.m_window, GLFW_KEY_Q) == GLFW_PRESS)
    state.playState = GAME_CLOSE;

  if (glfwGetKey(gameWindow.m_window, GLFW_KEY_R) == GLFW_PRESS) {
    state.crntLevel.p = &state.levels[0];
    state.player.levelState.m_points = 0;
    state.player.m_viewSide = game::SIDE_RIGHT;
    state.player.m_phy.resetPosition(halfWindowSize, state.tileSize, {20, 200});
    state.playState = GAME_PLAY;
  }

  state.font.renderCentered(state.uiBatch, "%wgame %rover", {0, 20}, 32);
  state.font.renderCentered(state.uiBatch, "%wpress %g'r' %wto restart!", {0, -20}, 16);
  state.font.renderCentered(state.uiBatch, "%wpress %y'q' %wto quit!", {0, -40}, 16);

  state.uiBatch.endFrame();
  gameWindow.endFrame();
}

int main() {
  gameWindow.create(720, 480, "Mario Adventures");
  glfwSetWindowSizeCallback(gameWindow.m_window, windowResizeCallback);

  gameState state;
  state.mainShader.create(gameVertexShader, gameFragmentShader);
  state.uiShader.create(uiVertexShader, uiFragmentShader);

  stbi_set_flip_vertically_on_load(true);
  cstmEngine::TextureData gameTextureData;
  gameTextureData.data = stbi_load("./assets/gamePixelArt.png", &gameTextureData.width,
                                   &gameTextureData.height, &gameTextureData.color_channels, 0);

  state.textures.create(gameTextureData);
  state.tileMap.load(gameTextureData);
  state.font.create({(float)gameTextureData.width, (float)gameTextureData.height});

  state.mainBatch.create();
  state.uiBatch.create();

  state.player.init({state.tileSize, state.tileSize}, 20.0f, 0.1f);

  state.levels = game::levelRead({
    "./levels/level1.txt",
    "./levels/level2.txt",
    "./levels/level3.txt"
  });
  state.crntLevel.p = &state.levels[state.crntLevel.idx];

  while (gameWindow.isOpen()) {
    cstmEngine::vec2 halfWindowSize = {
      (float)gameWindow.m_width / 2,
      (float)gameWindow.m_height / 2
    };

    if (state.playState == GAME_PLAY)
      renderGame(state, halfWindowSize);
    else if (state.playState == GAME_ENDSCREEN)
      renderEndScreen(state, halfWindowSize);
    else if (state.playState == GAME_CLOSE)
        break;
  }

  state.mainBatch.destroy();
  state.uiBatch.destroy();
  state.textures.destroy();
  state.mainShader.destroy();
  gameWindow.destroy();
  return 0;
}
