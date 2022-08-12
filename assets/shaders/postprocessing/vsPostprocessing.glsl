#version 400
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
//layout(location=2) in vec2 VertexTexcoord;

out vec4 Position;
out vec3 Normal;
out vec2 Texcoord;

void main()
{
    Position = vec4(VertexPos.x, VertexPos.y, 0.0, 1.0);
    //Texcoord = VertexTexcoord;
    Texcoord = vec2(VertexPos.x, VertexPos.y);
    gl_Position = Position;
}

