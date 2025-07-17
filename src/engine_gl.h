#ifndef ENGINE_GL_H
#define ENGINE_GL_H

#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// holds data required for basic window management, graphics context, etc..
static GLFWwindow *engine_glfw_window;

// function to intercept and log errors thrown by GLFW
void engine_glfw_error_callback(int error, const char *description) {
  (void)error;
  engine_error("%s\n", description);
}

void engine_glfw_framebuffer_size_callback(GLFWwindow *window, int width,
                                           int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

// allocates bare essentials and starts the engine.
static inline int engine_glfw_start(void) {

  engine_log("Rev up those fryers!");

  if (!glfwInit()) {
    engine_error("Initialization failed");
    return EXIT_FAILURE;
  }

  glfwSetErrorCallback(engine_glfw_error_callback);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  engine_glfw_window = glfwCreateWindow(640, 480, "Game Window", NULL, NULL);
  if (!engine_glfw_window) {
    engine_error("Window or OpenGL context creation failed");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(engine_glfw_window);
  if (!gladLoadGL(glfwGetProcAddress)) {
    engine_error("failed to initialize GLAD!");
    return EXIT_FAILURE;
  };

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glfwSetFramebufferSizeCallback(engine_glfw_window,
                                 engine_glfw_framebuffer_size_callback);

  glfwSwapInterval(1);

  return EXIT_SUCCESS;
}

// cleans up allocated memory and stops the engine
static inline void engine_stop(void) { glfwTerminate(); }

#endif // ENGINE_GL_H
