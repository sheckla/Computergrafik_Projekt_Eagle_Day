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
/*
//https://github.com/Erkaman/glsl-worley
vec2 rayBoxDst (in vec3 origin, in vec3 dir) {
    vec3 t0 = (boundsMin - origin) / dir;
    vec3 t1 = (boundsMax - origin) / dir;

    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);

    float dstA = max(max(tmin.x, tmin.y),tmin.z);
    float dstB = min(tmax.x, min(tmax.y, tmax.z));

    float dstToBox = max(0,dstA);
    float dstInsideBox = max(0, dstB - dstToBox);

    return vec2(dstToBox, dstInsideBox);
}
/*
float sampleDensity(in vec3 pos) {
    vec3 size = boundsMax - boundsMin;
    float heightPercent = (Position.y - boundsMin.y) / size.y;
    float widthPercent = (pos.x - boundsMin.x) / size.x;
    float depthPercent = (pos.z - boundsMin.z) / size.z;

    float normX = (gl_FragCoord.x / 1920.0f * 2) - 1;
    float normY = (gl_FragCoord.y / 1080.0f * 2) - 1;

    /* int layer = int(abs(heightPercent) * 48);
    layer = clamp(layer, 0, 47);
    layer = int(normY / size.y); */


    // DensityColor via x/z Texcoord
    //vec4 col = texture(DiffuseTexture, vec2(Position.x, Position.z)/200) + 3*texture(NormalTexture, vec2(Position.x, Position.z)/2000);
    // DensityColor via normalised screenspace
    /*
    vec4 col = texture(DiffuseTexture, vec2(normX, normY)) + texture(NormalTexture, vec2(normX, normY));

    float density = col.r;
    return density;
}
*/

