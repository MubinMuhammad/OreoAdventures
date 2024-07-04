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
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <vector>
#include <string>

// we create the gameWindow as a global variable for the
// function below.
cstmEngine::Window gameWindow;

// As you can see it has a `Callback` in it's name, A callback
// is an action taken when some event happens. Therefore, the
// following function will run only if the window is resized. it
// takes in the GLFWwindow, the new width, the new height and
// sets the new glViewport along with the new varibles.
void windowResizeCallback(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);

  gameWindow.m_width = w;
  gameWindow.m_height = h;
}

// endscreen is the screen you see when the game is finished.
// it returns true if we should close the game and false otherwise.
bool endScreen(cstmEngine::Batch &uiBatch, gameEngine::Font &font) {
  // this is a purple like color for the endscreen.
  gameWindow.beginFrame(30/255.0f, 29/255.0f, 57/255.0f, 1.0f);
  // we intialize the taken uiBatch.
  uiBatch.beginFrame();

  // we check if the player pressed the 'Q' key, if they did
  // we return true, meaning we should close the game.
  if (glfwGetKey(gameWindow.m_window, GLFW_KEY_Q) == GLFW_PRESS)
    return true;

  // these are handy fucntions to render some text.
  // Here, by default the text will be white, when the we see
  // "%r"(my style of writing(my handmade)) it tales the
  // font renderes that from now on print everything in red.
  // you can do something like:
  //
  //   "game %ro%wver"
  //
  // this means start drawing character white when you
  // see %r you start writing in red, when you see %w
  // start writing in white again. So, the end up result
  // the text will be "GAME OVER" while everything being
  // white expect 'o' being red.
  font.renderCentered(uiBatch, "game %rover", {0, 0}, 32);
  font.renderCentered(uiBatch, "press 'q' to quit!", {0, -40}, 16);

  // uiBatch.endFrame() actually calls glDrawElements() and renders
  // everything at the end. (each frame)
  uiBatch.endFrame();
  // some game window stuff. It just swaps buffers and
  // checks for keyboard input.
  gameWindow.endFrame();
  return false;
}

