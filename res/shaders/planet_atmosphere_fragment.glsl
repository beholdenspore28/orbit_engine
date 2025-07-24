#version 330 core

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec3 normal_local;
  vec3 frag_position;
} fs_in;

out vec4 FragColor;

uniform vec3 u_light_position;
uniform vec3 u_camera_position;

void main() {
  FragColor = vec4(0.2, 0.3, 0.4, 1.0);
}
