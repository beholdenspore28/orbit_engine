#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "mathf.h"
#include "logging.h"
#include "file.h"

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

engine_file_t engine_file_load_as_string(const char *filename);
void engine_file_free(const engine_file_t file);

GLuint engine_shader_create(const char *vertex_shader_file_path,
                            const char *fragment_shader_file_path);

mesh_t mesh_planet_alloc(const unsigned int subdivisions,
                         const float amplitude);

#endif // ENGINE_H
