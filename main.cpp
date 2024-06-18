#include "glm/include/glm.hpp"
#include "glm/include/gtc/matrix_transform.hpp"
#include "glm/include/gtc/type_ptr.hpp"

#include "cstmEngine/window.hpp"
#include "cstmEngine/buffer.hpp"
#include "cstmEngine/shader.hpp"
#include "cstmEngine/texture.hpp"
#include "cstmEngine/batch.hpp"

#include "player.hpp"
#include "levels.hpp"
#include "texture.hpp"

#include <vector>
#include <string>

namespace cstmEngine {
  struct Time {
    float crnt = 0;
    float last = 0; 
    float delta = 0;

    void refresh() {
      crnt = glfwGetTime();
      delta = crnt - last;
      last = crnt;
    }
  };
}

cstmEngine::Window game_window;

void windowResizeCallback(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);

  game_window.m_width = w;
  game_window.m_height = h;
}

void createLevel(
  cstmEngine::Batch &batch,
  float square_size,
  const std::string &level_rle,
  cstmEngine::Vector2 half_window_size,
  std::vector<cstmEngine::Vector2[4]> &texture_grid
) {
  std::string level_num;
  int texture_idx;
  int level_num_i;
  int bush_level = -1;
  char level_char;

  cstmEngine::Vector2 block_position = {
    -half_window_size.x + (square_size / 2), 
    -half_window_size.y + (square_size / 2)
  };

  for (int i = 0; i < level_rle.size(); i++) {
    if (level_rle[i] == '\n') {
      block_position.x = -half_window_size.x + (square_size / 2);
      block_position.y += square_size;
      continue;
    }

    if (level_rle[i] < '0' || level_rle[i] > '9') {
      level_num_i = std::stoi(level_num);
      level_char = level_rle[i];
      level_num = "";

      if (level_char == '|') continue;

      switch (level_char) {
        case ' ':
          block_position.x += level_num_i * (square_size);
          break;
        case 'G':
          texture_idx = 6;
          break;
        case 'd':
          texture_idx = 7;
          break;
        case 'L':
          texture_idx = 12;
        case 'l':
          texture_idx = 13;
        case 'B':
          texture_idx = 8;
      }

      for (int j = 0; j < level_num_i; j++) {

        block_position.x += square_size;
      }

      continue;
    }

    level_num += level_rle[i];
  }
};

int main() {
  const float GAME_GRAVITY = 9.80665f;
  const float GAME_SQUARE_SIZE = 50;

  game_window.create(720, 480, "Mario Adventures");
  glfwSetWindowSizeCallback(game_window.m_window, windowResizeCallback);

  const char *vertex_shader =
    "#version 330 core\n"
    "layout (location = 0) in vec2 pos;\n"
    "layout (location = 1) in vec2 tex_coords;\n"
    "layout (location = 2) in vec3 color;\n"

    "out vec2 v_tex_coords;\n"
    "out vec3 v_color;\n"

    "uniform mat4 ortho_proj;\n"
    "uniform mat4 view;\n"

    "void main() {\n"
    "  gl_Position = ortho_proj * view * vec4(pos, 0.0f, 1.0f);\n"
    "  v_tex_coords = tex_coords;\n"
    "  v_color = color;\n"
    "}";

  const char *fragment_shader =
    "#version 330 core\n"

    "out vec4 px_color;\n"

    "in vec2 v_tex_coords;\n"
    "in vec3 v_color;\n"

    "uniform sampler2D game_atlas;\n"

    "void main() {\n"
    "  px_color = texture2D(game_atlas, v_tex_coords) * vec4(v_color, 1.0f);\n"
    "}";

  cstmEngine::Shader game_shader;
  game_shader.create(vertex_shader, fragment_shader);

  stbi_set_flip_vertically_on_load(true);

  cstmEngine::TextureData game_atlas_data;
  game_atlas_data.data = stbi_load(
    "./assets/GamePixelArt.png",
    &game_atlas_data.width,
    &game_atlas_data.height,
    &game_atlas_data.color_channels,
    0
  );

  cstmEngine::Texture game_atlas;
  game_atlas.create(game_atlas_data);

  cstmEngine::Vector2 game_atlas_wh = {(float)game_atlas_data.width, (float)game_atlas_data.height};
  std::vector<cstmEngine::Vector2[4]> game_atlas_grid(8 * 8);

  for (int i = 0, k = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++, k++) {
      textureCropAtlas(game_atlas_grid[k], game_atlas_wh, 32, {1, 1}, {(float)j, (float)i});
    }
  }

  cstmEngine::Batch game_batch;
  game_batch.create();

  cstmEngine::Time game_time;
  Player game_player;
  game_player.w = game_player.h = GAME_SQUARE_SIZE;
  game_player.mass = 10;

  while (game_window.isOpen()) {
    float half_width = (float)game_window.m_width / 2;
    float half_height = (float)game_window.m_height / 2;

    // Calculation Scope
    {
      game_time.refresh();

      float ground_level = -half_height + game_player.h / 2;

      if (glfwGetKey(game_window.m_window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(game_window.m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        game_player.force.x = 150.0f;
      }
      else if (glfwGetKey(game_window.m_window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(game_window.m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        game_player.force.x = -150.0f;
      }
      else
        game_player.force.x = 0.0f;

      game_player.friction.x = 0.7f * GAME_GRAVITY * game_player.mass;
      game_player.accel.x = game_player.force.x / game_player.mass;
      game_player.velocity.x += game_player.accel.x * game_time.delta;

      float _v = game_player.friction.x / game_player.mass * game_time.delta;

      if (abs(game_player.velocity.x) < 0.01f) game_player.velocity.x = 0.0f;

      if (game_player.velocity.x > 0.0f) game_player.velocity.x -= _v;
      else if (game_player.velocity.x < 0.0f) game_player.velocity.x += _v;

      game_player.x += game_player.velocity.x;

      // TODO: Improve this part
      if (game_player.y == ground_level) game_player.on_ground = true;
      else game_player.on_ground = false;

      game_player.velocity.y -= GAME_GRAVITY * game_player.mass * game_time.delta;

      if (game_player.on_ground == true && glfwGetKey(game_window.m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        game_player.velocity.y = 1000.0f * game_time.delta;
      }

      game_player.y += game_player.velocity.y;
      game_player.y = std::max(ground_level, game_player.y);
    }

    // Render Scope
    {
      game_window.beginFrame(143/255.0f, 211/255.0f, 255/255.0f, 1.0f);
      game_batch.beginFrame();

      game_shader.use();
      game_atlas.use(0, "game_atlas", &game_shader);

      glm::mat4 ortho_proj = glm::ortho(-half_width, half_width, -half_height, half_height, 0.1f, 100.0f);
      glUniformMatrix4fv(glGetUniformLocation(game_shader.getShaderProgram(), "ortho_proj"), 1, GL_FALSE, glm::value_ptr(ortho_proj));

      glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-game_player.x, -game_player.y, -1.0f));
      glUniformMatrix4fv(glGetUniformLocation(game_shader.getShaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));

      createLevel(game_batch, GAME_SQUARE_SIZE, level_3, {half_width, half_height}, game_atlas_grid);

      // Drawing The Player
      game_batch.drawQuadT({game_player.w, game_player.h}, {game_player.x, game_player.y}, game_atlas_grid[0]);

      game_batch.endFrame();
      game_window.endFrame();
    }
  }

  game_batch.destroy();
  game_shader.destroy();
  game_window.destroy();
  return 0;
}
