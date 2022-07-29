#version 400

// vom VertexBuffer
layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;

out vec2 Texcoord;
out vec3 Position;

uniform mat4 ModelViewProjMat;

void main()
{
    gl_Position = ModelViewProjMat * VertexPos;
    Texcoord = VertexTexcoord;
    Position = gl_Position.xyz;
}