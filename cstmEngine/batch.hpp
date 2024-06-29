#ifndef _BATCH_RENDERER_H_
#define _BATCH_RENDERER_H_

#include "buffer.hpp"

#define BATCH_SQUARE_CAPACITY 1024

namespace cstmEngine {
  class Batch {
    public:
      void create() {
        int a = 3;
        for (int i = 0; i < BATCH_SQUARE_CAPACITY * 6; i += 6) {
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
        m_totalQuads = 0;
      }

      void endFrame() {
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer.m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_totalQuads * sizeof(Vertex) * 4, m_vertices);
        glBindVertexArray(m_buffer.m_vao);
        glDrawElements(GL_TRIANGLES, m_totalQuads * 6, GL_UNSIGNED_INT, 0);
      }

      void destroy() {
        m_buffer.destroy();
      }

      void drawQuad(vec2 size, vec2 pos, vec3 color) {
        if (m_totalQuads == BATCH_SQUARE_CAPACITY) {
          glBindBuffer(GL_ARRAY_BUFFER, m_buffer.m_vbo);
          glBufferSubData(GL_ARRAY_BUFFER, 0, m_totalQuads * sizeof(Vertex) * 4, m_vertices);
          glBindVertexArray(m_buffer.m_vao);
          glDrawElements(GL_TRIANGLES, m_totalQuads * 6, GL_UNSIGNED_INT, 0);
          m_totalQuads = 0;
        }

        float x = size.x / 2;
        float y = size.y / 2;

        m_vertices[m_totalQuads * 4 + 0] = {{-x + pos.x, y + pos.y}, {0.0f, 1.0f}, color};
        m_vertices[m_totalQuads * 4 + 1] = {{-x + pos.x,-y + pos.y}, {0.0f, 0.0f}, color};
        m_vertices[m_totalQuads * 4 + 2] = {{ x + pos.x,-y + pos.y}, {1.0f, 0.0f}, color};
        m_vertices[m_totalQuads * 4 + 3] = {{ x + pos.x, y + pos.y}, {1.0f, 1.0f}, color};

        m_totalQuads++;
      }

      void drawQuad(vec2 size, vec2 pos, vec2 *texture_coords) {
        if (m_totalQuads == BATCH_SQUARE_CAPACITY) {
          glBindBuffer(GL_ARRAY_BUFFER, m_buffer.m_vbo);
          glBufferSubData(GL_ARRAY_BUFFER, 0, m_totalQuads * sizeof(Vertex) * 4, m_vertices);
          glBindVertexArray(m_buffer.m_vao);
          glDrawElements(GL_TRIANGLES, m_totalQuads * 6, GL_UNSIGNED_INT, 0);
          m_totalQuads = 0;
        }

        float x = size.x / 2;
        float y = size.y / 2;

        m_vertices[m_totalQuads * 4 + 0] = {{-x + pos.x, y + pos.y}, {texture_coords[0]}, {1.0f, 1.0f, 1.0f}};
        m_vertices[m_totalQuads * 4 + 1] = {{-x + pos.x,-y + pos.y}, {texture_coords[1]}, {1.0f, 1.0f, 1.0f}};
        m_vertices[m_totalQuads * 4 + 2] = {{ x + pos.x,-y + pos.y}, {texture_coords[2]}, {1.0f, 1.0f, 1.0f}};
        m_vertices[m_totalQuads * 4 + 3] = {{ x + pos.x, y + pos.y}, {texture_coords[3]}, {1.0f, 1.0f, 1.0f}};

        m_totalQuads++;
      }

      void drawQuad(vec2 size, vec2 pos, vec2 *texture_coords, vec3 color) {
        if (m_totalQuads == BATCH_SQUARE_CAPACITY) {
          glBindBuffer(GL_ARRAY_BUFFER, m_buffer.m_vbo);
          glBufferSubData(GL_ARRAY_BUFFER, 0, m_totalQuads * sizeof(Vertex) * 4, m_vertices);
          glBindVertexArray(m_buffer.m_vao);
          glDrawElements(GL_TRIANGLES, m_totalQuads * 6, GL_UNSIGNED_INT, 0);
          m_totalQuads = 0;
        }

        float x = size.x / 2;
        float y = size.y / 2;

        m_vertices[m_totalQuads * 4 + 0] = {{-x + pos.x, y + pos.y}, {texture_coords[0]}, color};
        m_vertices[m_totalQuads * 4 + 1] = {{-x + pos.x,-y + pos.y}, {texture_coords[1]}, color};
        m_vertices[m_totalQuads * 4 + 2] = {{ x + pos.x,-y + pos.y}, {texture_coords[2]}, color};
        m_vertices[m_totalQuads * 4 + 3] = {{ x + pos.x, y + pos.y}, {texture_coords[3]}, color};

        m_totalQuads++;
      }
    private:
      Vertex m_vertices[BATCH_SQUARE_CAPACITY * 4];
      unsigned int m_indices[BATCH_SQUARE_CAPACITY * 6];
      unsigned int m_totalQuads = 0;
      cstmEngine::Buffer m_buffer;
  };
}

#endif
