#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in float visibility;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D texture_diffuse1;
uniform vec3 skyColour;

void main()
{
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec4 texture_color = texture(texture_diffuse1, TexCoords);
    if (texture_color.a < 0.5f)
    discard;

    vec3 result_color = (ambient + diffuse) * texture_color.xyz;
    vec4 final_color = mix(vec4(skyColour, 1.0), vec4(result_color, 1.0), visibility);
    FragColor = final_color;
}
