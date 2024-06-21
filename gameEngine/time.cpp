#include "time.hpp"
#include <GLFW/glfw3.h>

void gameEngine::Time::update() {
  m_crnt = glfwGetTime();
  m_delta = m_crnt - m_last;
  m_last = m_crnt;
}
