#include "mathf.h"
#include "engine.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>

// holds data required for basic window management, graphics context, etc..
typedef struct {
  GLFWwindow *window;
} engine;

DECLARE_LIST(vector2)
DECLARE_LIST(vector3)
DECLARE_LIST(GLuint)

// global engine context
static engine engine_context;
static GLuint hello_triangle_shader = 0;

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
static inline int engine_start(void) {

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

  engine_context.window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
  if (!engine_context.window) {
    engine_error("Window or OpenGL context creation failed");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(engine_context.window);
  if (!gladLoadGL(glfwGetProcAddress)) {
    engine_error("failed to initialize GLAD!");
    return EXIT_FAILURE;
  };

  glfwSetFramebufferSizeCallback(engine_context.window,
                                 engine_glfw_framebuffer_size_callback);

  glfwSwapInterval(1);

  return EXIT_SUCCESS;
}

// cleans up allocated memory and stops the engine
static inline void engine_stop(void) { glfwTerminate(); }

void game_main(void);

int main() {
  engine_log("Rev up those fryers!");
  engine_start();
  game_main();
  engine_stop();
}

void game_main(void) {

  hello_triangle_shader =
      engine_shader_create("res/shaders/hello_triangle_vertex.glsl",
                           "res/shaders/hello_triangle_fragment.glsl");

  float vertices[] = {
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f  // top left
  };
  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first Triangle
      1, 2, 3  // second Triangle
  };

  unsigned int VBO, VAO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(engine_context.window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(hello_triangle_shader);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0);

    glfwSwapBuffers(engine_context.window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(hello_triangle_shader);
}
