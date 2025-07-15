#ifndef ENGINE_H
#define ENGINE_H

#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ENGINE_PRAGMA(x) _Pragma(#x)

#define ENGINE_NOWARN(warnoption, ...)                                         \
  ENGINE_PRAGMA(GCC diagnostic push)                                           \
  ENGINE_PRAGMA(GCC diagnostic ignored warnoption)                             \
  __VA_ARGS__                                                                  \
  ENGINE_PRAGMA(GCC diagnostic pop)

#ifndef ENGINE_LOG_STREAM
#define ENGINE_LOG_STREAM stdout
#endif

#ifndef ENGINE_WARNING_STREAM
#define ENGINE_WARNING_STREAM ENGINE_LOG_STREAM
#endif

#ifndef ENGINE_ERROR_STREAM
#define ENGINE_ERROR_STREAM stderr
#endif

#define ENGINE_ANSI_COLOR_RED "\x1b[31m"
#define ENGINE_ANSI_COLOR_GREEN "\x1b[32m"
#define ENGINE_ANSI_COLOR_YELLOW "\x1b[33m"
#define ENGINE_ANSI_COLOR_BLUE "\x1b[34m"
#define ENGINE_ANSI_COLOR_MAGENTA "\x1b[35m"
#define ENGINE_ANSI_COLOR_CYAN "\x1b[36m"
#define ENGINE_ANSI_COLOR_RESET "\x1b[0m"

#define engine_log(...)                                                        \
  {                                                                            \
    fprintf(ENGINE_LOG_STREAM, ENGINE_ANSI_COLOR_CYAN "[ MESSAGE %s:%d ",      \
            __FILE__, __LINE__);                                               \
    fprintf(ENGINE_LOG_STREAM,                                                 \
            ENGINE_ANSI_COLOR_CYAN "] " ENGINE_ANSI_COLOR_RESET __VA_ARGS__);  \
    fprintf(ENGINE_LOG_STREAM, "\n" ENGINE_ANSI_COLOR_RESET);                  \
  }

#define engine_warn(...)                                                       \
  {                                                                            \
    fprintf(ENGINE_WARNING_STREAM,                                             \
            ENGINE_ANSI_COLOR_YELLOW "[ WARNING %s:%d ", __FILE__, __LINE__);  \
    fprintf(ENGINE_WARNING_STREAM, ENGINE_ANSI_COLOR_YELLOW                    \
            "] " ENGINE_ANSI_COLOR_RESET __VA_ARGS__);                         \
    fprintf(ENGINE_WARNING_STREAM, "\n" ENGINE_ANSI_COLOR_RESET);              \
  }

#define engine_error(...)                                                      \
  {                                                                            \
    fprintf(ENGINE_ERROR_STREAM, ENGINE_ANSI_COLOR_RED "[ ERROR %s:%d ",       \
            __FILE__, __LINE__);                                               \
    fprintf(ENGINE_ERROR_STREAM,                                               \
            ENGINE_ANSI_COLOR_RED "] " ENGINE_ANSI_COLOR_RESET __VA_ARGS__);   \
    fprintf(ENGINE_ERROR_STREAM, "\n" ENGINE_ANSI_COLOR_RESET);                \
  }

#define ENGINE_FILE_CHUNK_SIZE (64 /* chars */)
#define ENGINE_FILE_GROWTH (4 /* times */)

typedef struct {
  size_t length;
  char *text;
  int error;
} engine_file_t;

engine_file_t engine_file_load_as_string(const char *filename);
void engine_file_free(const engine_file_t file);

GLuint engine_shader_create(const char *vertex_shader_file_path,
                            const char *fragment_shader_file_path);

#endif // ENGINE_H
