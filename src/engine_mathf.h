#ifndef MATHF_H
#define MATHF_H

#include <math.h>
#include <stdio.h>

#define MATHF_FLOAT_EPSILON (1e-4)
#define MATHF_PI (3.14159265358)

struct quat {
  float x, y, z, w;
};

struct vec2 {
  float x, y;
};

#define vec2_one(magnitude)                                                    \
  (struct vec2) { 1, 1 }
#define vec2_zero(magnitude)                                                   \
  (struct vec2) { 0, 0, 0 }
#define vec2_up(magnitude)                                                     \
  (struct vec2) { 0, 1 }
#define vec2_down(magnitude)                                                   \
  (struct vec2) { 0, -1 }
#define vec2_left(magnitude)                                                   \
  (struct vec2) { -1, 0 }
#define vec2_right(magnitude)                                                  \
  (struct vec2) { 1, 0 }

struct vec3 {
  float x, y, z;
};

#define vec3_one(magnitude)                                                    \
  (struct vec3) { magnitude, magnitude, magnitude }
#define vec3_zero(magnitude)                                                   \
  (struct vec3) { 0, 0, 0 }
#define vec3_up(magnitude)                                                     \
  (struct vec3) { 0, magnitude, 0 }
#define vec3_down(magnitude)                                                   \
  (struct vec3) { 0, -magnitude, 0 }
#define vec3_left(magnitude)                                                   \
  (struct vec3) { -magnitude, 0, 0 }
#define vec3_right(magnitude)                                                  \
  (struct vec3) { magnitude, 0, 0 }
#define vec3_forward(magnitude)                                                \
  (struct vec3) { 0, 0, magnitude }
#define vec3_back(magnitude)                                                   \
  (struct vec3) { 0, 0, -magnitude }

struct vec4 {
  float x, y, z, w;
};

#define vec4_one(magnitude)                                                    \
  (struct vec4) { magnitude, magnitude, magnitude }
#define vec4_zero(magnitude)                                                   \
  (struct vec4) { 0, 0, 0 }
#define vec4_up(magnitude)                                                     \
  (struct vec4) { 0, magnitude, 0 }
#define vec4_down(magnitude)                                                   \
  (struct vec4) { 0, -magnitude, 0 }
#define vec4_left(magnitude)                                                   \
  (struct vec4) { -magnitude, 0, 0 }
#define vec4_right(magnitude)                                                  \
  (struct vec4) { magnitude, 0, 0 }
#define vec4_forward(magnitude)                                                \
  (struct vec4) { 0, 0, magnitude }
#define vec4_back(magnitude)                                                   \
  (struct vec4) { 0, 0, -magnitude }
#define vec4_future(magnitude)                                                 \
  (struct vec4) { 0, 0, 0, magnitude }
#define vec4_past(magnitude)                                                   \
  (struct vec4) { 0, 0, 0, -magnitude }

struct transform {
  struct vec3 position;
  struct quat rotation;
  struct vec3 scale;
};

#define mathf_fmod fmodf
#define mathf_fabs fabsf
#define mathf_sin sinf
#define mathf_cos cosf
#define mathf_pow powf
#define mathf_floor floorf

#define MATHF_VEC4_FORMAT_STRING(v) "{%f %f %f %f}\n", v.x, v.y, v.z, v.w
#define MATHF_VEC3_FORMAT_STRING(v) "{%f %f %f}\n", v.x, v.y, v.z
#define MATHF_VEC2_FORMAT_STRING(v) "{%f %f}\n", v.x, v.y

#define MATHF_QUAT_FORMAT_STRING(q) "{%f %f %f %f}\n", q.x, q.y, q.z, q.w

#define MATHF_MATRIX4_FORMAT_STRING(matrix)                                    \
  "\n%.2f, %.2f, %.2f, %.2f\n"                                                 \
  "%.2f, %.2f, %.2f, %.2f\n"                                                   \
  "%.2f, %.2f, %.2f, %.2f\n"                                                   \
  "%.2f, %.2f, %.2f, %.2f\n",                                                  \
      matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5],        \
      matrix[6], matrix[7], matrix[8], matrix[9], matrix[10], matrix[11],      \
      matrix[12], matrix[13], matrix[14], matrix[15]

