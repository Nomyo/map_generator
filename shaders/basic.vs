#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 ourColor;
out float visibility;

const float density = 0.007;
const float gradient = 1.5;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    vec4 world_position = vec4(FragPos, 1.0);
    vec4 position_relative_to_cam = view * world_position;

    ourColor = aColor;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y) * 0.2;
    gl_Position = projection * position_relative_to_cam;

    float distance = length(position_relative_to_cam.xyz);
    visibility = exp(-pow((distance * density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);
}