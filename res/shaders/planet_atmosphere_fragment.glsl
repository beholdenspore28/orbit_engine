#version 330 core

in VS_OUT {
  vec3 position;
  vec3 normal;
} fs_in;

out vec4 FragColor;

uniform vec3 u_camera_position;

void main() {
  FragColor = vec4(fs_in.normal, 0.3);
}
