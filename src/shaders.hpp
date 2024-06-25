#ifndef GAME_SHADERS_HPP_
#define GAME_SHADERS_HPP_

const char *gameVertexShader =
  "#version 330 core\n"
  "layout (location = 0) in vec2 pos;\n"
  "layout (location = 1) in vec2 texCoords;\n"
  "layout (location = 2) in vec3 color;\n"

  "out vec2 v_texCoords;\n"
  "out vec3 v_color;\n"

  "uniform mat4 orthoProj;\n"
  "uniform mat4 view;\n"

  "void main() {\n"
  "  gl_Position = orthoProj * view * vec4(pos, 0.0f, 1.0f);\n"
  "  v_texCoords = texCoords;\n"
  "  v_color = color;\n"
  "}";

const char *gameFragmentShader =
  "#version 330 core\n"

  "out vec4 px_color;\n"

  "in vec2 v_texCoords;\n"
  "in vec3 v_color;\n"

  "uniform sampler2D game_atlas;\n"

  "void main() {\n"
  "  px_color = texture2D(game_atlas, v_texCoords);\n"
  "}";

const char *uiVertexShader =
  "#version 330 core\n"
  "layout (location = 0) in vec2 pos;\n"
  "layout (location = 1) in vec2 texCoords;\n"
  "layout (location = 2) in vec3 color;\n"

  "out vec2 v_texCoords;\n"
  "out vec3 v_color;\n"

  "uniform mat4 orthoProj;\n"
  "uniform mat4 view;\n"

  "void main() {\n"
  "  gl_Position = orthoProj * view * vec4(pos, 0.0f, 1.0f);\n"
  "  v_texCoords = texCoords;\n"
  "  v_color = color;\n"
  "}";

const char *uiFragmentShader =
  "#version 330 core\n"

  "out vec4 pxColor;\n"

  "in vec2 v_texCoords;\n"
  "in vec3 v_color;\n"

  "uniform sampler2D gameAtlas;\n"

  "void main() {\n"
  "  pxColor = texture2D(gameAtlas, v_texCoords) * vec4(v_color, 1.0f);\n"
  "}";

#endif
