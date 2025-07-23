#include "engine.h"

struct camera camera_alloc(void) {
  return (struct camera){
      .transform =
          (struct transform){
              .position = (struct vec3){0, 0, -3},
              .rotation = (struct quat){0, 0, 0, 1},
              .scale = (vec3){1, 1, 1},
          },
      .matrix = calloc(16, sizeof(GLfloat)),
  };
}

void camera_update(struct camera *camera) {

  const GLfloat aspect = engine_get_aspect_ratio();

  GLfloat projection[16];
  mathf_mat4_identity(projection);
  mathf_mat4_perspective(projection, 70 * (3.14159 / 180.0), aspect, 0.0001,
                         1000);
  // mat4_orthographic(projection, -9, 9, -16, 16, 0.1, 75);

  vec3 offset =
      vec3_rotate((vec3){0, 0, -1}, camera->transform.rotation);

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
