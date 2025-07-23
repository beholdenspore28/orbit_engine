#version 330 core

in VS_OUT {
  vec3 position;
  vec3 normal;
  vec3 normal_local;
  vec3 frag_position;
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
  vec3 lightDir = normalize(u_light_position - fs_in.frag_position);
  vec3 normal = normalize(fs_in.normal);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * color;
  // specular
  vec3 viewDir = normalize(u_camera_position - fs_in.frag_position);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = 0.0;
  vec3 halfwayDir = normalize(lightDir + viewDir);
  spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
  vec3 specular = vec3(0.3) * spec; // assuming bright white light color

  FragColor = vec4(ambient + diffuse + specular, 1.0);
}
