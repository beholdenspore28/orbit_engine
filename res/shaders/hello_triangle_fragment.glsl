#version 330 core

in VS_OUT {
  vec3 position;
  vec3 normal;
} fs_in;

out vec4 FragColor;

uniform sampler2D u_diffuse_map;

void main() {
  // object color
  vec3 colorX = texture(u_diffuse_map, fs_in.position.zy).rgb;
  vec3 colorY = texture(u_diffuse_map, fs_in.position.xz).rgb;
  vec3 colorZ = texture(u_diffuse_map, fs_in.position.xy).rgb;

  float blend_sharpness = 4;

  vec3 blend_weight = pow(abs(normalize(fs_in.normal)), vec3(blend_sharpness));
  blend_weight /= dot(blend_weight, vec3(1));

  // vec3 color = fs_in.normal * 0.5;
  vec3 color = colorX * blend_weight.x + colorY * blend_weight.y +
    colorZ * blend_weight.z;

  FragColor = vec4(color, 1.0);
}
