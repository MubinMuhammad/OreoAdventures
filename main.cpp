#include <iostream>
#include "cstmEngine/window.hpp"
#include "cstmEngine/buffer.hpp"
#include "cstmEngine/shader.hpp"
#include "cstmEngine/texture.hpp"
#include "cstmEngine/batchRenderer.hpp"

int main() {
  cstmEngine::Window game_window;
  game_window.create(720, 480, "MarioAdventures");

  const char *vertex_shader =
    "#version 330 core\n"
    "layout(location = 0) in vec2 xy;\n"
    "layout(location = 1) in vec3 color;\n"

    "out vec3 v_color;\n"

    "void main() {\n"
    "  gl_Position = vec4(vec2(xy), 0.0f, 1.0f);\n"
    "  v_color = color;\n"
    "}";

  const char *fragment_shader =
    "#version 330 core\n"
    "out vec4 frag_color;\n"

    "in vec3 v_color;\n"

    "void main() {\n"
    "  frag_color = vec4(v_color, 1.0f);\n"
    "}";

  cstmEngine::Shader game_shader;
  game_shader.create(vertex_shader, fragment_shader);

  cstmEngine::BatchRenderer brenderer;
  brenderer.create();

  float playerX = 0.0f;
  float playerY = 0.0f;
  float last_time;
  float crnt_time;
  float delta_time;

  while (game_window.isOpen()) {
    crnt_time = glfwGetTime();
    delta_time = crnt_time - last_time;
    last_time = crnt_time;

    game_window.beginFrame(237/255.0f, 219/255.0f, 178/255.0f, 1.0f);
    brenderer.beginFrame();

    game_shader.use();
    brenderer.drawQuadC({0.5f, 0.5f}, {playerX, playerY}, {193 / 255.0f, 74 / 255.0f, 74 / 255.0f});

    if (
      glfwGetKey(game_window.m_window, GLFW_KEY_W) == GLFW_PRESS ||
      glfwGetKey(game_window.m_window, GLFW_KEY_UP) == GLFW_PRESS
    ) playerY += 1.0f * delta_time;

    if (
      glfwGetKey(game_window.m_window, GLFW_KEY_S) == GLFW_PRESS ||
      glfwGetKey(game_window.m_window, GLFW_KEY_DOWN) == GLFW_PRESS
    ) playerY -= 1.0f * delta_time;

    if (
      glfwGetKey(game_window.m_window, GLFW_KEY_A) == GLFW_PRESS ||
      glfwGetKey(game_window.m_window, GLFW_KEY_LEFT) == GLFW_PRESS
    ) playerX -= 1.0f * delta_time;

    if (
      glfwGetKey(game_window.m_window, GLFW_KEY_D) == GLFW_PRESS ||
      glfwGetKey(game_window.m_window, GLFW_KEY_RIGHT) == GLFW_PRESS
    ) playerX += 1.0f * delta_time;

    brenderer.endFrame();
    game_window.endFrame();
  }

  brenderer.destroy();
  game_shader.destroy();
  game_window.destroy();
  return 0;
}
