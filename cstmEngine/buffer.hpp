#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_
#include "glad/include/glad/glad.h"
#include <iostream>

namespace cstmEngine {
  struct Vertex {
    float pos[2];
    float texture_coords[2];
  };

  class Buffer {
public:
    void create(unsigned int vertices_size, Vertex *vertices, unsigned int indices_size, unsigned char *indices) {
      glGenVertexArrays(1, &m_vao);
      glBindVertexArray(m_vao);

      glGenBuffers(1, &m_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_DYNAMIC_DRAW);

      if (indices_size != 0 && indices != NULL) {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_DYNAMIC_DRAW);
      }
      
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));

      glBindBuffer(m_vbo, 0);
      glBindVertexArray(0);

      if (indices_size != 0 && indices != NULL) {
        glBindBuffer(m_ebo, 0);
      }
    }
    
    void destroy() {
      glDeleteBuffers(1, &m_vbo);
      glDeleteBuffers(1, &m_ebo);
      glDeleteVertexArrays(1, &m_vao);
    }

    void use() {
      glBindVertexArray(m_vao);
    }
private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;
  };
}

#endif
