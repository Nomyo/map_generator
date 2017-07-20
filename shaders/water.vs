#version 330 core
layout (location = 0) in vec3 aPos;

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 a_position = vec4(aPos, 1.0);
	vec4 new_position = vec4(a_position.x, a_position.y + 0.25 * sin(a_position.x+a_position.z + time * 0.5), a_position.z, a_position.w);
	gl_Position = projection * view * model * new_position;
}
