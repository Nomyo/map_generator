#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    vec4 texture_color = texture(texture_diffuse1, TexCoords);
    if (texture_color.a < 0.5f)
       discard;
    FragColor = texture_color;
}
