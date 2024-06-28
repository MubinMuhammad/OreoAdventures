#ifndef CSTM_ENGINE_TYPES_H_
#define CSTM_ENGINE_TYPES_H_

namespace cstmEngine {
  struct vec2  { float x, y; };
  struct vec3  { float x, y, z; };
  struct vec4  { float x, y, z, w; };
  struct ivec2 { int   x = 0, y = 0; };
  struct ivec3 { int   x = 0, y = 0, z = 0; };
  struct ivec4 { int   x = 0, y = 0, z = 0, w = 0; };
}

#endif
