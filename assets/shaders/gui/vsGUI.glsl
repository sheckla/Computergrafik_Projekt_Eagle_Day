#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;

out vec4 Position;
out vec3 Normal;
out vec2 Texcoord;

void main()
{
    Position = vec4(VertexPos.x, VertexPos.y, 0.0, 1.0);
    Texcoord = vec2(VertexPos.x - 1 + 0.5, VertexPos.y * -1 + 0.5);
    Texcoord = VertexTexcoord;
    Texcoord.y = -Texcoord.y;
    Texcoord.x = -Texcoord.x;
    gl_Position = Position;
}

