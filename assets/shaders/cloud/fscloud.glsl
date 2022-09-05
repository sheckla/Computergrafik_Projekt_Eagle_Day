#version 400
uniform vec3 EyePos;

in vec3 Position;
in vec2 Texcoord;
out vec4 FragColor;


uniform sampler2D DiffuseTexture; // Worley
uniform sampler2D NormalTexture; // noise

// stopped working?? Wtf
uniform sampler2D DetailTex[128]; 
uniform sampler2D noise;

uniform mat4 InverseViewMatrix;
uniform mat4 InverseProjectionMatrix;

uniform vec3 boundsMin;
uniform vec3 boundsMax;

uniform float TimeTranslation;

uniform float AspectWidth;
uniform float AspectHeight;

uniform int MaxTexture;

float BOTTOM = 80;
float TOP = 120;

void FadeOut(vec3 CameraPosition, vec3 WorldSpaceBottom,float fadeStart, float fadeEnd){
    if(!(CameraPosition.y > BOTTOM && CameraPosition.y < TOP)){
     float xDist=WorldSpaceBottom.x - CameraPosition.x;
     float zDist=WorldSpaceBottom.z - CameraPosition.z;
     
     if((xDist*xDist + zDist*zDist > fadeStart*fadeStart || xDist*xDist + zDist*zDist < -(fadeStart*fadeStart)) && !(xDist*xDist + zDist*zDist > fadeEnd*fadeEnd || xDist*xDist + zDist*zDist < -(fadeEnd*fadeEnd))){

     if(xDist*xDist + zDist*zDist>0)
     FragColor = vec4(FragColor.x,FragColor.y,FragColor.z,(FragColor.a * (1-((xDist*xDist + zDist*zDist - fadeStart*fadeStart)/(fadeEnd*fadeEnd - fadeStart*fadeStart)))));
     else
     FragColor = vec4(FragColor.x,FragColor.y,FragColor.z,(FragColor.a * (1-(-((xDist*xDist + zDist*zDist - fadeStart*fadeStart)/(fadeEnd*fadeEnd - fadeStart*fadeStart))))));
     }


     if(xDist*xDist + zDist*zDist > fadeEnd*fadeEnd || xDist*xDist + zDist*zDist < -(fadeEnd*fadeEnd))
     FragColor = vec4(0,0,0,0);
     }
}


