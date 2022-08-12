#version 400
in vec3 Position;
in vec2 Texcoord;
out vec4 FragColor;
uniform sampler2D DiffuseTexture;
void main()
{
    FragColor = texture(DiffuseTexture, Texcoord);
}