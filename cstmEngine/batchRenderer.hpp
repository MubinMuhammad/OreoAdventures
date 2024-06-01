#ifndef _BATCH_RENDERER_H_
#define _BATCH_RENDERER_H_

#include "buffer.hpp"
#include "shader.hpp"

namespace cstmEngine {
  class BatchRenderer {
  public:
    void create() {
      m_buffer.create(sizeof(m_vertices), NULL, sizeof(m_indices), NULL);
    }

    void beginFrame() {
      total_quads = 0;
    }

    void endFrame() {
      glBufferSubData(GL_ARRAY_BUFFER, 0, total_quads * sizeof(Vertex) * 4, m_vertices);
      glDrawElements(GL_ARRAY_BUFFER, total_quads * 6, GL_UNSIGNED_BYTE, 0);
    }

    void destroy() {
      m_buffer.destroy();
    }
  private:
    Vertex m_vertices[1024 * 4];
    unsigned char m_indices[1024 * 6];
    unsigned int total_quads;
    cstmEngine::Buffer m_buffer;
  };
}

#endif
