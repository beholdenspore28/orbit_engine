#ifndef ENGINE_GL_H
#define ENGINE_GL_H

#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

typedef struct {
  GLuint VAO;
  GLuint *VBOs;
  GLuint EBO;
  GLuint vertices_count;
  GLuint indices_count;
} mesh_t;

typedef struct {
  mathf_transform_t transform;
  float *matrix;
} camera_t;

DECLARE_LIST(vector2)
DECLARE_LIST(vector3)
DECLARE_LIST(GLuint)

GLuint engine_shader_create(const char *vertex_shader_file_path,
                            const char *fragment_shader_file_path);

mesh_t mesh_planet_alloc(const unsigned int subdivisions,
                         const float amplitude);

#endif // ENGINE_GL_H
