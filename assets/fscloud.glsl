#version 400
uniform vec3 EyePos;

in vec3 Position;
in vec2 Texcoord;
out vec4 FragColor;


uniform sampler2D DiffuseTexture; // Worley
uniform sampler2D NormalTexture; // noise

// stopped working?? Wtf
uniform sampler2D DetailTex[30]; 
uniform sampler2D noise;

uniform mat4 InverseViewMatrix;
uniform mat4 InverseProjectionMatrix;

uniform vec3 boundsMin;
uniform vec3 boundsMax;

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
    vec4 col = texture(DiffuseTexture, vec2(normX, normY)) + texture(NormalTexture, vec2(normX, normY));

    float density = col.r;
    return density;
}

// learnopengl
float near = 0.1; 
float far  = 100.0; 
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}


// Matrix-Transformationen von rechts nach links
void main()
{
    float normX = (gl_FragCoord.x / 1920.0f * 2)/2 - 1;
    float normY = (gl_FragCoord.y / 1080.0f * 2)/2 - 1;
    vec4 col = texture(noise, Texcoord); // Texcoord

    mat4 view = InverseViewMatrix;
    mat4 proj = InverseProjectionMatrix;
    vec3 v = vec3(proj * vec4(normX,normY,0,0));
    vec3 rayOrigin2 = vec3(view[0][3], view[1][3], view[2][3]);
    vec3 ray2 = vec3(view * vec4(v,0));


    // start
    vec3 rayOrigin = EyePos;
    vec3 rayDir = Position - EyePos;

    // bounds info
    vec2 boxInfo = rayBoxDst(rayOrigin, rayDir);
    float dstToBox = boxInfo.x;
    float dstInsideBox = boxInfo.y;

     // marching
    float NumSteps = 5;
    float dstTravelled = 0;
    float stepSize = dstInsideBox / NumSteps;
    float depth = length(rayDir) * 2;
    float dstLimit = dstInsideBox;//min(depth - dstToBox, dstInsideBox);

    float totalDensity = 0.0f;
    while (dstTravelled < dstLimit) {
        vec3 rayPos = Position * rayDir + dstTravelled;//rayOrigin * rayDir *(dstToBox + dstTravelled);
        totalDensity += sampleDensity(rayPos) * stepSize;
        dstTravelled += stepSize;
    }
    float t = exp(-totalDensity);
    float a = totalDensity;

    // debug float
    vec4 marker = vec4(1,0,0,1);

    if (totalDensity <= 0.25) {
        a = 0;
    }
    if (t <= 0.35) {
        t = mix(t,0.35,1.0);
    }
    FragColor = vec4(a,a,a,1) * t ;
    if ( a >= 0.8) a = 0.8;
    FragColor.w = a;
    if (dstInsideBox >= dstToBox && dstToBox <= 0.6) {
        FragColor = vec4(0,0,0,1);
    }

    if (FragColor.w <= 0.15) {
        FragColor.w = 0;
    }


    // final fog
    //vec3 cFog = vec3(0.95f, 0.95, 1);
    //float atmos = athmospheric(dist);
    //FragColor = FragColor * (1.0-atmos) + vec4(cFog * atmos,0);  
}
