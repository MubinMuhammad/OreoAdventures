#ifndef GAME_SHADERS_HPP_
#define GAME_SHADERS_HPP_

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
  "  px_color = texture2D(game_atlas, v_tex_coords);\n"
  "}";

#endif
