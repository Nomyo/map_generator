#version 330 core
in vec3 TexCoords;

uniform samplerCube cubeMap;
uniform float time;

void main(void)
{
   gl_FragColor = textureCube(cubeMap, TexCoords);
   gl_FragColor.b += 0.2;
   gl_FragColor.a = 0.75 + (TexCoords.y - 19.0);
}
