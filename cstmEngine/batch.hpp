#ifndef _BATCH_RENDERER_H_
#define _BATCH_RENDERER_H_

#include "buffer.hpp"

namespace cstmEngine {
  class Batch {
    public:
      void create() {
        int a = 3;
        for (int i = 0; i < 1024 * 6; i += 6) {
          m_indices[i + 0] = a - 3;
          m_indices[i + 1] = a - 2;
          m_indices[i + 2] = a - 1;
          m_indices[i + 3] = a - 3;
          m_indices[i + 4] = a - 0;
          m_indices[i + 5] = a - 1;
          a += 4;
        }
        
        m_buffer.create(sizeof(m_vertices), NULL, sizeof(m_indices), m_indices);
      }

      void beginFrame() {
        total_quads = 0;
      }

      void endFrame() {
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer.m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, total_quads * sizeof(Vertex) * 4, m_vertices);
        glBindVertexArray(m_buffer.m_vao);
        glDrawElements(GL_TRIANGLES, total_quads * 6, GL_UNSIGNED_INT, 0);
      }

      void destroy() {
        m_buffer.destroy();
      }

      void drawQuadC(Vector2 size, Vector2 pos, Vector3 color) {
        if (total_quads == 1024 * 4) {
          glBindBuffer(GL_ARRAY_BUFFER, m_buffer.m_vbo);
          glBufferSubData(GL_ARRAY_BUFFER, 0, total_quads * sizeof(Vertex) * 4, m_vertices);
          glBindVertexArray(m_buffer.m_vao);
          glDrawElements(GL_TRIANGLES, total_quads * 6, GL_UNSIGNED_INT, 0);
          total_quads = 0;
        }

        float x = size.x / 2;
        float y = size.y / 2;

        m_vertices[total_quads * 4 + 0] = {{-x + pos.x, y + pos.y}, {}, color};
        m_vertices[total_quads * 4 + 1] = {{-x + pos.x,-y + pos.y}, {}, color};
        m_vertices[total_quads * 4 + 2] = {{ x + pos.x,-y + pos.y}, {}, color};
        m_vertices[total_quads * 4 + 3] = {{ x + pos.x, y + pos.y}, {}, color};

        total_quads++;
      }

      void drawQuadT(Vector2 size, Vector2 pos, Vector2 *texture_coords) {
        if (total_quads == 1024 * 4) {
          glBindBuffer(GL_ARRAY_BUFFER, m_buffer.m_vbo);
          glBufferSubData(GL_ARRAY_BUFFER, 0, total_quads * sizeof(Vertex) * 4, m_vertices);
          glBindVertexArray(m_buffer.m_vao);
          glDrawElements(GL_TRIANGLES, total_quads * 6, GL_UNSIGNED_INT, 0);
          total_quads = 0;
        }

        float x = size.x / 2;
        float y = size.y / 2;

        m_vertices[total_quads * 4 + 0] = {{-x + pos.x, y + pos.y}, {texture_coords[0]}, {}};
        m_vertices[total_quads * 4 + 1] = {{-x + pos.x,-y + pos.y}, {texture_coords[1]}, {}};
        m_vertices[total_quads * 4 + 2] = {{ x + pos.x,-y + pos.y}, {texture_coords[2]}, {}};
        m_vertices[total_quads * 4 + 3] = {{ x + pos.x, y + pos.y}, {texture_coords[3]}, {}};

        total_quads++;
      }
    private:
      Vertex m_vertices[1024 * 4];
      unsigned int m_indices[1024 * 6];
      unsigned int total_quads = 0;
      cstmEngine::Buffer m_buffer;
  };
}

#endif
