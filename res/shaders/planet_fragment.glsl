#version 330 core

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec3 normal_local;
} fs_in;

out vec4 FragColor;

uniform sampler2D u_diffuse_map;
uniform vec3 u_light_position;
uniform vec3 u_camera_position;


void main() {
  // object color
  vec3 colorX = texture(u_diffuse_map, fs_in.position.zy).rgb;
  vec3 colorY = texture(u_diffuse_map, fs_in.position.xz).rgb;
  vec3 colorZ = texture(u_diffuse_map, fs_in.position.xy).rgb;

  float blend_sharpness = 4;

  vec3 blend_weight = pow(abs(normalize(fs_in.normal_local)), vec3(blend_sharpness));
  blend_weight /= dot(blend_weight, vec3(1));

  // vec3 color = fs_in.normal_local * 0.5;
  vec3 color = colorX * blend_weight.x + colorY * blend_weight.y +
    colorZ * blend_weight.z;

  vec3 ambient = 0.1 * color;
  // diffuse
  vec3 lightDir = normalize(u_light_position - fs_in.position);
  vec3 normal = normalize(fs_in.normal);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * color;
  FragColor = vec4(ambient + diffuse, 1.0);
}
