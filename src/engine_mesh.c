#include "engine.h"

struct mesh mesh_planet_alloc(const unsigned int subdivisions,
                              const float amplitude) {

  list_GLuint indices_initial = NULL;
  list_vector3 vertices_initial = NULL;

  { // create a baseline icosahedron.
    const GLfloat t = (1.0 + sqrt(5.0)) / 2.0;

    enum { vertices_count = 12 };
    vector3 vertices[vertices_count] = {
        (vector3){-1, t, 0},  (vector3){1, t, 0},
        (vector3){-1, -t, 0}, (vector3){1, -t, 0},

        (vector3){0, -1, t},  (vector3){0, 1, t},
        (vector3){0, -1, -t}, (vector3){0, 1, -t},

        (vector3){t, 0, -1},  (vector3){t, 0, 1},
        (vector3){-t, 0, -1}, (vector3){-t, 0, 1},
    };

    enum { indices_count = 60 };
    GLuint indices[indices_count] = {
        5, 11, 0,  1,  5, 0, 7,  1, 0,  10, 7, 0,  11, 10, 0, 9, 5, 1, 4, 11,
        5, 2,  10, 11, 6, 7, 10, 8, 1,  7,  4, 9,  3,  2,  4, 3, 6, 2, 3, 8,
        6, 3,  9,  8,  3, 5, 9,  4, 11, 4,  2, 10, 2,  6,  7, 6, 8, 1, 8, 9,
    };

    indices_initial = list_GLuint_alloc_from_array(indices, indices_count);
    vertices_initial = list_vector3_alloc_from_array(vertices, vertices_count);
  }

  // *===============================================*
  // * vertex layout                                 *
  // *===============================================*
  // |                      v1 [i1]                  |
  // |                     /  \                      |
  // |                    /    \                     |
  // |                   /      \                    |
  // |                  /        \                   |
  // |                 /          \                  |
  // |         [i4] m1------------m3 [i6]            |
  // |               /\            /\                |
  // |              /  \          /  \               |
  // |             /    \        /    \              |
  // |            /      \      /      \             |
  // |           /        \    /        \            |
  // |          /          \  /          \           |
  // | [i2] v2 ------------m2-[i5]---- v3 [i3]       |
  // *===============================================*

  for (unsigned int subdivision = 0; subdivision < subdivisions;
       subdivision++) {

    list_GLuint indices_subdivided = list_GLuint_alloc();

    for (unsigned int tri = 0; tri < list_GLuint_count(indices_initial);
         tri += 3) {

      const unsigned int i1 = indices_initial[tri];
      const unsigned int i2 = indices_initial[tri + 1];
      const unsigned int i3 = indices_initial[tri + 2];

      { // get vertices in this triangle
        const vector3 v1 = vertices_initial[i1];
        const vector3 v2 = vertices_initial[i2];
        const vector3 v3 = vertices_initial[i3];

        // create middle vertices
        const vector3 m1 = vector3_lerp(v1, v2, 0.5);
        const vector3 m2 = vector3_lerp(v2, v3, 0.5);
        const vector3 m3 = vector3_lerp(v3, v1, 0.5);

        list_vector3_add(&vertices_initial, m1);
        list_vector3_add(&vertices_initial, m2);
        list_vector3_add(&vertices_initial, m3);
      }

      { // get new indices
        const unsigned int i4 = list_vector3_count(vertices_initial) - 3;
        const unsigned int i5 = list_vector3_count(vertices_initial) - 2;
        const unsigned int i6 = list_vector3_count(vertices_initial) - 1;

        list_GLuint_add(&indices_subdivided, i4);
        list_GLuint_add(&indices_subdivided, i5);
        list_GLuint_add(&indices_subdivided, i6);

        list_GLuint_add(&indices_subdivided, i1);
        list_GLuint_add(&indices_subdivided, i4);
        list_GLuint_add(&indices_subdivided, i6);

        list_GLuint_add(&indices_subdivided, i4);
        list_GLuint_add(&indices_subdivided, i2);
        list_GLuint_add(&indices_subdivided, i5);

        list_GLuint_add(&indices_subdivided, i6);
        list_GLuint_add(&indices_subdivided, i5);
        list_GLuint_add(&indices_subdivided, i3);
      }
    }
    list_GLuint_free(indices_initial);
    indices_initial = indices_subdivided;
  }

#if 1
  // normalize all points after subdividing
  for (unsigned int i = 0; i < list_vector3_count(vertices_initial); i++) {
    vector3_normalize(&vertices_initial[i]);
    float noise = mathf_noise3_fbm(vertices_initial[i].x, vertices_initial[i].y,
                                   vertices_initial[i].z);
    vector3_add(&vertices_initial[i],
                vector3_scaled(vertices_initial[i], noise * amplitude));
  }
#endif

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  GLuint *VBOs = calloc(5, sizeof(*VBOs));
  glGenBuffers(4, VBOs);

  // positions
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER,
               list_vector3_count(vertices_initial) * sizeof(*vertices_initial),
               vertices_initial, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(0);

  GLuint EBO = 0;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(*indices_initial) * list_GLuint_count(indices_initial),
               indices_initial, GL_STATIC_DRAW);

  glBindVertexArray(0);

  struct mesh mesh = {0};
  mesh.VAO = VAO;
  mesh.VBOs = VBOs;
  mesh.EBO = EBO;
  mesh.vertices_count = list_vector3_count(vertices_initial);
  mesh.indices_count = list_GLuint_count(indices_initial);

  return mesh;
}