static inline float mathf_sign(const float n) { return n >= 0; }

static inline float mathf_rad2deg(const float n) {
  return n * (180.0 / MATHF_PI);
}

static inline float mathf_deg2rad(const float n) {
  return n * (MATHF_PI / 180.0);
}

static inline float mathf_max(float a, float b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

static inline float mathf_min(float a, float b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

static inline void mathf_swap(float *a, float *b) {
  float tmp = *a;
  *a = *b;
  *b = tmp;
}

static inline float mathf_wrap_max(float n, float max) {
  return mathf_fmod(max + mathf_fmod(n, max), max);
}

static inline float mathf_wrap(float n, float min, float max) {
  return min + mathf_wrap_max(n - min, max - min);
}

static inline float mathf_clamp(float n, const float min, const float max) {
  n = n < min ? min : n;
  return n > max ? max : n;
}

static inline float mathf_clamp01(float n) {
  n = n < 0 ? 0 : n;
  return n > 1 ? 1 : n;
}

static inline float mathf_lerp(float a, float b, float t) {
  return a + (b - a) * t;
}

static inline float mathf_lerpclamped(float a, float b, float t) {
  return a + (b - a) * mathf_clamp01(t);
}

static inline float mathf_norm(float n, float min, float max) {
  return (n - min) / (max - min);
}

static inline float mathf_map(float n, float fromMin, float fromMax,
                              float toMin, float toMax) {
  return (n - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}

static inline int mathf_equal(float a, float b, float tolerance) {
  return (mathf_fabs(a - b) < tolerance);
}

// cosine interpolation
static inline float mathf_cerp(float a, float b, float t) {
  float f = (1.0 - mathf_cos(t * MATHF_PI)) * 0.5;
  return a * (1.0 - f) + b * f;
}

static inline float mathf_loop(float n, const float length) {
  return mathf_clamp(n - mathf_floor(n / length) * length, 0.0, length);
}

static inline float mathf_ping_pong(float n, const float length) {
  n = mathf_loop(n, length * 2.0);
  return mathf_fabs(n - length);
}

static inline float mathf_angle_delta(const float a, const float b) {
  float delta = mathf_loop((b - a), 360.0);
  if (delta > 180.0) {
    delta -= 360.0;
  }
  return delta;
}

static inline float mathf_fraction(float x) { return x - mathf_floor(x); }

// Single dimensional pseudo-random noise
static inline float mathf_noise1(float x) {
  float wave = mathf_sin(x * 53) * 6151;
  return mathf_fraction(wave);
}

// Two dimensional pseudo-random noise
static inline float mathf_noise2(float x, float y) {
  float wave = mathf_sin(x * 53 + y * 97) * 6151;
  return mathf_fraction(wave);
}

// Three dimensional pseudo-random noise
static inline float mathf_noise3(float x, float y, float z) {
  float wave = mathf_sin(x * 53 + y * 97 + z * 193) * 6151;
  return mathf_fraction(wave);
}

// Three dimensional pseudo-random noise
static inline float mathf_noise3_interpolated(float x, float y, float z) {
  float fractX = mathf_fraction(x), fractY = mathf_fraction(y),
        fractZ = mathf_fraction(z), floorX = mathf_floor(x),
        floorY = mathf_floor(y), floorZ = mathf_floor(z);

  // interpolate between adjacent noise values
  // ==================================================
  // two vertices 'v' make an edge 'e'
  // two edges make a face 'f'
  // two faces make a cube.
  // ==================================================

  //===================================================
  float v1 = mathf_noise3(floorX, floorY, floorZ),
        v2 = mathf_noise3(floorX + 1, floorY, floorZ),
        e1 = mathf_cerp(v1, v2, fractX), // rear bottom

      v3 = mathf_noise3(floorX, floorY + 1, floorZ),
        v4 = mathf_noise3(floorX + 1, floorY + 1, floorZ),
        e2 = mathf_cerp(v3, v4, fractX), // rear top

      v5 = mathf_noise3(floorX, floorY, floorZ + 1),
        v6 = mathf_noise3(floorX + 1, floorY, floorZ + 1),
        e3 = mathf_cerp(v5, v6, fractX), // front bottom

      v7 = mathf_noise3(floorX, floorY + 1, floorZ + 1),
        v8 = mathf_noise3(floorX + 1, floorY + 1, floorZ + 1),
        e4 = mathf_cerp(v7, v8, fractX), // front top

      f1 = mathf_cerp(e1, e2, fractY), f2 = mathf_cerp(e3, e4, fractY),
        cube = mathf_cerp(f1, f2, fractZ);

  return cube;
}

static inline float mathf_noise3_fbm(float x, float y, float z) {
  float total = 0.0;
  float freq = 1.0;
  float amplitude = 1.0;
  float persistance = 0.5;
  int octaves = 16;
  for (int i = 0; i < octaves; i++) {
    freq = mathf_pow(2, i);
    amplitude = mathf_pow(persistance, i);
    total +=
        mathf_noise3_interpolated(x * freq, y * freq, z * freq) * amplitude;
  }
  return total;
}

static inline float mathf_noise3_fbm_warped(float x, float y, float z,
                                            float warpFactor) {
  float fbm1 = mathf_noise3_fbm(x, y, z);
  float fbm2 =
      mathf_noise3_fbm(x + 5.2, y + 1.3 * warpFactor, z + 6.4 * warpFactor);
  float fbm3 =
      mathf_noise3_fbm(x + 7.5, y + 0.3 * warpFactor, z + 3.6 * warpFactor);
  return mathf_noise3_fbm(fbm1, fbm2, fbm3);
}

static inline void vec2_sub(struct vec2 *a, struct vec2 b) {
  a->x -= b.x;
  a->y -= b.y;
}

static inline struct vec2 vec2_subbed(struct vec2 a, struct vec2 b) {
  return (struct vec2){
      a.x - b.x,
      a.y - b.y,
  };
}

static inline void vec2_add(struct vec2 *a, struct vec2 b) {
  a->x += b.x;
  a->y += b.y;
}

static inline struct vec2 vec2_added(struct vec2 a, struct vec2 b) {
  return (struct vec2){
      a.x + b.x,
      a.y + b.y,
  };
}

static inline void vec2_scale(struct vec2 *v, const float s) {
  v->x *= s;
  v->y *= s;
}

static inline struct vec2 vec2_scaled(const struct vec2 v, const float s) {
  return (struct vec2){
      v.x * s,
      v.y * s,
  };
}

static inline float vec2_square_magnitude(const struct vec2 v) {
  return v.x * v.x + v.y * v.y;
}

static inline float vec2_magnitude(const struct vec2 v) {
  return sqrtf(vec2_square_magnitude(v));
}

static inline float vec2_square_distance(const struct vec2 a,
                                         const struct vec2 b) {
  return vec2_square_magnitude(vec2_subbed(b, a));
}

static inline float vec2_distance(const struct vec2 a, const struct vec2 b) {
  return vec2_magnitude(vec2_subbed(b, a));
}

static inline float vec2_dot(const struct vec2 a, const struct vec2 b) {
  return a.x * b.x + a.y * b.y;
}

static inline struct vec2 vec2_lerp(struct vec2 a, struct vec2 b, float t) {
  return (struct vec2){
      .x = a.x + (b.x - a.x) * t,
      .y = a.y + (b.y - a.y) * t,
  };
}

static inline struct vec2 vec2_min(struct vec2 a, struct vec2 b) {
  return (struct vec2){
      .x = a.x <= b.x ? a.x : b.x,
      .y = a.y <= b.y ? a.y : b.y,
  };
}

static inline struct vec2 vec2_max(struct vec2 a, struct vec2 b) {
  return (struct vec2){
      .x = a.x >= b.x ? a.x : b.x,
      .y = a.y >= b.y ? a.y : b.y,
  };
}

static inline int vec2_equal(struct vec2 a, struct vec2 b) {
  return fabs(a.x - b.x) <= MATHF_FLOAT_EPSILON &&
         fabs(a.y - b.y) <= MATHF_FLOAT_EPSILON;
}

static inline float vec2_angle(struct vec2 v1, struct vec2 v2) {
  float dot = vec2_dot(v1, v2);
  float mag1 = vec2_magnitude(v1);
  float mag2 = vec2_magnitude(v2);

  // Handle potential division by zero if a vec has zero magnitude
  if (mag1 == 0 || mag2 == 0) {
    return 0.0;
  }

  float cos_theta = dot / (mag1 * mag2);
  cos_theta = mathf_clamp(cos_theta, -1.0, 1.0);

  return acos(cos_theta); // Returns angle in radians
}

static inline struct vec2 vec2_normalized(struct vec2 v) {
  float m = vec2_magnitude(v);
  if (m == 0) { // prevent NAN values
    return (struct vec2){0, 0};
  }
  return vec2_scaled(v, 1 / m);
}

static inline void vec2_normalize(struct vec2 *v) {
  float m = vec2_magnitude(*v);
  if (m == 0) { // prevent NAN values
    *v = (struct vec2){0, 0};
    return;
  }
  vec2_scale(v, 1 / m);
}

static inline void vec3_scale(struct vec3 *v, const float s) {
  v->x *= s;
  v->y *= s;
  v->z *= s;
}

static inline struct vec3 vec3_scaled(const struct vec3 v, const float s) {
  return (struct vec3){
      v.x * s,
      v.y * s,
      v.z * s,
  };
}

static inline void vec3_sub(struct vec3 *a, struct vec3 b) {
  a->x -= b.x;
  a->y -= b.y;
  a->z -= b.z;
}

static inline struct vec3 vec3_subbed(struct vec3 a, struct vec3 b) {
  return (struct vec3){
      a.x - b.x,
      a.y - b.y,
      a.z - b.z,
  };
}

static inline void vec3_add(struct vec3 *a, struct vec3 b) {
  a->x += b.x;
  a->y += b.y;
  a->z += b.z;
}

static inline struct vec3 vec3_added(struct vec3 a, struct vec3 b) {
  return (struct vec3){
      a.x + b.x,
      a.y + b.y,
      a.z + b.z,
  };
}

static inline float vec3_square_magnitude(const struct vec3 v) {
  return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline float vec3_magnitude(const struct vec3 v) {
  return sqrtf(vec3_square_magnitude(v));
}

static inline struct vec3 vec3_normalized(struct vec3 v) {
  float m = vec3_magnitude(v);
  if (m == 0) { // prevent NAN values
    return (struct vec3){0, 0, 0};
  }
  return vec3_scaled(v, 1 / m);
}

static inline void vec3_normalize(struct vec3 *v) {
  float m = vec3_magnitude(*v);
  if (m == 0) { // prevent NAN values
    *v = (struct vec3){0, 0, 0};
    return;
  }
  vec3_scale(v, 1 / m);
}

static inline float vec3_square_distance(const struct vec3 a,
                                         const struct vec3 b) {
  return vec3_square_magnitude(vec3_subbed(b, a));
}

static inline float vec3_distance(const struct vec3 a, const struct vec3 b) {
  return vec3_magnitude(vec3_subbed(b, a));
}

static inline float vec3_dot(const struct vec3 a, const struct vec3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline struct vec3 vec3_cross(struct vec3 a, struct vec3 b) {
  return (struct vec3){.x = (a.y * b.z) - (a.z * b.y),
                       .y = -((a.x * b.z) - (a.z * b.x)),
                       .z = (a.x * b.y) - (a.y * b.x)};
}

static inline struct vec3 vec3_lerp(struct vec3 a, struct vec3 b, float t) {
  return (struct vec3){
      .x = a.x + (b.x - a.x) * t,
      .y = a.y + (b.y - a.y) * t,
      .z = a.z + (b.z - a.z) * t,
  };
}

static inline struct vec3 vec3_min(struct vec3 a, struct vec3 b) {
  return (struct vec3){
      .x = a.x <= b.x ? a.x : b.x,
      .y = a.y <= b.y ? a.y : b.y,
      .z = a.z <= b.z ? a.z : b.z,
  };
}

static inline struct vec3 vec3_max(struct vec3 a, struct vec3 b) {
  return (struct vec3){
      .x = a.x >= b.x ? a.x : b.x,
      .y = a.y >= b.y ? a.y : b.y,
      .z = a.z >= b.z ? a.z : b.z,
  };
}

static inline struct vec3 vec3_noise(unsigned int seed, float range) {
  float d, x, y, z;
  int i = 0;
  do {
    x = mathf_noise3(i + seed + 1, i + seed, i + seed) * (2.0) - 1.0;
    y = mathf_noise3(i + seed, i + seed + 1, i + seed) * (2.0) - 1.0;
    z = mathf_noise3(i + seed, i + seed, i + seed + 1) * (2.0) - 1.0;
    d = x * x + y * y + z * z;
    i++;
  } while (d > 1.0);
  return vec3_scaled((struct vec3){x, y, z}, range);
}

static inline int vec3_equal(struct vec3 a, struct vec3 b) {
  return fabs(a.x - b.x) <= MATHF_FLOAT_EPSILON &&
         fabs(a.y - b.y) <= MATHF_FLOAT_EPSILON &&
         fabs(a.z - b.z) <= MATHF_FLOAT_EPSILON;
}

static inline float vec3_angle(struct vec3 v1, struct vec3 v2) {
  float dot = vec3_dot(v1, v2);
  float mag1 = vec3_magnitude(v1);
  float mag2 = vec3_magnitude(v2);

  // Handle potential division by zero if a vec has zero magnitude
  if (mag1 == 0 || mag2 == 0) {
    return 0.0;
  }

  float cos_theta = dot / (mag1 * mag2);
  cos_theta = mathf_clamp(cos_theta, -1.0, 1.0);

  return acos(cos_theta); // Returns angle in radians
}

static inline struct quat quat_from_angle_axis(float angle, struct vec3 axis) {
  struct quat ret;
  float s = sinf(angle / 2);
  ret.x = axis.x * s;
  ret.y = axis.y * s;
  ret.z = axis.z * s;
  ret.w = cosf(angle / 2);
  return ret;
}

static inline struct quat quat_multiply(struct quat q1, struct quat q2) {
  struct quat ret = {0};
  ret.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
  ret.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
  ret.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
  ret.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
  return ret;
}

static inline struct quat quat_conjugate(struct quat q) {
  return (struct quat){-q.x, -q.y, -q.z, q.w};
}

static inline struct vec3 vec3_rotate(struct vec3 v, struct quat q) {
  struct quat ret = (struct quat){v.x, v.y, v.z, 0.0};
  ret = quat_multiply(quat_multiply(q, ret), quat_conjugate(q));
  return (struct vec3){ret.x, ret.y, ret.z};
}

static inline struct quat quat_from_euler(struct vec3 euler_angles) {
  struct quat q;

  float cos_roll = cosf(euler_angles.x * 0.5f),
        sin_roll = sinf(euler_angles.x * 0.5f),
        cos_pitch = cosf(euler_angles.y * 0.5f),
        sin_pitch = sinf(euler_angles.y * 0.5f),
        cos_yaw = cosf(euler_angles.z * 0.5f),
        sin_yaw = sinf(euler_angles.z * 0.5f);

  q.w = cos_roll * cos_pitch * cos_yaw + sin_roll * sin_pitch * sin_yaw;
  q.x = sin_roll * cos_pitch * cos_yaw - cos_roll * sin_pitch * sin_yaw;
  q.y = cos_roll * sin_pitch * cos_yaw + sin_roll * cos_pitch * sin_yaw;
  q.z = cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw;

  return q;
}

static inline struct quat quat_rotate_euler(struct quat q,
                                            struct vec3 euler_angles) {
  return quat_multiply(q, quat_from_euler(euler_angles));
}

static inline float *quat_to_mat4(struct quat q, float matrix[16]) {
  float xx = q.x * q.x, xy = q.x * q.y, xz = q.x * q.z, xw = q.x * q.w,

        yy = q.y * q.y, yz = q.y * q.z, yw = q.y * q.w,

        zz = q.z * q.z, zw = q.z * q.w;

  matrix[0] = 1 - 2 * (yy + zz);
  matrix[4] = 2 * (xy - zw);
  matrix[8] = 2 * (xz + yw);

  matrix[1] = 2 * (xy + zw);
  matrix[5] = 1 - 2 * (xx + zz);
  matrix[9] = 2 * (yz - xw);

  matrix[2] = 2 * (xz - yw);
  matrix[6] = 2 * (yz + xw);
  matrix[10] = 1 - 2 * (xx + yy);

  matrix[12] = matrix[13] = matrix[14] = matrix[3] = matrix[7] = matrix[11] = 0;
  matrix[15] = 1;

  return matrix;
}

// sets a 4x4 matrix 'matrix' to the identity matrix.
static inline void mathf_mat4_identity(float matrix[16]) {

  // row 1
  matrix[0] = 1.0;
  matrix[1] = 0.0;
  matrix[2] = 0.0;
  matrix[3] = 0.0;

  // row 2
  matrix[4] = 0.0;
  matrix[5] = 1.0;
  matrix[6] = 0.0;
  matrix[7] = 0.0;

  // row 3
  matrix[8] = 0.0;
  matrix[9] = 0.0;
  matrix[10] = 1.0;
  matrix[11] = 0.0;

  // row 4
  matrix[12] = 0.0;
  matrix[13] = 0.0;
  matrix[14] = 0.0;
  matrix[15] = 1.0;
}

/*Multiplies a 4x4 matrix with another 4x4 matrix*/
static inline void mathf_mat4_multiply(float *result, const float *a,
                                       const float *b) {

  // row 0
  result[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
  result[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
  result[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
  result[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];

  // row 1
  result[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
  result[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
  result[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
  result[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];

  // row 2
  result[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
  result[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
  result[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
  result[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];

  // row 3
  result[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
  result[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
  result[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
  result[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
}

// creates an 4x4 orthographic projection matrix and stores it inside 'matrix'
static inline void mathf_mat4_orthographic(float matrix[16], const float bottom,
                                           const float top, const float left,
                                           const float right, const float near,
                                           const float far) {

  mathf_mat4_identity(matrix);

  matrix[0] = 2.0 / (right - left);
  matrix[5] = 2.0 / (top - bottom);
  matrix[10] = -2.0 / (far - near);
  matrix[15] = 1.0;

  matrix[12] = -(right + left) / (right - left);
  matrix[13] = -(top + bottom) / (top - bottom);
  matrix[14] = (far + near) / (far - near);
}

// creates a 4x4 perspective projection matrix and stores it inside 'matrix'
static inline void mathf_mat4_perspective(float matrix[16], const float fov,
                                          const float aspect, const float near,
                                          const float far) {

  const float cotan = (1.0 / tanf(fov * 0.5));

  matrix[0] = (cotan / aspect);
  matrix[5] = cotan;
  matrix[10] = -((near + far) / (near - far));
  matrix[11] = 1.0f;
  matrix[14] = ((2.0 * near * far) / (near - far));
}

// converts a given 'transform' to a 4x4 view matrix and stores it inside
// 'matrix'
static inline void
mathf_transform_view_matrix(float matrix[16],
                            const struct transform *transform) {

  mathf_mat4_identity(matrix);

  float translation[16];
  mathf_mat4_identity(translation);
  translation[12] = -transform->position.x;
  translation[13] = -transform->position.y;
  translation[14] = -transform->position.z;

  float rotation[16];
  mathf_mat4_identity(rotation);
  quat_to_mat4(quat_conjugate(transform->rotation), rotation);

  mathf_mat4_multiply(matrix, translation, rotation);
}

// converts a given 'transform' to a 4x4 model matrix and stores it inside
// 'matrix'
static inline void mathf_transform_matrix(float matrix[16],
                                          const struct transform *transform) {
  mathf_mat4_identity(matrix);

  {
    float scale[16];
    mathf_mat4_identity(scale);
    scale[0] = transform->scale.x;
    scale[5] = transform->scale.y;
    scale[10] = transform->scale.z;

    float translation[16];
    mathf_mat4_identity(translation);
    translation[12] = transform->position.x;
    translation[13] = transform->position.y;
    translation[14] = transform->position.z;

    float rotation[16] = {0};
    quat_to_mat4(transform[0].rotation, rotation);

    mathf_mat4_multiply(matrix, scale, rotation);
    mathf_mat4_multiply(matrix, matrix, translation);
  }
}

#endif // MATHF_H
