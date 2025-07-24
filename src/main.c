#include "engine.h"
#include <time.h>

static struct camera camera = {0};

static vec3 light_position = (vec3){10, 10, 0};

static GLuint planet_shader = 0;
static struct mesh planet_mesh = {0};
static GLuint planet_texture = 0;

static struct transform planet_transform = (struct transform){
    .position = (struct vec3){1, 0, 0},
    .scale = (struct vec3){1, 1, 1},
    .rotation = (struct quat){0, 0, 0, 1},
};

static struct mesh quad_mesh = {0};

static struct transform quad_transform = {
    .position = (struct vec3){-1, 0, 0},
    .rotation = (struct quat){0, 0, 0, 1},
    .scale = (struct vec3){1, 1, 1},
};

struct engine_time {
  double FPS, delta, last, current;
} engine_time;

struct engine_time engine_time_instance = {0};

struct engine_time *engine_time_get(void) {
  return &engine_time_instance;
}

void engine_time_update(void) {
  struct timespec spec;
  if (clock_gettime(CLOCK_MONOTONIC, &spec) != 0) {
    engine_error("failed to get time spec.");
    exit(0);
  }

  engine_time_instance.current = spec.tv_sec + spec.tv_nsec * 1e-9;
  engine_time_instance.delta =
      engine_time_instance.current - engine_time_instance.last;
  engine_time_instance.last = engine_time_instance.current;
  engine_time_instance.FPS = 1 / engine_time_instance.delta;

#if 0
  engine_log("TIME: delta %lf | current %lf", engine_time_instance.delta,
             engine_time_instance.current);
#endif
}

void engine_scene_load(void) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0, 0, 0, 1);

  planet_shader = engine_shader_create("res/shaders/planet_vertex.glsl",
                                       "res/shaders/planet_fragment.glsl");

  planet_texture = engine_texture_alloc("res/textures/moon_1.jpeg");
  camera = camera_alloc();
  planet_mesh = engine_mesh_planet_alloc(6, vec3_one(1.0), vec3_zero(), 0.5);

  quad_mesh = engine_mesh_quad_alloc();
}

void engine_scene_unload(void) { glDeleteProgram(planet_shader); }

void engine_draw(struct mesh mesh, struct transform transform, GLuint shader,
                 GLuint texture) {
  {
    GLint camera_matrix_location =
        glGetUniformLocation(shader, "u_camera_matrix");
    glUniformMatrix4fv(camera_matrix_location, 1, GL_FALSE, camera.matrix);
  }

  {
    GLfloat transform_matrix[16];
    mathf_transform_matrix(transform_matrix, &transform);

    GLint model_matrix_location =
        glGetUniformLocation(shader, "u_transform_matrix");
    glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, transform_matrix);
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniform1i(glGetUniformLocation(shader, "u_diffuse_map"), 0);

  glUniform3f(glGetUniformLocation(shader, "u_light_position"),
              light_position.x, light_position.y, light_position.z);

  glUniform3f(glGetUniformLocation(shader, "u_camera_position"),
              camera.transform.position.x, camera.transform.position.y,
              camera.transform.position.z);

  glBindVertexArray(mesh.VAO);
  if (mesh.use_indexed_draw) {
    glDrawElements(GL_TRIANGLES, mesh.indices_count, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}

void engine_scene_update(void) {
  vec3 look_angles = (vec3){
      engine_key_get(ENGINE_KEY_K) - engine_key_get(ENGINE_KEY_J),
      engine_key_get(ENGINE_KEY_L) - engine_key_get(ENGINE_KEY_H),
      engine_key_get(ENGINE_KEY_I) - engine_key_get(ENGINE_KEY_O),
  };

  vec3_scale(&look_angles, engine_time_get()->delta * 2);

  camera.transform.rotation =
      quat_rotate_euler(camera.transform.rotation, look_angles);

  struct vec3 movedir = (struct vec3){
      engine_key_get(ENGINE_KEY_D) - engine_key_get(ENGINE_KEY_A),
      engine_key_get(ENGINE_KEY_SPACE) - engine_key_get(ENGINE_KEY_SHIFT),
      engine_key_get(ENGINE_KEY_W) - engine_key_get(ENGINE_KEY_S),
  };

  vec3_normalize(&movedir);
  vec3_scale(&movedir, 0.1);

  // translate to local directions
  movedir = vec3_rotate(movedir, camera.transform.rotation);

  // engine_log(MATHF_vec3_FORMAT_STRING(movedir));
  vec3_add(&camera.transform.position, movedir);

  camera_update(&camera);
  planet_transform.rotation = quat_rotate_euler(
      planet_transform.rotation, vec3_one(engine_time_get()->delta * 0.000729));

  quad_transform.rotation =
      quat_rotate_euler(quad_transform.rotation, vec3_up(0.005));
}

void engine_scene_draw(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(planet_shader);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  engine_draw(planet_mesh, planet_transform, planet_shader, planet_texture);
  engine_draw(quad_mesh, quad_transform, planet_shader, planet_texture);
}

int main() {
  engine_start();
  engine_scene_load();

  while (engine_is_running()) {
    engine_time_update();
    engine_scene_update();
    engine_scene_draw();
    engine_update();

    if (engine_key_get(ENGINE_KEY_ESCAPE)) {
      break;
    }
  }

  engine_scene_unload();
  engine_stop();
}
