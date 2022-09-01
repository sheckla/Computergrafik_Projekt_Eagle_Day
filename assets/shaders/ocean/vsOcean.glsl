#version 400

layout(location=0) in vec4 VertexPos; // given from VB
layout(location=1) in vec4 VertexNormal; // given from VB
layout(location=2) in vec2 VertexTexcoord; //given from VB

out vec3 Position; // (to be scaled) Position in World-Space
out vec3 Normal; // (to be scaled) Normal in World-Space
out vec2 Texcoord;
out vec3 Pos;

out vec4 PositionWS;

// Uniforms are unique global variables accessable from any shader stage
// Declared via. PhongShader::activate -> glUniformMatrixx4fv(...)
uniform mat4 ModelMat; // BaseShader::modelTransform Matrix -> Position (World Space)
uniform mat4 ModelViewProjMat; // CamProjMatrix * CamViewMatrix * TransformMat -> normalised Viewspace
uniform vec3 Scaling; // global Scaling Vector

uniform sampler2D Perlin;

//uniform vec3 Resolution;

uniform sampler2D MixTex;

void main()
{
    float PerlinInfluenceStrength = 3.0f;

    float heightMultip=0.8f;
    Texcoord = VertexTexcoord;
        

    vec3 _vertex = vec3(VertexPos.xyz);

    vec2 globalUV = vec2(VertexPos.x / 3000,VertexPos.z / 3000);




    vec4 worldSpace = ModelMat * vec4(_vertex.xyz, 1.0);
    vec2 wsUV = vec2(worldSpace.x / 4000,worldSpace.z / 4000);




    _vertex.y = texture(MixTex, Texcoord).r * heightMultip /* (texture(Perlin,globalUV).r)*/;

    _vertex.y *= 1 + texture(Perlin,wsUV).r * PerlinInfluenceStrength;

    float height_N = _vertex.y;

    float abst=0.01953125f;
    float abstTC=0.00195312f/* * 2.0f*/;
    /*
    if(Resolution/32 == 0){
    abst*=16;
    abstTC*=8;
    }
    */
    /*
    if(Resolution/64 == 0){
    abst*=8;
    abstTC*=4;
    }
    
    if(Resolution/128 == 0){
    abst*=4;
    abstTC*=2;
    }
    */
    //if(Resolution/256 == 0)
    //abst*=2;
    //float abst=0.01953125f /* 2.0f * 2.0f*/;
    


        float height_A =  texture(MixTex, vec2(Texcoord.x,Texcoord.y + abstTC)).r * heightMultip            * (1 + texture(Perlin,vec2(wsUV.x, wsUV.y + abstTC)).r * PerlinInfluenceStrength);
        float height_B =  texture(MixTex, vec2(Texcoord.x + abstTC,Texcoord.y)).r * heightMultip            * (1 + texture(Perlin,vec2(wsUV.x + abstTC, wsUV.y)).r * PerlinInfluenceStrength);
        float height_C =  texture(MixTex, vec2(Texcoord.x,Texcoord.y - abstTC)).r * heightMultip            * (1 + texture(Perlin,vec2(wsUV.x, wsUV.y - abstTC)).r * PerlinInfluenceStrength);
        float height_D =  texture(MixTex, vec2(Texcoord.x - abstTC,Texcoord.y)).r * heightMultip            * (1 + texture(Perlin,vec2(wsUV.x - abstTC, wsUV.y)).r * PerlinInfluenceStrength);

        vec3 va = vec3(0,(height_A - height_N),abst);
        vec3 vb = vec3(abst,(height_B - height_N),0);
        vec3 vc = vec3(0,(height_C - height_N),-abst);
        vec3 vd = vec3(-abst,(height_D - height_N),0);

        vec3 average_n = normalize( (cross(va,vb) + cross(vb,vc) + cross(vc,vd) + cross(vd,va)) / 4);
        Normal = average_n;
 

    //Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz * 1/Scaling; 

    Pos = vec3(_vertex.xyz);

    Texcoord = VertexTexcoord;
    PositionWS = ModelMat * vec4(_vertex.xyz, 1.0); // <--


    gl_Position = ModelViewProjMat * vec4(_vertex.xyz, 1.0); // clip-space output position
}
