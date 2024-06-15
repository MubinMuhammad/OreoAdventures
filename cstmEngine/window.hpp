#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>

namespace cstmEngine {
  struct Window {
    GLFWwindow *m_window;
    int m_width;
    int m_height;
    const char *m_title;

    void create(int width, int height, const char *title) {
      m_width = width;
      m_height = height;
      m_title = title;

      if (!glfwInit()) {
        std::cout << "Failed to initialize glfw!\n" << '\n';
      }

      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
      m_window = glfwCreateWindow(width, height, title, NULL, NULL);
      
      if (m_window == NULL) {
        std::cout << "Failed to create window!\n" << '\n';
      }

      glfwMakeContextCurrent(m_window);

      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL!" << '\n';
      }

      glViewport(0, 0, width, height);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void destroy() {
      glfwDestroyWindow(m_window);
      glfwTerminate();
    }

    bool isOpen() {
      return !glfwWindowShouldClose(m_window);
    }
    
    void beginFrame(float r, float g, float b, float a) {
      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(r, g, b, a);
    }

    void endFrame() {
      glfwSwapBuffers(m_window);
      glfwPollEvents();
    }
  };
}

#endif
