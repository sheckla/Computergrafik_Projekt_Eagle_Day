#version 400

layout(location=0) in vec4 VertexPos; // given from VB
layout(location=1) in vec4 VertexNormal; // given from VB
layout(location=2) in vec2 VertexTexcoord; //given from VB

out vec3 Position; // (to be scaled) Position in World-Space
out vec3 Normal; // (to be scaled) Normal in World-Space
out vec2 Texcoord;
out vec3 Pos;

// Uniforms are unique global variables accessable from any shader stage
// Declared via. PhongShader::activate -> glUniformMatrixx4fv(...)
uniform mat4 ModelMat; // BaseShader::modelTransform Matrix -> Position (World Space)
uniform mat4 ModelViewProjMat; // CamProjMatrix * CamViewMatrix * TransformMat -> normalised Viewspace
uniform vec3 Scaling; // global Scaling Vector

//uniform vec3 Resolution;

uniform sampler2D MixTex;

void main()
{
    Texcoord = VertexTexcoord;
    gl_Position = ModelViewProjMat * VertexPos; // clip-space output position
}
