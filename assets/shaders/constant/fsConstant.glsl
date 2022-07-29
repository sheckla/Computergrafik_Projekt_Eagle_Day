#version 400
uniform vec3 Color;
in vec3 Position;
out vec4 FragColor;
void main()
{
    FragColor = vec4(Color, 1);
}