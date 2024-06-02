#include <iostream>
#include "cstmEngine/window.hpp"
#include "cstmEngine/buffer.hpp"
#include "cstmEngine/shader.hpp"
#include "cstmEngine/texture.hpp"

int main() {
  cstmEngine::Window game_window;
  game_window.create(400, 500, "MarioAdventures");

  const char *vertex_shader =
    "#version 330 core\n"
    "layout(location = 0) in vec2 xy;\n"
    "layout(location = 1) in vec2 tex_coords;\n"

    "out vec2 vtex_coords;\n"

    "void main() {\n"
    "  gl_Position = vec4(vec2(xy), 0.0f, 1.0f);\n"
    "  vtex_coords = tex_coords;\n"
    "}";

  const char *fragment_shader =
    "#version 330 core\n"
    "out vec4 frag_color;\n"

    "in vec2 vtex_coords;\n"
    "uniform sampler2D uv_Texture;\n"

    "void main() {\n"
    "  frag_color = texture2D(uv_Texture, vtex_coords);\n"
    "}";

  cstmEngine::Shader game_shader;
  game_shader.create(vertex_shader, fragment_shader);

  cstmEngine::TextureData game_texture_data;
  stbi_set_flip_vertically_on_load(1);
  game_texture_data.data = stbi_load(
    "/home/bssm/Pictures/nord.jpg", 
    &game_texture_data.width, 
    &game_texture_data.height, 
    &game_texture_data.color_channels, 0
  );

  cstmEngine::Texture game_texture;
  game_texture.create(game_texture_data);

  cstmEngine::Vertex vertices[] = {
    {{-0.5f, 0.5f}, {0.0f, 1.0f}},
    {{-0.5f,-0.5f}, {0.0f, 0.0f}},
    {{ 0.5f,-0.5f}, {1.0f, 0.0f}},
    {{ 0.5f, 0.5f}, {1.0f, 1.0f}},
  };

  unsigned char indices[] = {
    0, 1, 2,
    0, 3, 2
  };

  cstmEngine::Buffer game_buffer;
  game_buffer.create(sizeof(vertices), vertices, sizeof(indices), indices);

  while (game_window.isOpen()) {
    game_window.beginFrame(125/255.0f, 196/255.0f, 207/255.0f, 1.0f);

    game_shader.use();
    game_buffer.use();
    game_texture.use(0, "uv_Texture", &game_shader);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

    game_window.endFrame();
  }

  game_texture.destroy();
  game_buffer.destroy();
  game_shader.destroy();
  game_window.destroy();
  return 0;
}
