#include "engine.h"
#include <time.h>

static struct camera camera = {0};

static vec3 light_position = (vec3){10, 10, 0};

static GLuint planet_shader = 0;
static struct mesh planet_mesh = {0};
static GLuint planet_texture = 0;

static struct transform planet_transform = (struct transform){
    .position = (struct vec3){0, 0, 2000},
    .scale = (struct vec3){1000, 1000, 1000},
    .rotation = (struct quat){0, 0, 0, 1},
};
static GLuint planet_atmosphere_shader = 0;
static struct mesh planet_atmosphere_mesh = {0};
static struct transform planet_atmosphere_transform = {0};

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

  float amplitude = 0.1;
  planet_mesh = engine_mesh_planet_alloc(6, vec3_one(1.0), vec3_zero(), amplitude);

  planet_atmosphere_shader = engine_shader_create("res/shaders/planet_vertex.glsl",
                                       "res/shaders/planet_atmosphere_fragment.glsl");
  planet_atmosphere_mesh = engine_mesh_planet_alloc(6, vec3_one(1.0), vec3_zero(), 0);
  planet_atmosphere_mesh.use_clockwise_winding = true;
  planet_atmosphere_transform = planet_transform;
  planet_atmosphere_transform.scale = vec3_scaled(planet_transform.scale, amplitude * 12);

  quad_mesh = engine_mesh_quad_alloc();
}

void engine_draw(struct mesh mesh, struct transform transform, GLuint shader,
                 GLuint texture) {
  if (mesh.use_clockwise_winding) {
    glFrontFace(GL_CW);
  } else {
    glFrontFace(GL_CCW);
  }
  glUseProgram(shader);
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

  vec3 look_angles = vec3_zero();
  look_angles.z = 5.0 * (engine_key_get(ENGINE_KEY_Q) - engine_key_get(ENGINE_KEY_E));
  engine_mouse_delta_get(&look_angles.y, &look_angles.x);
  vec3_scale(&look_angles, engine_time_get()->delta * 0.1);

  camera.transform.rotation =
      quat_rotate_euler(camera.transform.rotation, look_angles);

  struct vec3 movedir = (struct vec3){
      engine_key_get(ENGINE_KEY_D) - engine_key_get(ENGINE_KEY_A),
      engine_key_get(ENGINE_KEY_SPACE) - engine_key_get(ENGINE_KEY_SHIFT),
      engine_key_get(ENGINE_KEY_W) - engine_key_get(ENGINE_KEY_S),
  };

  vec3_normalize(&movedir);
  float speed = engine_key_get(ENGINE_KEY_CTRL) ? 0.4 : 4.0;
  vec3_scale(&movedir, speed);

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

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  engine_draw(planet_mesh, planet_transform, planet_shader, planet_texture);
  engine_draw(planet_atmosphere_mesh, planet_atmosphere_transform, planet_atmosphere_shader, planet_texture);
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

  engine_stop();
}
