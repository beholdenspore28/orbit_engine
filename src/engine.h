#ifndef ENGINE_H
#define ENGINE_H

#include "engine_list.h"
#include "engine_logging.h"
#include "engine_mathf.h"

#include "glad/gl.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// these function pointers will change depending on which platform you're building for
extern float (*engine_get_aspect_ratio)(void);
extern bool (*engine_is_running)(void);

#ifdef __linux__
#define ENGINE_GLX
#endif // __linux__

#ifdef _WIN32
#define ENGINE_WGL
#endif // _WIN32

#ifdef ENGINE_GLX

#include "glad/glx.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct engine_window;
extern struct engine_window engine_window_instance;

bool engine_glx_start(void);
void engine_glx_stop(void);
void engine_glx_update(void);

#endif // ENGINE_GLX

#ifdef ENGINE_WGL
#endif // ENGINE_WGL

struct engine_file {
  size_t length;
  char *text;
  int error;
};

struct engine_file engine_file_load_as_string(const char *filename);

void engine_file_free(const struct engine_file file);

struct mesh {
  GLuint VAO;
  GLuint *VBOs;
  GLuint EBO;
  GLuint vertices_count;
  GLuint indices_count;
};

struct mesh mesh_planet_alloc(const unsigned int subdivisions,
                              const float amplitude);

struct camera {
  struct transform transform;
  float *matrix;
};

struct camera camera_alloc(void);
void camera_update(struct camera *camera);

GLuint engine_shader_compile_source(const char *file_path,
                                    uint32_t shader_type);
GLuint engine_shader_create(const char *vertex_shader_file_path,
                            const char *fragment_shader_file_path);

typedef struct vector2 vector2;
typedef struct vector3 vector3;

DECLARE_LIST(vector2)
DECLARE_LIST(vector3)
DECLARE_LIST(GLuint)

#endif // ENGINE_H
