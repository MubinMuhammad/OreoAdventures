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

const cstmEngine::Vector3 GAME_CLOUD1          = {1, 1, 0};
const cstmEngine::Vector3 GAME_CLOUD2          = {2, 1, 1};
const cstmEngine::Vector3 GAME_CLOUD3          = {1, 1, 3};
const cstmEngine::Vector3 GAME_CLOUD4          = {1, 1, 4};
const cstmEngine::Vector3 GAME_COIN            = {1, 1, 5};
const cstmEngine::Vector3 GAME_GRASS           = {1, 1, 6};
const cstmEngine::Vector3 GAME_DIRT            = {1, 1, 7};
const cstmEngine::Vector3 GAME_CACTUS          = {1, 1, 14};
const cstmEngine::Vector3 GAME_SAND            = {1, 1, 15};
const cstmEngine::Vector3 GAME_HEART1          = {1, 1, 16};
const cstmEngine::Vector3 GAME_HEART2          = {1, 1, 17};
const cstmEngine::Vector3 GAME_HEART3          = {1, 1, 18};
const cstmEngine::Vector3 GAME_WATER           = {1, 1, 22};
const cstmEngine::Vector3 GAME_BRICK           = {1, 1, 23};
const cstmEngine::Vector3 GAME_QUESTION_BLOCK  = {1, 1, 24};
const cstmEngine::Vector3 GAME_FENCE           = {1, 1, 25};
const cstmEngine::Vector3 GAME_WOODPILE        = {1, 1, 26};
const cstmEngine::Vector3 GAME_PIPE_UP         = {1, 1, 27};
const cstmEngine::Vector3 GAME_PIPE_HEAD       = {1, 1, 28};
const cstmEngine::Vector3 GAME_BOX             = {1, 1, 30};
const cstmEngine::Vector3 GAME_DOOR            = {1, 1, 31};
const cstmEngine::Vector3 GAME_SLAB            = {1, 1, 32};
const cstmEngine::Vector3 GAME_PLAYER1         = {1, 1, 33};
const cstmEngine::Vector3 GAME_PLAYER2         = {1, 1, 34};
const cstmEngine::Vector3 GAME_BUSH1           = {2, 1, 35};
const cstmEngine::Vector3 GAME_BUSH2           = {2, 1, 37};
const cstmEngine::Vector3 GAME_TREE1           = {2, 2, 38};

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
  std::vector<cstmEngine::Vector2[4]> &texture_grid,
  cstmEngine::Vector2 player_pos
) {
  std::string level_num;
  char level_char;
  const cstmEngine::Vector3 *_block = nullptr;
  int level_num_i;

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
          block_position.x += level_num_i * square_size;
          continue;
          break;
        case 'G':
          _block = &GAME_GRASS;
          break;
        case 'd':
          _block = &GAME_DIRT;
          break;
        case 'T':
          _block = &GAME_TREE1;
          break;
        case 'B':
          _block = &GAME_BUSH1;
          break;
        case 'P':
          _block = &GAME_PLAYER1;
          break;
        case 'C':
          _block = &GAME_COIN;
          break;
        case 'D':
          _block = &GAME_DOOR;
          break;
        case 'W':
          _block = &GAME_WOODPILE;
          break;
        case 'w':
          _block = &GAME_WATER;
          break;
        case 'S':
          _block = &GAME_SLAB;
          break;
        case '_':
          _block = &GAME_SLAB;
          break;
        case 'U':
          _block = &GAME_SLAB;
          break;
        case 'b':
          _block = &GAME_BOX;
          break;
        case 'f':
          _block = &GAME_FENCE;
          break;
        default:
          _block = &GAME_QUESTION_BLOCK;
          break;
      }

      for (int j = 0; j < level_num_i; j++) {
        batch.drawQuadT(
          {_block->x * square_size, _block->y * square_size},
          {block_position.x, block_position.y},
          texture_grid[_block->z]
        );
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

  cstmEngine::Vector2 game_atlas_wh = {
    (float)game_atlas_data.width,
    (float)game_atlas_data.height
  };

  std::vector<cstmEngine::Vector2[4]> game_atlas_grid(8 * 8);

  for (int i = 0, k = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++, k++) {
      textureCropAtlas( game_atlas_grid[k], 
        game_atlas_wh, 32, {1, 1},
        {(float)j, (float)i}
      );
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

      if (glfwGetKey(game_window.m_window, GLFW_KEY_D) == GLFW_PRESS ||
          glfwGetKey(game_window.m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        game_player.force.x = 150.0f;
      }
      else if (glfwGetKey(game_window.m_window, GLFW_KEY_A) == GLFW_PRESS ||
               glfwGetKey(game_window.m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
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

      if (game_player.on_ground == true &&
          glfwGetKey(game_window.m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        game_player.velocity.y = 1000.0f * game_time.delta;
      }

      game_player.y += game_player.velocity.y;
      game_player.y = std::max(ground_level, game_player.y);
    }

    // Render Scope
    {
      game_window.beginFrame(129/255.0f, 151/255.0f, 150/255.0f, 1.0f);
      game_batch.beginFrame();

      game_shader.use();
      game_atlas.use(0, "game_atlas", &game_shader);

      glm::mat4 ortho_proj = glm::ortho(
        -half_width, half_width,
        -half_height, half_height,
        0.1f, 100.0f
      );
      glUniformMatrix4fv(
        glGetUniformLocation(game_shader.getShaderProgram(), "ortho_proj"),
        1, GL_FALSE, glm::value_ptr(ortho_proj)
      );

      glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-game_player.x, 0.0f, -1.0f));
      glUniformMatrix4fv(
        glGetUniformLocation(game_shader.getShaderProgram(), "view"),
        1, GL_FALSE, glm::value_ptr(view)
      );

      createLevel(
        game_batch, GAME_SQUARE_SIZE, level_1,
        {half_width, half_height},
        game_atlas_grid, {game_player.x, game_player.y}
      );

      // Drawing The Player
      game_batch.drawQuadT(
        {GAME_PLAYER1.x * GAME_SQUARE_SIZE, GAME_PLAYER1.y * GAME_SQUARE_SIZE},
        {game_player.x, game_player.y},
        game_atlas_grid[GAME_PLAYER1.z]
      );

      game_batch.endFrame();
      game_window.endFrame();
    }
  }

  game_batch.destroy();
  game_shader.destroy();
  game_window.destroy();
  return 0;
}
