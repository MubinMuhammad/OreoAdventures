#ifndef GAME_ENGINE_TIME_H_
#define GAME_ENGINE_TIME_H_

#include <GLFW/glfw3.h>

namespace gameEngine {
  struct Time {
    float m_crnt;
    float m_last = 0.0f;
    float m_delta;

    void update() {
      m_crnt = glfwGetTime();
      m_delta = m_crnt - m_last;
      m_last = m_crnt;
    }
  };
}

#endif
