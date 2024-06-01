#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "glad/include/glad/glad.h"

namespace cstmEngine {
  struct TextureData {
    int width, height;
    int color_channels;
    unsigned char *data;
  };

  class Texture {
    public:
      void create(TextureData img_data) {
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(
          GL_TEXTURE_2D, 0, 
          img_data.color_channels == 3 ? GL_RGB : GL_RGBA,
          img_data.width, img_data.height, 0, img_data.color_channels == 3 ? GL_RGB : GL_RGBA,
          GL_UNSIGNED_BYTE, img_data.data
        );
      }

      void destroy() {
        
      }
    private:
      unsigned int m_texture;
  };
}

#endif
