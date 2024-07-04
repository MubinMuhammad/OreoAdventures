#ifndef GAME_SHADERS_HPP_
#define GAME_SHADERS_HPP_

// this file is separated form main.cpp so that we
// can easily know where the shaders are located. Instead
// of searching the main file when ever we have to change
// something.

// NOTE: if you are not used to seeing `const char *` datatype,
// you shall figure it out by yourself by watching some youtube
// tutorials.
//
// gameVertexShader is the vertex shader used in the game.
// And, it's separate from the UI. The reason for is this
// we have separate batches for UI & game and the UI shader
// has a different view projection.
const char *gameVertexShader =
  "#version 330 core\n"

  // here we take in some variables from VBO & EBO
  // through VAO. these are done in cstmEngine/buffers.hpp.
  // There is an abstraction done by using a Batch renderer.
  "layout (location = 0) in vec2 pos;\n"
  "layout (location = 1) in vec2 texCoords;\n"
  "layout (location = 2) in vec3 color;\n"

  // Sending the texCoords and color
  // to the fragment shade
  "out vec2 v_texCoords;\n"
  "out vec3 v_color;\n"

  // taking in the projection, the projection matrix
  // is sent by the main function. So, make sure you take a
  // look at that.
  "uniform mat4 orthoProj;\n"
  "uniform mat4 view;\n"

  // just like in C/C++ a shader execution starts
  // from the main function, unlike C/C++ the main
  // function doesn't return anything so the return
  // type is void.
  "void main() {\n"
  // here we multiply the orthoProj by the view and by the object's
  // position to move it to screen space from world space.
  //
  // In matrices, the oriantion you multiply the values does matter
  // unlike when we do scalar multiplications. In APIs like Vulkan
  // the multiplication order will be filpped it will be:
  //
  //   vec4(pos, 0.0f, 1.0f) * view * orthoProj;
  //
  // (don't ask me why).
  "  gl_Position = orthoProj * view * vec4(pos, 0.0f, 1.0f);\n"
  // this is a traditional thing we have to do in OpenGL.
  // we have to first create a varible then send it to fragment
  // shader. we can't directly do it.
  "  v_texCoords = texCoords;\n"
  "  v_color = color;\n"
  "}";

const char *gameFragmentShader =
  "#version 330 core\n"

  // here, the out pixel color is going to be the end up
  // pixel on the display, the fragment shader runs for
  // every pixel on your screen.
  "out vec4 pxColor;\n"

  // the values we passed out from vertex shader are now
  // imported to the fragment shader.
  "in vec2 v_texCoords;\n"
  "in vec3 v_color;\n"

  // this line is quite confusing but this means that
  // you got the texture you binded for the game.
  // (i.e. assets/GamePixelArt.png)
  "uniform sampler2D gameAtlas;\n"

  "void main() {\n"
  // you now provide the image and the image coordinates
  // and OpenGL will crop it in the right way.
  "  pxColor = texture2D(gameAtlas, v_texCoords);\n"
  "}";

// the same goes for the UI vertex shader.
// (it doesn't change)
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

  // here we take in the assets/font.png
  "uniform sampler2D fontAtlas;\n"

  "void main() {\n"
  // we also make sure to multiply v_color by the image.
  // as the image font is only white, we can multiply other colors
  // with it to get our expected result. here we got the v_color from
  // the vertex shader and that got it from vertex buffer.
  "  pxColor = texture2D(fontAtlas, v_texCoords) * vec4(v_color, 1.0f);\n"
  "}";

#endif
