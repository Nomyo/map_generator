#version 330 core
out vec4 FragColor;

in vec4 gl_FragCoord;
uniform float time;

void main()
{
    FragColor = vec4(0.0, 0.0, 0.5, 0.2); // set alle 4 vector values to 1.0
}
