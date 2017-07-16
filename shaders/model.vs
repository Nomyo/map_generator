#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float fake_lighting;

const float density = 0.0045;
const float gradient = 1.5;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    vec4 world_position = vec4(FragPos, 1.0);
    vec4 position_relative_to_cam = view * world_position;
    vec3 actual_normal = aNormal;

    if (fake_lighting > 0.5f)
    {
	actual_normal = vec3(0.0f, 1.0f, 0.0f);
    }

    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);

    float distance = length(position_relative_to_cam.xyz);
    visibility = exp(-pow((distance * density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);
}
