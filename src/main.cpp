// GLM Includes
#include "../glm/include/glm.hpp"
#include "../glm/include/gtc/matrix_transform.hpp"
#include "../glm/include/gtc/type_ptr.hpp"

// Graphics Engine Includes
#include "../cstmEngine/window.hpp"
#include "../cstmEngine/buffer.hpp"
#include "../cstmEngine/shader.hpp"
#include "../cstmEngine/texture.hpp"
#include "../cstmEngine/batch.hpp"

// Game Engine Includes
#include "../gameEngine/time.hpp"

// Game Includes
#include "player.hpp"
#include "levels.hpp"
#include "texture.hpp"
#include "shaders.hpp"

#include <GLFW/glfw3.h>
#include <vector>
#include <string>

/*
 * TODO:
 *  Fix tree texture coordinates
 *  Fix player position
 * */

cstmEngine::Window gameWindow;

void windowResizeCallback(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);

  gameWindow.m_width = w;
  gameWindow.m_height = h;
}

int main() {
  const float GAME_SQUARE_SIZE = 60;

  gameWindow.create(720, 480, "Mario Adventures");
  glfwSetWindowSizeCallback(gameWindow.m_window, windowResizeCallback);

  cstmEngine::Shader gameShader;
  gameShader.create(vertex_shader, fragment_shader);

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
  std::vector<cstmEngine::vec2> game_quad_size;
  game::getTexCoordsFromAtlas(gameAtlasGrid, game_quad_size, gameAtlasWH, 32);

  cstmEngine::Batch gameBatch;
  gameBatch.create();

  game::Player player;
  player.m_size = {GAME_SQUARE_SIZE, GAME_SQUARE_SIZE};
  player.m_phy.m_mass = 10.0f;

  gameEngine::Time gameTime;

  while (gameWindow.isOpen()) {
    float half_width = (float)gameWindow.m_width / 2;
    float half_height = (float)gameWindow.m_height / 2;

    // Calculation Scope
    {
      gameTime.update();
      cstmEngine::vec2 playerForce;

      if (glfwGetKey(gameWindow.m_window, GLFW_KEY_A) == GLFW_PRESS ||
          glfwGetKey(gameWindow.m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        playerForce.x = -100.0f;
        player.m_viewSide = game::SIDE_LEFT;
      }
      else if (glfwGetKey(gameWindow.m_window, GLFW_KEY_D) == GLFW_PRESS ||
               glfwGetKey(gameWindow.m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        playerForce.x = 100.0f;
        player.m_viewSide = game::SIDE_RIGHT;
      }
      else
        playerForce.x =  0.0f;

      if (glfwGetKey(gameWindow.m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        playerForce.y = 40.0f;
      else
        playerForce.y = 0.0f;

      player.m_phy.update(gameTime.m_delta, playerForce, {0.9f, 0.0f});
    }

    // Render Scope
    {
      gameWindow.beginFrame(129/255.0f, 151/255.0f, 150/255.0f, 1.0f);
      gameBatch.beginFrame();

      gameShader.use();
      gameAtlas.use(0, "gameAtlas", &gameShader);

      glm::mat4 ortho_proj = glm::ortho(
        -half_width, half_width,
        -half_height, half_height,
        0.1f, 100.0f
      );
      glUniformMatrix4fv(
        glGetUniformLocation(gameShader.getShaderProgram(), "ortho_proj"),
        1, GL_FALSE, glm::value_ptr(ortho_proj)
      );

      glm::mat4 view = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(std::min(0.0f, 0.0f), 0.0f, -1.0f)
      );
      glUniformMatrix4fv(
        glGetUniformLocation(gameShader.getShaderProgram(), "view"),
        1, GL_FALSE, glm::value_ptr(view)
      );

      // Drawing The Player
      player.render(gameBatch, gameAtlasGrid);

      gameBatch.endFrame();
      gameWindow.endFrame();
    }
  }

  gameBatch.destroy();
  gameShader.destroy();
  gameWindow.destroy();
  return 0;
}
