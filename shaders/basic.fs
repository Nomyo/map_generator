#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float visibility;
in vec3 FragPos;
in vec3 ourColor;
in vec3 blendColour;
in vec3 Normal;

uniform sampler2D background;
uniform sampler2D rtexture;
uniform sampler2D gtexture;
uniform sampler2D btexture;
uniform vec3 skyColour;
uniform vec3 lightPos;
uniform vec3 lightColor;


void main()
{
    float backTextureAmount = 1 - (blendColour.r + blendColour.g + blendColour.b);
    vec2 tiledCoords = TexCoord * 0.2;
    vec4 backgroundTextureColour = texture(background, tiledCoords) * backTextureAmount;
    vec4 rTextureColour = texture(rtexture, tiledCoords) * blendColour.r; // snow
    vec4 gTextureColour = texture(gtexture, tiledCoords) * blendColour.g; // grassy
    vec4 bTextureColour = texture(btexture, tiledCoords) * blendColour.b; // rocky

    if (blendColour.r > 0.1)
    {
	rTextureColour = rTextureColour * vec4(ourColor, 1.0);
	if(blendColour.r > 0.49 && blendColour.r < 0.50)
	{
	    backgroundTextureColour = backgroundTextureColour * vec4(0.0f, (1.0f / 255.0f) * 102.0f, 0.0, 1.0);
	    gTextureColour = gTextureColour * vec4(0.0f, (1.0f / 255.0f) * 102.0f, 0.0, 1.0);
	}
    }
    else
    {
	backgroundTextureColour = backgroundTextureColour * vec4(ourColor, 1.0);
	rTextureColour = rTextureColour * vec4(ourColor, 1.0);
	gTextureColour = gTextureColour * vec4(ourColor, 1.0);
	bTextureColour = bTextureColour * vec4(ourColor, 1.0);
    }

    vec4 totalColour = backgroundTextureColour + rTextureColour + gTextureColour + bTextureColour;

    // LIGHT
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result_color = (ambient + diffuse) * totalColour.xyz;
    vec4 final_color = mix(vec4(skyColour, 1.0), vec4(result_color, 1.0), visibility);

    FragColor = final_color;
}
