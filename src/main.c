#include "engine.h"

DEFINE_LIST(vector2)
DEFINE_LIST(vector3)
DEFINE_LIST(GLuint)

const char *window_title = "Game Window";
bool engine_running = false;
const int window_width = 854, window_height = 480;

void engine_update(void) {

  GLuint hello_triangle_shader =
      engine_shader_create("res/shaders/hello_triangle_vertex.glsl",
                           "res/shaders/hello_triangle_fragment.glsl");

  struct camera camera = camera_alloc();

  struct mesh planet = mesh_planet_alloc(2, 0.1);
  struct transform planet_transform = (struct transform){
      .position = (vector3){0, 0, 0},
      .scale = (vector3){1, 1, 1},
      .rotation = (struct quaternion){0, 0, 0, 1},
  };

  while (!engine_running) {
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

    engine_update_glx();
  }

  glDeleteVertexArrays(1, &planet.VAO);
  glDeleteBuffers(1, &planet.VBOs[0]);
  glDeleteBuffers(1, &planet.EBO);
  glDeleteProgram(hello_triangle_shader);
}

int main() {
  engine_start_glx();
  engine_update();
  engine_stop_glx();
}
