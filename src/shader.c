#include "engine.h"

GLuint engine_shader_compile_source(const char *file_path,
                                    uint32_t shader_type) {
  engine_file_t file = engine_file_load_as_string(file_path);

  GLuint shader = glCreateShader(shader_type);
  const char *shader_source = file.text;

  glShaderSource(shader, 1, &shader_source, NULL);
  glCompileShader(shader);

  engine_file_free(file);

  GLint success;
  char info_log[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    engine_error("vertex shader compilation failed%s", info_log);
  }

  return shader;
}

GLuint engine_shader_create(const char *vertex_shader_file_path,
                            const char *fragment_shader_file_path) {

  engine_log("creating shader program from:\n\t%s\n\t%s",
             vertex_shader_file_path, fragment_shader_file_path);

  GLuint vertex_shader =
      engine_shader_compile_source(vertex_shader_file_path, GL_VERTEX_SHADER);

  GLuint fragment_shader = engine_shader_compile_source(
      fragment_shader_file_path, GL_FRAGMENT_SHADER);

  GLuint shader_program;
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return shader_program;
}