int main() {
  // the default tile size for the game.
  // can be changed to your liking.
  const float GAME_TILE_SIZE = 60;
  // the seed for the random number generations.
  // it is set to time(NULL), that means it will set
  // GAME_SEED to current unixTimeStamp look it up
  // on the internet.
  const uint32_t GAME_SEED = time(NULL);

  // creates a 720x480 window with "Mario Adventures"
  // being the title.
  gameWindow.create(720, 480, "Mario Adventures");
  // this is the function that will run the
  // windowResizeCallback() when the window actually
  // resizes. Otherwise, windowResizeCallback() will
  // be any other ordinary functions.
  glfwSetWindowSizeCallback(gameWindow.m_window, windowResizeCallback);

  // creating and intializing gameShader
  cstmEngine::Shader gameShader;
  gameShader.create(gameVertexShader, gameFragmentShader);

  // creating and intializing gameShader
  cstmEngine::Shader uiShader;
  uiShader.create(uiVertexShader, uiFragmentShader);

  // this is because OpenGL renders textures upside down
  // so we tell stb to load the texture upside down
  // so, at the end we see it the right way on our screen.
  stbi_set_flip_vertically_on_load(true);

  // creates and initializes gameAtlas.
  cstmEngine::TextureData gameAtlasData;
  gameAtlasData.data = stbi_load(
    "./assets/GamePixelArt.png",
    &gameAtlasData.width,
    &gameAtlasData.height,
    &gameAtlasData.color_channels,
    0
  );

  // creates and initializes gameAtlas.
  cstmEngine::Texture gameAtlas;
  gameAtlas.create(gameAtlasData);

  cstmEngine::vec2 gameAtlasWH = {
    (float)gameAtlasData.width,
    (float)gameAtlasData.height
  };

  // get s the atlasGrid which is a vector full of
  // texture coordinates of different sprites.
  std::vector<cstmEngine::vec2> gameAtlasGrid;
  // the quadSizes holds the factors of each quad.
  // we now can multiply the factors with GAME_QUAD_SIZE
  // to get the actual screen size of the quad.
  std::vector<cstmEngine::vec2> gameQuadSizes;
  // look up the src/texture.cpp
  game::textureGetCoordsIdxs(gameAtlasGrid, gameQuadSizes, gameAtlasWH, 32);

  // we do kind of the same stuff we did with
  // texture gameAtlas.
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

  // look up the gameEngine/font.cpp
  gameEngine::Font uiFont;
  uiFont.create({
    (float)fontAtlasData.width,
    (float)fontAtlasData.height
  });

  // creating a gameBatch.
  cstmEngine::Batch gameBatch;
  gameBatch.create();

  // creating a uiBatch.
  cstmEngine::Batch uiBatch;
  uiBatch.create();

  // this creates a player sprite with a size and mass.
  // if you increase the mass the player will move slower
  // because you are giving it the same force.
  game::Player player;
  player.setSize({GAME_TILE_SIZE, GAME_TILE_SIZE});
  player.m_phy.m_mass = 20.0f;

  // the player state contains information about the player
  // when it's in the level, and what taskes it has accompilished.
  game::PlayerLevelState playerState;
  // this is the frame interval for the players next
  // sprite texture.
  float playerNextFrameInterval = 0.05f;
  // this is necessary to have a precise time difference and
  // to go to the next frame of player at a regular interval.
  gameEngine::Time playerNextFrameTimer;
  playerNextFrameTimer.m_last = glfwGetTime();

  // this is mainly for the gameTime.m_delta, needed
  // when we are dealing with objects that are moving.
  gameEngine::Time gameTime;

  // loading all 3 levels. if you want to add more
  // levels just add them in here. It's better to have
  // a vector so we can easily index them and here, we
  // can just use a for loop to go through all the levels,
  // but we didn't have much time!
  game::Level level1;
  level1.loadLevel(levelRle1, levelPoints1);
  game::Level level2;
  level2.loadLevel(levelRle2, levelPoints2);
  game::Level level3;
  level3.loadLevel(levelRle3, levelPoints3);

  // current level is a pointer to the level the player
  // should be.
  game::Level *crntLevel = &level1;

  // everything in the while loop will be ran
  // for each frame we see on the screen.
  while (gameWindow.isOpen()) {
    float half_width = (float)gameWindow.m_width / 2;
    float half_height = (float)gameWindow.m_height / 2;

    // Calculation Scope
    {
      // here we get the time difference between
      // two frame and store them in gameTime.m_delta
      gameTime.m_crnt = glfwGetTime();
      gameTime.m_delta = gameTime.m_crnt - gameTime.m_last;
      gameTime.m_last = gameTime.m_crnt;

      // WARNING: the checks below aren't the best and may contain
      // bugs. needs for thinking and fixing.
      //
      // checking if the player level has gone to next
      // levels or not.
      if (playerState.m_crntLevel > 0) {
        // if the player have passed any level, this
        // will happend when he reaches the door with the
        // needed coins.
        if (crntLevel->m_passed) {
          // we reset the coordinates to:
          // X = 10
          // Y = 200
          player.m_phy.resetPosition(
            {half_width, half_height},
            GAME_TILE_SIZE,
            {10.0f, 200.0f}
          );

          // we reset his coin earnings.
          playerState.m_points = 0;

          // is print end game if the player finishes
          // all the levels.
          if (playerState.m_crntLevel > 2) {
            if (endScreen(uiBatch, uiFont)) break;
            continue;
          }
        }

        // we go to next level depending on what the m_crntLevel says.
        if (playerState.m_crntLevel == 1) crntLevel = &level2;
        else if (playerState.m_crntLevel == 2) crntLevel = &level3;
      }

      // here we just set a different forces when the player
      // presses specific keys.
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

      // we update the physics of the player each frame
      player.m_phy.update(
        gameTime.m_delta, playerForce, {0.7f, 0.7f},
        {half_width, half_height}, GAME_TILE_SIZE,
        {10.0f, 200.0f}
      );

      // set a maximum velocity player can reach in both
      // dimensions. if we don't limit the player's velocity,
      // it will get increased and increased by the
      // player's acceleration and may touch the speed of
      // light, so where's some code to stop that!
      player.m_phy.m_velocity.x = 
        std::clamp(player.m_phy.m_velocity.x, -500.0f, 500.0f);

      player.m_phy.m_velocity.y = 
        std::clamp(player.m_phy.m_velocity.y, -500.0f, 500.0f);
    }

    // Render Scope
    {
      // this frame part is basically to to keep up the reguler
      // texture change interval.
      playerNextFrameTimer.m_crnt = glfwGetTime();
      gameWindow.beginFrame(87/255.0f, 114/255.0f, 119/255.0f, 1.0f);

      // this is the ortho graphic projection.
      // this changes the screen cordinate from being like:
      //
      // -------------------------------------------
      // |               Y=1.0                     |
      // |                  ↑                      |
      // |X                 │                     X|
      // |=←────────────────┼───────────────────→ =|
      // |-                 │                     1|
      // |1                 │                     .|
      // |.                 ↓                     0|
      // |0              Y=-1.0                    |
      // -------------------------------------------
      //
      // to:
      //
      // -------------------------------------------
      // |               Y=window_half_height      |
      // |                  ↑                      |
      // |X                 │                     X|
      // |=←────────────────┼───────────────────→ =|
      // |-window_half      │     window_half_width|
      // |width             │                      |
      // |                  ↓                      |
      // |               Y=-window_half_height     |
      // -------------------------------------------
      glm::mat4 orthoProj = glm::ortho(
        -half_width, half_width,
        -half_height, half_height,
        0.1f, 100.0f
      );
      glm::mat4 view;

      gameBatch.beginFrame();

      // checkout cstmEngine/shader.hpp
      gameShader.use();
      // checkout gameEngine/texture.hpp & cherno's video on textures.
      gameAtlas.use(0, "gameAtlas", &gameShader);

      // sending the orthographic projection matrices to the shaders.
      glUniformMatrix4fv(
        glGetUniformLocation(gameShader.getShaderProgram(), "orthoProj"),
        1, GL_FALSE, glm::value_ptr(orthoProj)
      );

      // translating and clamping the values of X
      // with 0, and level's length.
      // and clamping Y values with 0.0f and 10
      // both are in tiles unit.
      view = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(
          std::clamp(
            -player.m_phy.m_pos.x,
            -((float)crntLevel->m_length * GAME_TILE_SIZE - gameWindow.m_width - GAME_TILE_SIZE),
            0.0f
          ),
          std::clamp(
            -player.m_phy.m_pos.y,
            -10.0f * GAME_TILE_SIZE,
            0.0f
          ), -1.0f
        )
      );

      // sending the view matrix to the vertex shader.
      glUniformMatrix4fv(
        glGetUniformLocation(gameShader.getShaderProgram(), "view"),
        1, GL_FALSE, glm::value_ptr(view)
      );

      // renders the cloud.
      game::renderCloud(
        gameBatch,
        gameAtlasGrid,
        gameQuadSizes,
        GAME_TILE_SIZE,
        2, 0,
        GAME_SEED,
        crntLevel->m_length
      );

      // renders the level. here we do crntLevel->
      // that means we can set crntLevel to any game::Level
      // refrence and be that level renderLevel() function.
      //
      // for example:
      // if crntLevel = &level2;
      //
      // the line below would be something like:
      //  level2.renderLevel(...)
      //
      // this feature helps us put the level in some kind
      // of variable to make it easier.
      crntLevel->renderLevel(
        gameBatch, player,
        playerState,
        gameAtlasGrid,
        gameQuadSizes,
        GAME_TILE_SIZE,
        {(float)gameWindow.m_width, (float)gameWindow.m_height},
        GAME_SEED
      );

      // rendering the player
      player.render(gameBatch, gameAtlasGrid, playerNextFrameTimer, playerNextFrameInterval);

      // ending the batch frame this actually call the
      // glDrawElements() function to draw everything
      // we did(level vertices & player vertices) to
      // the screen.
      gameBatch.endFrame();

      // starting the UI batch.
      uiBatch.beginFrame();

      // look up cherno's videos.
      uiShader.use();
      fontAtlas.use(1, "fontAtlas", &uiShader);

      // sending the same orthographic projection as game projection.
      glUniformMatrix4fv(
        glGetUniformLocation(uiShader.getShaderProgram(), "orthoProj"),
        1, GL_FALSE, glm::value_ptr(orthoProj)
      );

      // there should be no change the position of the camera
      // for the ui. therefore we keep it a constant
      // (0.0f, 0.0f, -1.0f)
      view = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f)
      );

      // sending the view projection to the vertex shader.
      glUniformMatrix4fv(
        glGetUniformLocation(uiShader.getShaderProgram(), "view"),
        1, GL_FALSE, glm::value_ptr(view)
      );

      // creating a string that is going to be shown as the
      // status line. and adding player points to the string.
      std::string uiStatusLineString = "%gPoints:" + std::to_string(playerState.m_points);
      // adding the level + 1 to the status line because
      // it m_crntLevel is 0 when we are in level 1.
      uiStatusLineString += "%b Level:" + std::to_string(playerState.m_crntLevel + 1);
      // the door message is here.
      std::string doorWarningMsg = "You need %wpoints for next level!";

      // if there should be door message printed,
      // this is going to be happening when the player reaches
      // the door with less than the required coins. the
      // m_doorMsg is changed by the renderLevel() function
      // to the correct state.
      if (playerState.m_doorMsg) {
        int levelPoints = 0;

        // changing the level points to the correct
        // one. This could have been done way easily
        // if we used a vector.
        //
        // here's a simple example code if we had used vector:
        //   levelPoints = levelRlePoints[playerState.m_crntLevel];
        // 
        // this one line can represent the 3 if statements.
        // here instead of initalizing levelPoints1 levelPoints2
        // levelPoints3 as 3 different variables we could have called
        // them as an array/vector. Then, we could have easily indexed
        // them.
        if (playerState.m_crntLevel == 0)
          levelPoints = levelPoints1;
        else if (playerState.m_crntLevel == 1)
          levelPoints = levelPoints2;
        else if (playerState.m_crntLevel == 2)
          levelPoints = levelPoints3;

        // inserting levelPoints to the  doorWarning message with red
        // color. at 9-th index.
        doorWarningMsg.insert(9, "%r" + std::to_string(levelPoints) + " ");

        // printing the text on game's screen.
        uiFont.renderCentered(uiBatch, doorWarningMsg, {0, 0}, 18);
      }

      // printing the status line with a gap of 10px in both
      // X and Y.
      uiFont.render(uiBatch, uiStatusLineString, {-half_width + 9 + 10, half_height - 9 - 10}, 18);

      // endframe will actually call glDrawElements()
      // and draw everything we mentioned.
      uiBatch.endFrame();

      // endframe is going to swap the buffer sothat we can
      // start drawing the next one.
      //
      // remember this code is in a while loop and everything
      // we wrote is going to run iteratively.
      gameWindow.endFrame();
    }
  }

  // cleaning the memory otherwise the program may
  // cause memory leak.
  gameBatch.destroy();
  uiBatch.destroy();
  gameAtlas.destroy();
  gameShader.destroy();
  gameWindow.destroy();
  return 0;
}