void main()
{
    float NormScreenCoordX =  (gl_FragCoord.x / AspectWidth * 2 - 1);
    float NormScreenCoordY = (gl_FragCoord.y / AspectHeight * 2 - 1);
    
    // @ proj.transformVec4x4(Vector(xpos, ypos, 0));
    float Xm = InverseProjectionMatrix[0][0] * NormScreenCoordX + InverseProjectionMatrix[1][0] * NormScreenCoordY + InverseProjectionMatrix[2][0] * 0 + InverseProjectionMatrix[3][0];
    float Ym = InverseProjectionMatrix[0][1] * NormScreenCoordX + InverseProjectionMatrix[1][1] * NormScreenCoordY + InverseProjectionMatrix[2][1] * 0 + InverseProjectionMatrix[3][1];
    float Zm = InverseProjectionMatrix[0][2] * NormScreenCoordX + InverseProjectionMatrix[1][2] * NormScreenCoordY + InverseProjectionMatrix[2][2] * 0 + InverseProjectionMatrix[3][2];
    float Wm = InverseProjectionMatrix[0][3] * NormScreenCoordX + InverseProjectionMatrix[1][3] * NormScreenCoordY + InverseProjectionMatrix[2][3] * 0 + InverseProjectionMatrix[3][3];
    vec3 v = vec3(Xm/Wm,Ym/Wm,Zm/Wm);

    // @view.transformVec3x3(v)
    float Vec3x3_X = InverseViewMatrix[0][0] * v.x + InverseViewMatrix[1][0] + v.y + InverseViewMatrix[2][0] + v.z;
    float Vec3x3_Y = InverseViewMatrix[0][1] * v.x + InverseViewMatrix[1][1] + v.y + InverseViewMatrix[2][1] + v.z;
    float Vec3x3_Z = InverseViewMatrix[0][2] * v.x + InverseViewMatrix[1][2] + v.y + InverseViewMatrix[2][2] + v.z;

    vec3 PixelStrahl = (InverseViewMatrix * vec4(v,0)).xyz;  

    vec3 CameraPosition = vec3(InverseViewMatrix[3][0],InverseViewMatrix[3][1],InverseViewMatrix[3][2]);  

    float StrahlLaenge = (TOP - CameraPosition.y) / PixelStrahl.y;     // CameraPosition.y + PixelStrahl.y * StrahlLaenge = 120.0f;
    vec3 WorldSpaceTop = CameraPosition + (PixelStrahl * StrahlLaenge); //The Position in World Space at the Top position of the cloudbox

    float StrahlLaengeBottom = (BOTTOM - CameraPosition.y) / PixelStrahl.y;     
    vec3 WorldSpaceBottom = CameraPosition + (PixelStrahl * StrahlLaengeBottom); //The Position in World Space at the Bottom position of the cloudbox

////////////////////////////////////////////////////////////////////////////////////////////////////////////Essential Calculation Complete/////////////////////////////////////////////////
                            
    vec2 co=vec2(WorldSpaceBottom.x,WorldSpaceBottom.y);
    float rand = fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453); //Pseudo random function for generating static noise
    float static_noise = 1 + ((rand * 0.2)-.1) / 2;

    float div=100;


    float AlfaIntensity=0;
    //Ray marching through cloudbox adding the noise of each step, adding static noise for fuzzyness, than normalizing (This is for when the camera is under or over the cloudbox)
    for(int i=0;i<MaxTexture;i++){

        vec2 coor = vec2(WorldSpaceBottom.x/div+((WorldSpaceTop.x/div - WorldSpaceBottom.x/div)/MaxTexture)*i -TimeTranslation*.1f,WorldSpaceBottom.z/div+((WorldSpaceTop.z/div - WorldSpaceBottom.z/div)/MaxTexture)*i);

        float texVal=texture(DetailTex[i], coor).r;
        texVal*=static_noise;
        if(texVal>1)texVal=1;
        if(texVal<0)texVal=0;
        AlfaIntensity += texVal;
    }
    AlfaIntensity/=MaxTexture;// <- Transparency
    
    FragColor = vec4(1,1,1,AlfaIntensity);


    //INSIDE This part is for when the camera is inside the cloudbox
    int level = int(((CameraPosition.y-BOTTOM) / (TOP-BOTTOM)) * MaxTexture);
    float AlfaIntensityInside=0;
    float e = 2.718f;

    
    if(CameraPosition.y > BOTTOM && CameraPosition.y < TOP){

        if(StrahlLaenge>0){   // Looking up /\ /\ /\              
            vec3 CamToTop = CameraPosition - WorldSpaceTop;
            float lengthToTop = sqrt(CamToTop.x * CamToTop.x + CamToTop.y * CamToTop.y + CamToTop.z * CamToTop.z);

             for(int i=level;i<MaxTexture;i++){
                 vec2 coor = vec2(WorldSpaceBottom.x/div+((WorldSpaceTop.x/div - WorldSpaceBottom.x/div)/MaxTexture)*i -TimeTranslation*.1f,WorldSpaceBottom.z/div+((WorldSpaceTop.z/div - WorldSpaceBottom.z/div)/MaxTexture)*i);
                 float texVal=texture(DetailTex[i], coor).r;
                 texVal*=static_noise;
                 if(texVal>1)texVal=1;
                 if(texVal<0)texVal=0;
                 AlfaIntensityInside += texVal  * (1 + 2.0f * (1 - (pow(e,(-0.003f*lengthToTop))))); //Calculate density with distance through cloud-ray
            }

            AlfaIntensityInside/=(MaxTexture);
            if(AlfaIntensityInside<0)AlfaIntensityInside=0;
            if(AlfaIntensityInside>0.8)AlfaIntensityInside=0.8; //Too bright, clamp to 0.8

            AlfaIntensityInside*=0.8f; //Still too bright, reducing by 20%

            FragColor = vec4(1,1,1,AlfaIntensityInside);
        } 

        if(StrahlLaenge<=0){    // Looking down V V V
            vec3 CamToBottom = CameraPosition - WorldSpaceBottom;
            float lengthToBottom = sqrt(CamToBottom.x * CamToBottom.x + CamToBottom.y * CamToBottom.y + CamToBottom.z * CamToBottom.z);

            for(int i=0;i<level;i++){ 
                 vec2 coor = vec2(WorldSpaceBottom.x/div+((WorldSpaceTop.x/div - WorldSpaceBottom.x/div)/MaxTexture)*i -TimeTranslation*.1f,WorldSpaceBottom.z/div+((WorldSpaceTop.z/div - WorldSpaceBottom.z/div)/MaxTexture)*i);
                 float texVal=texture(DetailTex[i], coor).r;
                 texVal*=static_noise;
                 if(texVal>1)texVal=1;
                 if(texVal<0)texVal=0;           
                 AlfaIntensityInside += texVal *(1+ 2.0f * (1 - (pow(e,(-0.003f*(lengthToBottom)))))); //Calculate density with distance through cloud-ray
            }

            AlfaIntensityInside/=(MaxTexture);
            if(AlfaIntensityInside<0)AlfaIntensityInside=0;
            if(AlfaIntensityInside>0.8)AlfaIntensityInside=0.8; //Too bright, clamp to 0.8

            AlfaIntensityInside*=0.8f; //Still too bright, reducing by 20%

            FragColor = vec4(1,1,1,AlfaIntensityInside);
        }
    }
    
    if(CameraPosition.y < BOTTOM)
    FadeOut(CameraPosition,WorldSpaceBottom,200,400);

    if(CameraPosition.y > TOP)
    FadeOut(CameraPosition,WorldSpaceTop,200,400);
}