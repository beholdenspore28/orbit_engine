#version 330 core
layout (location = 0) in vec3 in_position;

uniform mat4 u_transform_matrix;
uniform mat4 u_camera_matrix;

void main() {
   gl_Position = u_camera_matrix * u_transform_matrix * vec4(in_position, 1.0);
}