void FadeOut(vec3 CameraPosition, vec3 WorldSpaceBottom,float fadeStart, float fadeEnd){
    if(!(CameraPosition.y > 80 && CameraPosition.y < 120)){
     //FADE OUT
     float xDist=WorldSpaceBottom.x - CameraPosition.x;
     float zDist=WorldSpaceBottom.z - CameraPosition.z;

     //float fadeStart=200;
     //float fadeEnd=400;
     
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

/*
// learnopengl
float near = 0.1; 
float far  = 100.0; 
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
*/

// Matrix-Transformationen von rechts nach links
void main()
{
    float NormScreenCoordX =  (gl_FragCoord.x / 1920.0f * 2 - 1);
    float NormScreenCoordY = (gl_FragCoord.y / 1080.0f * 2 - 1);
    
    
   // mat4 InverseViewMatrix=inverse(ViewMatrix);
   // mat4 InverseProjectionMatrix=inverse(ProjectionMatrix);
   //float abc= TimeTranslation;
    /*
    float X = m00*v.X + m01*v.Y + m02*v.Z + m03;
    float Y = m10*v.X + m11*v.Y + m12*v.Z + m13;
    float Z = m20*v.X + m21*v.Y + m22*v.Z + m23;
    float W = m30*v.X + m31*v.Y + m32*v.Z + m33;
    return Vector( X/W, Y/W, Z/W);
    */

    // @ proj.transformVec4x4(Vector(xpos, ypos, 0));
    float Xm = InverseProjectionMatrix[0][0] * NormScreenCoordX + InverseProjectionMatrix[1][0] * NormScreenCoordY + InverseProjectionMatrix[2][0] * 0 + InverseProjectionMatrix[3][0];
    float Ym = InverseProjectionMatrix[0][1] * NormScreenCoordX + InverseProjectionMatrix[1][1] * NormScreenCoordY + InverseProjectionMatrix[2][1] * 0 + InverseProjectionMatrix[3][1];
    float Zm = InverseProjectionMatrix[0][2] * NormScreenCoordX + InverseProjectionMatrix[1][2] * NormScreenCoordY + InverseProjectionMatrix[2][2] * 0 + InverseProjectionMatrix[3][2];
    float Wm = InverseProjectionMatrix[0][3] * NormScreenCoordX + InverseProjectionMatrix[1][3] * NormScreenCoordY + InverseProjectionMatrix[2][3] * 0 + InverseProjectionMatrix[3][3];
    vec3 v = vec3(Xm/Wm,Ym/Wm,Zm/Wm);

    // @view.transformVec3x3(v)
    /*
    float X = m00*v.X + m01*v.Y + m02*v.Z;
    float Y = m10*v.X + m11*v.Y + m12*v.Z;
    float Z = m20*v.X + m21*v.Y + m22*v.Z;
    return Vector( X, Y, Z);
    */
    float Vec3x3_X = InverseViewMatrix[0][0] * v.x + InverseViewMatrix[1][0] + v.y + InverseViewMatrix[2][0] + v.z;
    float Vec3x3_Y = InverseViewMatrix[0][1] * v.x + InverseViewMatrix[1][1] + v.y + InverseViewMatrix[2][1] + v.z;
    float Vec3x3_Z = InverseViewMatrix[0][2] * v.x + InverseViewMatrix[1][2] + v.y + InverseViewMatrix[2][2] + v.z;

    //vec3 PixelStrahl = vec3(Vec3x3_X,Vec3x3_Y,Vec3x3_Z);
    vec3 PixelStrahl = (InverseViewMatrix * vec4(v,0)).xyz;  

    //PixelStrahl = vec4(dirWorld,0);

    vec3 CameraPosition = vec3(InverseViewMatrix[3][0],InverseViewMatrix[3][1],InverseViewMatrix[3][2]);  

    float StrahlLaenge = (120 - CameraPosition.y) / PixelStrahl.y;     // CameraPosition.y + PixelStrahl.y * StrahlLaenge = 120.0f;

    vec3 WorldSpaceTop = CameraPosition + (PixelStrahl * StrahlLaenge);

    float StrahlLaengeBottom = (80 - CameraPosition.y) / PixelStrahl.y;     
    vec3 WorldSpaceBottom = CameraPosition + (PixelStrahl * StrahlLaengeBottom);

                             
    //float StrahlLaengeBottom = 49.5f+(-CameraPosition.y / PixelStrahl.y); 
    //vec4 WorldSpaceBottom = CameraPosition + (StrahlLaengeBottom * StrahlLaenge);
    /*
    Matrix view, proj;
    view = Cam.getViewMatrix();
    view.invert();                      // inverseViewMatrix
    proj = Cam.getProjectionMatrix();
    proj.invert();                      // inverseProjectionMatrix


    Vector v = proj.transformVec4x4(Vector(x, y, 0));         //    v = inverseProjectionMatrix (?) * vec4(NormScreenCoordX,0,NormScreenCoordY);
    CameraPosition = view.translation();                     //              CAMERA POSITION

   // return view.transformVec3x3(v);                        //  STRAHL                        vec3 RAY = inverseViewMatrix (?) * v;                  
                                                         //                 ! RAY !

    /*
        Vector CameraPosition;                                                                      
        Vector PixelStrahl = calc3DRay(ScreenCoord.x, ScreenCoord.y, CameraPosition);                                       

        float StrahlLaenge = hoehe + (-CameraPosition.Y / PixelStrahl.Y);                               

        Vector WorldSpaceTop = CameraPosition + (PixelStrahl * StrahlLaenge);                     

    */
    
    //CalcWorldSpace of Ray @ Pixel w/ y=top -> UV-Coords -- WorldSpaceTop
    //CalcWorldSpace of Ray @ Pixel w/ y=bottom -> UV-Coords -- WorldSpaceBottom

    vec2 co=vec2(WorldSpaceBottom.x,WorldSpaceBottom.y);
    float rand = fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
    float static_noise = 1 + ((rand * 0.2)-.1) / 2;


    float div=100;


    float AlfaIntensity=0;
    for(int i=0;i<128;i++){

    vec2 coor = vec2(WorldSpaceBottom.x/div+((WorldSpaceTop.x/div - WorldSpaceBottom.x/div)/128)*i +TimeTranslation*.1f,WorldSpaceBottom.z/div+((WorldSpaceTop.z/div - WorldSpaceBottom.z/div)/128)*i);

        float texVal=texture(DetailTex[i], coor).r;
        texVal*=static_noise;
        if(texVal>1)texVal=1;
        if(texVal<0)texVal=0;
        AlfaIntensity += texVal;
    }
    AlfaIntensity/=128;// <- Transparent
    
    
    //vec2 Coords = vec2(WorldSpaceTop.x /100,WorldSpaceTop.z /100);
    //vec2 Coords2 = vec2(WorldSpaceBottom.x,WorldSpaceBottom.z);

    //vec3 col, col2;
    //col = texture(WorleyNoise, ScreenCoord).rgb;
    //col = texture(DetailTex[20], (Coords)).rgb;


    //col2 = texture(WorleyNoise, (Coords2)).rgb;

   // vec3 colUndcol2 = vec3((col.r+col2.r)/2,(col.r+col2.r)/2,(col.r+col2.r)/2);

    //FragColor = vec4(colUndcol2.rgb,colUndcol2.r);

    float clampVar=0.0f;

    AlfaIntensity-=clampVar;
    if(AlfaIntensity<0)AlfaIntensity=0;


    //FragColor = vec4(col.rgb,col.r);
    //if(AlfaIntensity<.6f)AlfaIntensity=0;
    FragColor = vec4(1,1,1,AlfaIntensity);


    //INSIDE

    if(CameraPosition.y > 80 && CameraPosition.y < 120){

        int level = int(((CameraPosition.y-80) / 40) * 128);
        float AlfaIntensityInside=0;

        
        if(StrahlLaenge>0){ 
             for(int i=0;i<(128-level);i++){

                 vec2 coor = vec2(WorldSpaceBottom.x/div+((WorldSpaceTop.x/div - WorldSpaceBottom.x/div)/128)*i,WorldSpaceBottom.z/div+((WorldSpaceTop.z/div - WorldSpaceBottom.z/div)/128)*i);
                 
                 float texVal=texture(DetailTex[i], coor).r;
                 texVal*=static_noise;
                 if(texVal>1)texVal=1;
                 if(texVal<0)texVal=0;
                 AlfaIntensityInside += texVal;

                 //AlfaIntensityInside += texture(DetailTex[i], coor).r;
            }
            AlfaIntensityInside/=(128/*-level*/);

            AlfaIntensityInside-=clampVar;
            if(AlfaIntensityInside<0)AlfaIntensityInside=0;

            FragColor = vec4(1,1,1,AlfaIntensityInside);
        }
        
        else{
        //INCORRECT
         for(int i=level;i<128;i++){

         float z=128;
                 vec2 coor = vec2(WorldSpaceBottom.x/div+((WorldSpaceTop.x/div - WorldSpaceBottom.x/div)/z)*i,WorldSpaceBottom.z/div+((WorldSpaceTop.z/div - WorldSpaceBottom.z/div)/z)*i);
                 
                 float texVal=texture(DetailTex[i], coor).r;
                 texVal*=static_noise;
                 if(texVal>1)texVal=1;
                 if(texVal<0)texVal=0;
                 AlfaIntensityInside += texVal;

                 //AlfaIntensityInside += texture(DetailTex[i], coor).r;

            }
            AlfaIntensityInside/=(128/*-level*/);
            
            AlfaIntensityInside-=clampVar;
            if(AlfaIntensityInside<0)AlfaIntensityInside=0;

            FragColor = vec4(1,1,1,AlfaIntensityInside);
        }


        //FragColor = vec4(1,1,1,AlfaIntensityInside);
    }

    //if(StrahlLaenge<0){FragColor = vec4(1,0,0,1);} //NEGATIV WENN CAMERA ÜBER y = 0!!

      //FragColor = vec4(dist/10,dist/10,dist/10,1);
    //FragColor = vec4(CameraPosition.rgb,1);

    // final fog
    //vec3 cFog = vec3(0.95f, 0.95, 1);
    //float atmos = athmospheric(dist);
    //FragColor = FragColor * (1.0-atmos) + vec4(cFog * atmos,0);  
    
    FadeOut(CameraPosition,WorldSpaceBottom,200,400);

    //vec2 co=vec2(WorldSpaceBottom.x,WorldSpaceBottom.y);
    //float rand = fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
    //float static_noise=(0.5f-rand) / 2;

    //if(FragColor.a != 0)
   // FragColor=vec4(FragColor.r,FragColor.g,FragColor.b,FragColor.a + static_noise);
}
