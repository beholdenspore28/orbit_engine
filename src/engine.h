#ifndef ENGINE_H
#define ENGINE_H

#define _POSIX_C_SOURCE 200809L
#include "engine_list.h"
#include "engine_logging.h"
#include "engine_mathf.h"

#include "glad/gl.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool engine_start(void);
void engine_stop(void);
void engine_update(void);
float engine_get_aspect_ratio(void);
bool engine_is_running(void);

#ifdef __linux__
#define ENGINE_GLX
#endif // __linux__

#ifdef ENGINE_GLX

#include "glad/glx.h"
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct engine_window;
extern struct engine_window engine_window_instance;

#endif // ENGINE_GLX

#ifdef _WIN32
#define ENGINE_WGL
#endif // _WIN32

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
                              const struct vector3 noise_scale,
                              const struct vector3 noise_offset,
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

GLuint engine_texture_alloc(const char *imageFile);
void engine_texture_free(GLuint texture);

void engine_scene_load(void);
void engine_scene_unload(void);
void engine_scene_update(void);
void engine_scene_draw(void);

enum {
  ENGINE_KEY_W = 25,
  ENGINE_KEY_S = 39,
  ENGINE_KEY_A = 38,
  ENGINE_KEY_D = 40,
  ENGINE_KEY_SPACE = 65,
  ENGINE_KEY_LSHIFT = 50,
  ENGINE_KEY_LEFT_CONTROL = 37,
  ENGINE_KEY_COMMA = 59,
  ENGINE_KEY_PERIOD = 60,
};

bool engine_key_get(int keycode);

typedef struct vector2 vector2;
typedef struct vector3 vector3;

DECLARE_LIST(vector2)
DECLARE_LIST(vector3)
DECLARE_LIST(GLuint)

#endif // ENGINE_H
