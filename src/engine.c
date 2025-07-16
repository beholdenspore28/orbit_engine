#include "engine.h"
#include "list.h"
#include "mathf.h"

#include <stdio.h>
#include <stdlib.h>

// holds data required for basic window management, graphics context, etc..
typedef struct {
  GLFWwindow *window;
} engine;

DEFINE_LIST(vector2)
DEFINE_LIST(vector3)
DEFINE_LIST(GLuint)

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

  engine_context.window = glfwCreateWindow(640, 480, "Game Window", NULL, NULL);
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

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glfwSetFramebufferSizeCallback(engine_context.window,
                                 engine_glfw_framebuffer_size_callback);

  glfwSwapInterval(1);

  return EXIT_SUCCESS;
}

// cleans up allocated memory and stops the engine
static inline void engine_stop(void) { glfwTerminate(); }

void game_main(void);

camera_t camera_alloc(void) {
  return (camera_t){
      .transform =
          (mathf_transform_t){
              .position = (vector3){0, 0, -3},
              .rotation = (quaternion){0, 0, 0, 1},
              .scale = (vector3){1, 1, 1},
          },
      .matrix = calloc(16, sizeof(GLfloat)),
  };
}

void camera_update(camera_t *camera) {
  int width, height;
  glfwGetFramebufferSize(engine_context.window, &width, &height);

  const GLfloat aspect = (GLfloat)width / height;

  GLfloat projection[16];
  mathf_mat4_identity(projection);
  mathf_mat4_perspective(projection, 70 * (3.14159 / 180.0), aspect, 0.0001,
                         1000);
  // mat4_orthographic(projection, -9, 9, -16, 16, 0.1, 75);

  vector3 offset =
      vector3_rotate((vector3){0, 0, -1}, camera->transform.rotation);

  GLfloat translation_matrix[16];
  mathf_mat4_identity(translation_matrix);
  translation_matrix[12] = -camera->transform.position.x + offset.x;
  translation_matrix[13] = -camera->transform.position.y + offset.y;
  translation_matrix[14] = -camera->transform.position.z + offset.z;

  GLfloat rotation_matrix[16];
  mathf_mat4_identity(rotation_matrix);
  quat_to_mat4(quat_conjugate(camera->transform.rotation), rotation_matrix);

  mathf_mat4_multiply(camera->matrix, translation_matrix, rotation_matrix);
  mathf_mat4_multiply(camera->matrix, camera->matrix, projection);
}

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

  camera_t camera = camera_alloc();

#if 0
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
#endif

  mesh_t planet = mesh_planet_alloc(2, 0.1);
  mathf_transform_t planet_transform = (mathf_transform_t){
      .position = (vector3){0, 0, 0},
      .scale = (vector3){1, 1, 1},
      .rotation = (quaternion){0, 0, 0, 1},
  };

  while (!glfwWindowShouldClose(engine_context.window)) {
    camera_update(&camera);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(hello_triangle_shader);

    {
      GLint camera_matrix_location =
          glGetUniformLocation(hello_triangle_shader, "u_camera_matrix");
      glUniformMatrix4fv(camera_matrix_location, 1, GL_FALSE, camera.matrix);
    }

    {
      GLfloat transform_matrix[16];
      mathf_transform_matrix(transform_matrix, &planet_transform);

      GLint model_matrix_location =
          glGetUniformLocation(hello_triangle_shader, "u_transform_matrix");
      glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, transform_matrix);
    }

    glBindVertexArray(planet.VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, planet.indices_count, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0);

    glfwSwapBuffers(engine_context.window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &planet.VAO);
  glDeleteBuffers(1, &planet.VBOs[0]);
  glDeleteBuffers(1, &planet.EBO);
  glDeleteProgram(hello_triangle_shader);
}
