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
#include <X11/keysym.h>

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
  bool use_indexed_draw;
};

struct mesh engine_mesh_quad_alloc(void);

struct mesh engine_mesh_planet_alloc(const unsigned int subdivisions,
                                     const struct vec3 noise_scale,
                                     const struct vec3 noise_offset,
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
  ENGINE_KEY_A = XK_a,
  ENGINE_KEY_B = XK_b,
  ENGINE_KEY_C = XK_c,
  ENGINE_KEY_D = XK_d,
  ENGINE_KEY_E = XK_e,
  ENGINE_KEY_F = XK_f,
  ENGINE_KEY_G = XK_g,
  ENGINE_KEY_H = XK_h,
  ENGINE_KEY_I = XK_i,
  ENGINE_KEY_J = XK_j,
  ENGINE_KEY_K = XK_k,
  ENGINE_KEY_L = XK_l,
  ENGINE_KEY_M = XK_m,
  ENGINE_KEY_N = XK_n,
  ENGINE_KEY_O = XK_o,
  ENGINE_KEY_P = XK_p,
  ENGINE_KEY_Q = XK_q,
  ENGINE_KEY_R = XK_r,
  ENGINE_KEY_S = XK_s,
  ENGINE_KEY_T = XK_t,
  ENGINE_KEY_U = XK_u,
  ENGINE_KEY_V = XK_v,
  ENGINE_KEY_W = XK_w,
  ENGINE_KEY_X = XK_x,
  ENGINE_KEY_Y = XK_y,
  ENGINE_KEY_Z = XK_z,

  ENGINE_KEY_0 = XK_0,
  ENGINE_KEY_1 = XK_1,
  ENGINE_KEY_2 = XK_2,
  ENGINE_KEY_3 = XK_3,
  ENGINE_KEY_4 = XK_4,
  ENGINE_KEY_5 = XK_5,
  ENGINE_KEY_6 = XK_6,
  ENGINE_KEY_7 = XK_7,
  ENGINE_KEY_8 = XK_8,
  ENGINE_KEY_9 = XK_9,

  ENGINE_KEY_SPACE = XK_space,
  ENGINE_KEY_ENTER = XK_Return,
  ENGINE_KEY_TAB = XK_Tab,
  ENGINE_KEY_BACKSPACE = XK_BackSpace,
  ENGINE_KEY_ESCAPE = XK_Escape,
  ENGINE_KEY_DELETE = XK_Delete,

  ENGINE_KEY_UP = XK_Up,
  ENGINE_KEY_DOWN = XK_Down,
  ENGINE_KEY_LEFT = XK_Left,
  ENGINE_KEY_RIGHT = XK_Right,

  ENGINE_KEY_F1 = XK_F1,
  ENGINE_KEY_F2 = XK_F2,
  ENGINE_KEY_F3 = XK_F3,
  ENGINE_KEY_F4 = XK_F4,
  ENGINE_KEY_F5 = XK_F5,
  ENGINE_KEY_F6 = XK_F6,
  ENGINE_KEY_F7 = XK_F7,
  ENGINE_KEY_F8 = XK_F8,
  ENGINE_KEY_F9 = XK_F9,
  ENGINE_KEY_F10 = XK_F10,
  ENGINE_KEY_F11 = XK_F11,
  ENGINE_KEY_F12 = XK_F12,

  // Punctuation and special characters
  ENGINE_KEY_COMMA = XK_comma,
  ENGINE_KEY_PERIOD = XK_period,
  ENGINE_KEY_SEMICOLON = XK_semicolon,
  ENGINE_KEY_COLON = XK_colon,
  ENGINE_KEY_APOSTROPHE = XK_apostrophe,
  ENGINE_KEY_QUOTE = XK_quotedbl,
  ENGINE_KEY_SLASH = XK_slash,
  ENGINE_KEY_BACKSLASH = XK_backslash,
  ENGINE_KEY_LEFT_BRACKET = XK_bracketleft,
  ENGINE_KEY_RIGHT_BRACKET = XK_bracketright,
  ENGINE_KEY_MINUS = XK_minus,
  ENGINE_KEY_EQUAL = XK_equal,
  ENGINE_KEY_GRAVE = XK_grave,

  // Modifier keys
  ENGINE_KEY_SHIFT = XK_Shift_L,
  ENGINE_KEY_CTRL = XK_Control_L,
  ENGINE_KEY_ALT = XK_Alt_L,
  ENGINE_KEY_META = XK_Meta_L,
  ENGINE_KEY_SHIFT_R = XK_Shift_R,
  ENGINE_KEY_CTRL_R = XK_Control_R,
  ENGINE_KEY_ALT_R = XK_Alt_R,
  ENGINE_KEY_META_R = XK_Meta_R,

  // Additional keys
  ENGINE_KEY_PRINT_SCREEN = XK_Print,
  ENGINE_KEY_SCROLL_LOCK = XK_Scroll_Lock,
  ENGINE_KEY_PAUSE = XK_Pause,
  ENGINE_KEY_INSERT = XK_Insert,
  ENGINE_KEY_HOME = XK_Home,
  ENGINE_KEY_END = XK_End,
  ENGINE_KEY_PAGE_UP = XK_Page_Up,
  ENGINE_KEY_PAGE_DOWN = XK_Page_Down,

  // Numpad keys
  ENGINE_KEY_NUMPAD_0 = XK_KP_0,
  ENGINE_KEY_NUMPAD_1 = XK_KP_1,
  ENGINE_KEY_NUMPAD_2 = XK_KP_2,
  ENGINE_KEY_NUMPAD_3 = XK_KP_3,
  ENGINE_KEY_NUMPAD_4 = XK_KP_4,
  ENGINE_KEY_NUMPAD_5 = XK_KP_5,
  ENGINE_KEY_NUMPAD_6 = XK_KP_6,
  ENGINE_KEY_NUMPAD_7 = XK_KP_7,
  ENGINE_KEY_NUMPAD_8 = XK_KP_8,
  ENGINE_KEY_NUMPAD_9 = XK_KP_9,
  ENGINE_KEY_NUMPAD_ADD = XK_KP_Add,
  ENGINE_KEY_NUMPAD_SUBTRACT,
  ENGINE_KEY_NUMPAD_MULTIPLY = XK_KP_Multiply,
  ENGINE_KEY_NUMPAD_DIVIDE = XK_KP_Divide,
  ENGINE_KEY_NUMPAD_DECIMAL = XK_KP_Decimal,
  ENGINE_KEY_NUMPAD_ENTER = XK_KP_Enter,

  // Additional keys
  ENGINE_KEY_APPLICATION = XK_Menu,        // Application key
  ENGINE_KEY_CONTEXT_MENU = XK_Menu,       // Context menu key
  ENGINE_KEY_HELP = XK_Help,               // Help key
                                           // Miscellaneous keys
  ENGINE_KEY_NUM_LOCK = XK_Num_Lock,       // Num Lock
  ENGINE_KEY_CAPS_LOCK = XK_Caps_Lock,     // Caps Lock
  ENGINE_KEY_LEFT_CONTROL = XK_Control_L,  // Left Control
  ENGINE_KEY_RIGHT_CONTROL = XK_Control_R, // Right Control
  ENGINE_KEY_LEFT_SHIFT = XK_Shift_L,      // Left Shift
  ENGINE_KEY_RIGHT_SHIFT = XK_Shift_R,     // Right Shift
  ENGINE_KEY_LEFT_ALT = XK_Alt_L,          // Left Alt
  ENGINE_KEY_RIGHT_ALT = XK_Alt_R,         // Right Alt
  ENGINE_KEY_LEFT_META = XK_Meta_L,        // Left Meta
  ENGINE_KEY_RIGHT_META = XK_Meta_R,       // Right Meta

  // Function keys (if not already included)
  ENGINE_KEY_F13 = XK_F13,
  ENGINE_KEY_F14 = XK_F14,
  ENGINE_KEY_F15 = XK_F15,
  ENGINE_KEY_F16 = XK_F16,
  ENGINE_KEY_F17 = XK_F17,
  ENGINE_KEY_F18 = XK_F18,
  ENGINE_KEY_F19 = XK_F19,
  ENGINE_KEY_F20 = XK_F20,
  ENGINE_KEY_F21 = XK_F21,
  ENGINE_KEY_F22 = XK_F22,
  ENGINE_KEY_F23 = XK_F23,
  ENGINE_KEY_F24 = XK_F24,
};

bool engine_key_get(int keycode);

typedef struct vec2 vec2;
typedef struct vec3 vec3;

DECLARE_LIST(vec2)
DECLARE_LIST(vec3)
DECLARE_LIST(GLuint)

#endif // ENGINE_H
