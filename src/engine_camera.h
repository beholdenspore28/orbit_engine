#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

typedef struct {
  mathf_transform_t transform;
  float *matrix;
} camera_t;

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
  glfwGetFramebufferSize(engine_glfw_window, &width, &height);

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

#endif // ENGINE_CAMERA_H
