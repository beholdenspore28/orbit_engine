#include "engine.h"

DEFINE_LIST(vector2)
DEFINE_LIST(vector3)
DEFINE_LIST(GLuint)

static GLuint hello_triangle_shader = 0;
static struct camera camera = {0};
static struct mesh planet_mesh = {0};

static struct transform planet_transform = (struct transform){
  .position = (vector3){0, 0, 0},
    .scale = (vector3){1, 1, 1},
    .rotation = (struct quaternion){0, 0, 0, 1},
};

void engine_scene_load(void) {
  glClearColor(0.2,0.3,0.4,1);

  hello_triangle_shader =
      engine_shader_create("res/shaders/hello_triangle_vertex.glsl",
                           "res/shaders/hello_triangle_fragment.glsl");
  camera = camera_alloc();
  planet_mesh = mesh_planet_alloc(2, 0.1);
}

void engine_scene_unload(void) {
  glDeleteVertexArrays(1, &planet_mesh.VAO);
  glDeleteBuffers(1, &planet_mesh.VBOs[0]);
  glDeleteBuffers(1, &planet_mesh.EBO);
  glDeleteProgram(hello_triangle_shader);
}

void engine_scene_update(void) {
    camera_update(&camera);
}

void engine_scene_draw(void) {
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

    glBindVertexArray(planet_mesh.VAO);
    glDrawElements(GL_TRIANGLES, planet_mesh.indices_count, GL_UNSIGNED_INT, 0);
}
