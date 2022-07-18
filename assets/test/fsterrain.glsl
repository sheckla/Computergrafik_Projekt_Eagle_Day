#version 400
uniform vec3 EyePos;
uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

uniform sampler2D MixTex; // for exercise 3
uniform sampler2D DetailTex[2]; // for exercise 3
uniform vec3 Scaling;

in vec3 Position;
in vec3 Pos;
in vec3 Normal;
in vec2 Texcoord;
out vec4 FragColor;

float sat( in float a)
{
    return clamp(a, 0.0, 1.0);
}


void main()
{
    vec3 N      = normalize(Normal);
    vec3 L      = normalize(LightPos); // light is treated as directional source
    vec3 D      = EyePos-Position;
    float Dist  = length(D);
    vec3 E      = D/Dist;
    vec3 R      = reflect(-L,N);

    vec3 DiffuseColorRGB = vec3(0.023f,0.25f,0.45f);
    vec3 SpecularColorRGB = vec3(.8f,.8f,.8f);

    vec3 Heightlight = vec3(1,1,1);

    //vec3 DiffuseComponent = LightColor * DiffuseColorRGB * sat(dot(N,L));

    vec3 DiffuseComponent = LightColor * DiffuseColorRGB * sat(dot(N,L));

    vec3 SpecularComponent = LightColor * SpecularColorRGB * pow( sat(dot(R,E)), SpecularExp);
    
    // Exercise 3
    // TODO: Add texture blending code here..
    //if(Pos.y>0.365f)FragColor = vec4((Heightlight * sat(dot(N,L)) * 1.7f),1);
    //else
    float abstTC=0.00195312f * 2.0f;
    float pixelCol = texture(MixTex, vec2(Texcoord.x,Texcoord.y + abstTC)).r;

   // DiffuseComponent.y *= (1-pixelCol) *.6f;
    //DiffuseComponent.x *= (1-pixelCol) *.6f;
   // DiffuseComponent.z *= (pixelCol*1.3f);
    /*
    vec3 pCol;
    if(pixelCol<0.5)
    pCol = vec3(1,0,0) * pixelCol;
    else if(pixelCol>0.5 && pixelCol<.8f)
    pCol = vec3(0,1,0) * pixelCol;
    else
    pCol = vec3(.5f,.5f,0) * pixelCol;
    */
    FragColor = vec4(((DiffuseComponent + AmbientColor) + SpecularComponent),1);

   // FragColor = vec4(vec3(pixelCol,pixelCol,pixelCol),1);
}
