#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include "glad/include/glad/glad.h"
#include <iostream>

namespace cstmEngine {
  class Shader {
public:
    void create(const char *vs_source, const char *fs_source) {
      unsigned int vs_bin = glCreateShader(GL_VERTEX_SHADER);
      unsigned int fs_bin = glCreateShader(GL_FRAGMENT_SHADER);

      glShaderSource(vs_bin, 1, &vs_source, NULL);
      glShaderSource(fs_bin, 1, &fs_source, NULL);

      glCompileShader(vs_bin);
      glCompileShader(fs_bin);

      checkShaderErrors(vs_bin, GL_VERTEX_SHADER);
      checkShaderErrors(fs_bin, GL_FRAGMENT_SHADER);

      m_shader_bin = glCreateProgram();
      glAttachShader(m_shader_bin, vs_bin);
      glAttachShader(m_shader_bin, fs_bin);

      glDeleteShader(vs_bin);
      glDeleteShader(fs_bin);
      glLinkProgram(m_shader_bin);
    }

    void destroy() {
      glDeleteProgram(m_shader_bin);
    }

    unsigned int getShaderProgram() { 
      return m_shader_bin; 
    }

    void use() {
      glUseProgram(m_shader_bin);
    }
private:
    void checkShaderErrors(unsigned vsfs_shader_bin, GLenum shader_type) {
      int success;
      char logInfo[512];

      glGetShaderiv(vsfs_shader_bin, GL_COMPILE_STATUS, &success);

      if (!success) {
        glGetShaderInfoLog(vsfs_shader_bin, 512, NULL, logInfo);
        fprintf(stderr, "[error][%d:%s][%s] %s\n", __LINE__, __FILE__, 
                shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment", logInfo);
      }
    }

    unsigned int m_shader_bin;
  };
}

#endif
