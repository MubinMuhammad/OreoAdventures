#include <iostream>
#include "cstmEngine/window.hpp"
#include "cstmEngine/buffer.hpp"
#include "cstmEngine/shader.hpp"

int main() {
  cstmEngine::Window game_window;
  game_window.create(400, 500, "MarioAdventures");

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


  cstmEngine::Vertex vertices[4] = {
    {{-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
    {{-0.5f,-0.5f}, {1.0f, 0.0f, 0.0f}},
    {{ 0.5f,-0.5f}, {1.0f, 0.0f, 0.0f}},
  };

  cstmEngine::Buffer game_buffer;
  game_buffer.create(sizeof(vertices), vertices, 0, NULL);

  while (game_window.isOpen()) {
    game_window.beginFrame(125/255.0f, 196/255.0f, 207/255.0f, 1.0f);

    game_shader.useShader();
    game_buffer.useBuffer();

    glDrawArrays(GL_TRIANGLES, );

    game_window.endFrame();
  }

  
  game_buffer.destroy();
  game_shader.destroy();
  game_window.destroy();
  return 0;
}
