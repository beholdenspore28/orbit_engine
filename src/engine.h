#ifndef ENGINE_H
#define ENGINE_H

#include "engine_list.h"
#include "engine_logging.h"
#include "engine_mathf.h"

#include "glad/gl.h"
#include "glad/glx.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern const char *window_title;
extern bool engine_running;
extern const int window_width, window_height;
extern Display *display;
extern int screen;
extern GLXContext context;
extern Window window;
extern Colormap colormap;

bool engine_start_glx(void);
void engine_stop_glx(void);
void engine_update_glx(void);

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
