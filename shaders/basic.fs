#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float visibility;
in vec3 FragPos;

uniform sampler2D texture1;
uniform vec3 skyColour;

void main()
{

	vec4 result_color = texture(texture1, TexCoord);
	vec4 final_color = mix(vec4(skyColour, 1.0), result_color, visibility);
	FragColor = final_color;
}
