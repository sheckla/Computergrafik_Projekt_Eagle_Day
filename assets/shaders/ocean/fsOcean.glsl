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
uniform sampler2D Perlin;
uniform samplerCube CubeMapTexture;


uniform mat4 InvViewMatrix;
uniform mat4 InvProjMatrix;

in vec3 Position;
in vec3 Pos;
in vec3 Normal;
in vec2 Texcoord;
in vec4 PositionWS;
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

    vec2 uv=vec2(Position.x / 200,Position.z / 200);
    //DiffuseColorRGB.r *= 1+ .1f-(texture(Perlin,Texcoord).r * .2f);
    //DiffuseColorRGB.g *= 1+ .1f-(texture(Perlin,Texcoord).r * .2f);
    DiffuseColorRGB.b *= 1+ (texture(Perlin,Texcoord).r * .4f);

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
    //FragColor = vec4(((DiffuseComponent + AmbientColor) + SpecularComponent),1);
     FragColor = vec4(/*((DiffuseComponent + AmbientColor) + SpecularComponent)*/DiffuseComponent * (.4f +Normal.y),1);

     //FragColor = vec4(PositionWS.r,1,PositionWS.b,1);
     //FragColor = vec4(texture(Perlin,Texcoord).r,0,0,1);
     float dist = PositionWS.w;
     float hoehe = InvViewMatrix[3][1];

     float flatDistance = dist*dist - hoehe*hoehe;
     //if(dist>100 || dist< -100)FragColor = vec4(1,0,0,1);
     //else
     //FragColor = vec4(DiffuseComponent*(dist*.1f),1); //Cool effect, useless though


     ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     float NormScreenCoordX =  (gl_FragCoord.x / 1920.0f * 2 - 1);
    float NormScreenCoordY = (gl_FragCoord.y / 1080.0f * 2 - 1);
   
    float Xm = InvProjMatrix[0][0] * NormScreenCoordX + InvProjMatrix[1][0] * NormScreenCoordY + InvProjMatrix[2][0] * 0 + InvProjMatrix[3][0];
    float Ym = InvProjMatrix[0][1] * NormScreenCoordX + InvProjMatrix[1][1] * NormScreenCoordY + InvProjMatrix[2][1] * 0 + InvProjMatrix[3][1];
    float Zm = InvProjMatrix[0][2] * NormScreenCoordX + InvProjMatrix[1][2] * NormScreenCoordY + InvProjMatrix[2][2] * 0 + InvProjMatrix[3][2];
    float Wm = InvProjMatrix[0][3] * NormScreenCoordX + InvProjMatrix[1][3] * NormScreenCoordY + InvProjMatrix[2][3] * 0 + InvProjMatrix[3][3];
    vec3 v = vec3(Xm/Wm,Ym/Wm,Zm/Wm);

    float Vec3x3_X = InvViewMatrix[0][0] * v.x + InvViewMatrix[1][0] + v.y + InvViewMatrix[2][0] + v.z;
    float Vec3x3_Y = InvViewMatrix[0][1] * v.x + InvViewMatrix[1][1] + v.y + InvViewMatrix[2][1] + v.z + 3;
    float Vec3x3_Z = InvViewMatrix[0][2] * v.x + InvViewMatrix[1][2] + v.y + InvViewMatrix[2][2] + v.z;

    vec3 PixelStrahl = (InvViewMatrix * vec4(v,0)).xyz;  

    vec3 CameraPosition = vec3(InvViewMatrix[3][0],InvViewMatrix[3][1],InvViewMatrix[3][2]);  

    float StrahlLaenge = (CameraPosition.y) / PixelStrahl.y;

    vec3 WorldSpaceTop = CameraPosition + (PixelStrahl * StrahlLaenge);
    
     float xDist=WorldSpaceTop.x - CameraPosition.x;
     float zDist=WorldSpaceTop.z - CameraPosition.z;
     float fadeStart=500;
     if((xDist*xDist + zDist*zDist > fadeStart*fadeStart || xDist*xDist + zDist*zDist < -(fadeStart*fadeStart))){
        FragColor = vec4(1,1,1,0);
        }
   // FragColor = vec4(vec3(pixelCol,pixelCol,pixelCol),1);
   // CubeMap SpecularComponent
        vec3 o = normalize(Position - EyePos);
        vec3 p = reflect(o, normalize(Normal));
        vec4 Reflection = texture(CubeMapTexture, p);
        FragColor = mix(FragColor, Reflection, 0.5);
}
