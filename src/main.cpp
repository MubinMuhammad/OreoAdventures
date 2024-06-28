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
#include "levelsRle.hpp"
#include "levels.hpp"

// Other Includes
#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <vector>
#include <string>

cstmEngine::Window gameWindow;

void windowResizeCallback(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);

  gameWindow.m_width = w;
  gameWindow.m_height = h;
}

int main() {
  const float GAME_TILE_SIZE = 60;
  const uint32_t GAME_SEED = time(NULL);

  gameWindow.create(720, 480, "Mario Adventures");
  glfwSetWindowSizeCallback(gameWindow.m_window, windowResizeCallback);

  cstmEngine::Shader gameShader;
  gameShader.create(gameVertexShader, gameFragmentShader);

  cstmEngine::Shader uiShader;
  uiShader.create(uiVertexShader, uiFragmentShader);

  stbi_set_flip_vertically_on_load(true);

  cstmEngine::TextureData gameAtlasData;
  gameAtlasData.data = stbi_load(
    "./assets/GamePixelArt.png",
    &gameAtlasData.width,
    &gameAtlasData.height,
    &gameAtlasData.color_channels,
    0
  );

  cstmEngine::Texture gameAtlas;
  gameAtlas.create(gameAtlasData);

  cstmEngine::vec2 gameAtlasWH = {
    (float)gameAtlasData.width,
    (float)gameAtlasData.height
  };

  std::vector<cstmEngine::vec2> gameAtlasGrid;
  std::vector<cstmEngine::vec2> gameQuadSizes;
  game::textureGetCoordsIdxs(gameAtlasGrid, gameQuadSizes, gameAtlasWH, 32);

  cstmEngine::TextureData fontAtlasData;
  fontAtlasData.data = stbi_load(
    "./assets/font.png",
    &fontAtlasData.width,
    &fontAtlasData.height,
    &fontAtlasData.color_channels,
    0
  );

  cstmEngine::Texture fontAtlas;
  fontAtlas.create(fontAtlasData);

  gameEngine::Font uiFont;
  uiFont.create({
    (float)fontAtlasData.width,
    (float)fontAtlasData.height
  });

  cstmEngine::Batch gameBatch;
  gameBatch.create();

  cstmEngine::Batch uiBatch;
  uiBatch.create();

  game::Player player;
  player.setSize({GAME_TILE_SIZE, GAME_TILE_SIZE});
  player.m_phy.m_mass = 20.0f;

  game::PlayerLevelState playerState;
  bool playerInitalPosSet = false;

  gameEngine::Time gameTime;
  float playerNextFrameInterval = 0.005f;
  gameEngine::Time playerNextFrameTimer;
  playerNextFrameTimer.m_last = glfwGetTime();

  game::Level level1;
  level1.loadLevel(levelRle1, levelPoints1);
  game::Level level2;
  level2.loadLevel(levelRle2, levelPoints2);
  game::Level level3;
  level3.loadLevel(levelRle3, levelPoints3);

  while (gameWindow.isOpen()) {
    float half_width = (float)gameWindow.m_width / 2;
    float half_height = (float)gameWindow.m_height / 2;

    // Calculation Scope
    {
      gameTime.m_crnt = glfwGetTime();
      gameTime.m_delta = gameTime.m_crnt - gameTime.m_last;
      gameTime.m_last = gameTime.m_crnt;

      cstmEngine::vec2 playerForce;

      if (glfwGetKey(gameWindow.m_window, GLFW_KEY_A) == GLFW_PRESS ||
          glfwGetKey(gameWindow.m_window, GLFW_KEY_LEFT) == GLFW_PRESS ||
          glfwGetKey(gameWindow.m_window, GLFW_KEY_H) == GLFW_PRESS) {
        playerForce.x = -1000.0f;
        player.m_viewSide = game::SIDE_LEFT;
      }
      else if (glfwGetKey(gameWindow.m_window, GLFW_KEY_D) == GLFW_PRESS ||
               glfwGetKey(gameWindow.m_window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
               glfwGetKey(gameWindow.m_window, GLFW_KEY_L) == GLFW_PRESS) {
        playerForce.x = 1000.0f;
        player.m_viewSide = game::SIDE_RIGHT;
      }
      else
        playerForce.x =  0.0f;

      if (glfwGetKey(gameWindow.m_window, GLFW_KEY_SPACE) == GLFW_PRESS ||
          glfwGetKey(gameWindow.m_window, GLFW_KEY_K) == GLFW_PRESS)
        playerForce.y = 500.0f;
      else if (glfwGetKey(gameWindow.m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
               glfwGetKey(gameWindow.m_window, GLFW_KEY_J) == GLFW_PRESS)
        playerForce.y = -500.0f;
      else
        playerForce.y = 0.0f;

      player.m_phy.update(
        gameTime.m_delta, playerForce, {0.7f, 0.7f},
        {half_width, half_height}, GAME_TILE_SIZE,
        {10.0f, 200.0f}
      );

      player.m_phy.m_velocity.x = 
          player.m_phy.m_velocity.x > 0.0f ?
          std::min(player.m_phy.m_velocity.x,  500.0f):
          std::max(player.m_phy.m_velocity.x, -500.0f);
    }

    // Render Scope
    {
      playerNextFrameTimer.m_crnt = glfwGetTime();
      gameWindow.beginFrame(115/255.0f, 190/255.0f, 211/255.0f, 1.0f);

      glm::mat4 orthoProj = glm::ortho(
        -half_width, half_width,
        -half_height, half_height,
        0.1f, 100.0f
      );
      glm::mat4 view;

      gameBatch.beginFrame();

      gameShader.use();
      gameAtlas.use(0, "gameAtlas", &gameShader);

      glUniformMatrix4fv(
        glGetUniformLocation(gameShader.getShaderProgram(), "orthoProj"),
        1, GL_FALSE, glm::value_ptr(orthoProj)
      );

      view = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(std::min(0.0f, -player.m_phy.m_pos.x), 0.0f, -1.0f)
      );
      glUniformMatrix4fv(
        glGetUniformLocation(gameShader.getShaderProgram(), "view"),
        1, GL_FALSE, glm::value_ptr(view)
      );

      game::renderCloud(
        gameBatch,
        gameAtlasGrid,
        gameQuadSizes,
        GAME_TILE_SIZE,
        2, 0,
        GAME_SEED
      );

      level1.renderLevel(
        gameBatch, player,
        playerState,
        gameAtlasGrid,
        gameQuadSizes,
        GAME_TILE_SIZE,
        {(float)gameWindow.m_width, (float)gameWindow.m_height},
        GAME_SEED
      );

      player.render(gameBatch, gameAtlasGrid, playerNextFrameTimer, playerNextFrameInterval);

      gameBatch.endFrame();

      uiBatch.beginFrame();

      uiShader.use();
      fontAtlas.use(1, "fontAtlas", &uiShader);

      glUniformMatrix4fv(
        glGetUniformLocation(uiShader.getShaderProgram(), "orthoProj"),
        1, GL_FALSE, glm::value_ptr(orthoProj)
      );

      view = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f)
      );
      glUniformMatrix4fv(
        glGetUniformLocation(uiShader.getShaderProgram(), "view"),
        1, GL_FALSE, glm::value_ptr(view)
      );

      std::string uiStatusLineString = "Points:" + std::to_string(playerState.score);
      uiStatusLineString += ",Level:" + std::to_string(playerState.crntLevel + 1);
      std::string doorWarningMsg = "You need points for next level!";

      if (playerState.doorMsg) {
        int levelPoints = 0;

        if (playerState.crntLevel == 0)
          levelPoints = levelPoints1;
        else if (playerState.crntLevel == 1)
          levelPoints = levelPoints2;
        else if (playerState.crntLevel == 2)
          levelPoints = levelPoints3;

        doorWarningMsg.insert(9, std::to_string(levelPoints) + " ");
        uiFont.renderCentered(uiBatch, doorWarningMsg, {0, 0}, 18);
      }

      uiFont.render(uiBatch, uiStatusLineString, {-half_width + 9 + 10, half_height - 9 - 10}, 18);

      uiBatch.endFrame();

      gameWindow.endFrame();
    }
  }

  gameBatch.destroy();
  uiBatch.destroy();
  gameAtlas.destroy();
  gameShader.destroy();
  gameWindow.destroy();
  return 0;
}
