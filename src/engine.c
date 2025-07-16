#include <stdio.h>
#include <stdlib.h>

// the order of these header files is extremely important!
// each header contains both API and implementation.
#include "engine_logging.h"
#include "engine_mathf.h"
#include "engine_gl.h"
#include "engine_list.h"
#include "engine_list_definitions.h"
#include "engine_file.h"
#include "engine_shader.h"
#include "engine_mesh.h"
#include "engine_camera.h"

void engine_run(void) {

  GLuint hello_triangle_shader =
      engine_shader_create("res/shaders/hello_triangle_vertex.glsl",
                           "res/shaders/hello_triangle_fragment.glsl");

  camera_t camera = camera_alloc();

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

int main() {
  engine_log("Rev up those fryers!");
  engine_start();
  engine_run();
  engine_stop();
}
