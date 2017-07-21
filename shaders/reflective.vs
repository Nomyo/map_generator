#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main(void)
{
  vec4 a_position = vec4(aPos, 1.0);
  vec3 apos2 = vec3(a_position.x + sin(a_position.y - 19.0f) * 100.0, a_position.y + 0.125 * sin(int(a_position.x) % 5 + int(a_position.z) % 3 + time * 0.125), a_position.z);
  vec4 new_position = vec4(apos2.x, apos2.y, apos2.z, a_position.w);
  TexCoords = apos2;
  gl_Position = projection * view * model * new_position;
}
