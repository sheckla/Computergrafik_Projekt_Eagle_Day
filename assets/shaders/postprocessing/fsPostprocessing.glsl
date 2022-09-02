#version 400
in vec2 Texcoord;
in vec4 Position;

out vec4 FragColor;

uniform sampler2D ScreenTexture;
uniform int GaussianBlur;
uniform float ElapsedTime;
uniform float TimeMaxPostProcessing;
uniform int Shake;
uniform float ShakeTime;
uniform float HP; // [0,100]
uniform int SepiaEnabled;

uniform float AspectWidth;
uniform float AspectHeight;

const float offset = 1.0 / 700.0;  

float remapBounds(in float i, in float fromMin, in float fromMax, in float toMin, in float toMax)
{
    return (i - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}

void main()
{
    float normX = (gl_FragCoord.x / AspectWidth * 2);
    float normY = (gl_FragCoord.y / AspectHeight * 2);
    if (Shake == 1)
    {
        normX += cos(ShakeTime*45) / 800;
        normY += cos(ShakeTime*100) / 400;
    }
    FragColor = texture(ScreenTexture, vec2(normX/2, normY/2));

    if (GaussianBlur == 1 && ElapsedTime > 0) {

        vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
        ); 

        // Gaussian Kernel
        float kFactor = 20;
        float kernel[9] = float[](
        1.0 / kFactor, 2.0 / kFactor, 1.0 / kFactor,
        2.0 / kFactor, 4.0 / kFactor, 2.0 / kFactor,
        1.0 / kFactor, 2.0 / kFactor, 1.0 / kFactor  
        );
        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(ScreenTexture, vec2(normX/2, normY/2).xy + offsets[i]));
        }
        vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++)
            col += sampleTex[i] * kernel[i];
        
        vec4 GaussianFragColor = vec4(col, 1.0); 
        FragColor = mix(FragColor, GaussianFragColor, remapBounds(ElapsedTime, 0, TimeMaxPostProcessing, 0, 1)); // Mix with Gauss

        // Vignette
        vec2 norm = vec2(normX/2-0.5, normY/2-0.5); // [-0.5,0.5]
        FragColor = mix(FragColor, vec4(0,0,0,1), (abs(norm.x) + abs(norm.y)) * remapBounds(ElapsedTime, 0, TimeMaxPostProcessing, 0, 1)); // Mix with Vignette
    }

    // Sepia Color Adjust
    if (SepiaEnabled == 1)
    {
        float sepiaRed = (FragColor.r * .393) + (FragColor.g *.769) + (FragColor.b * .189);
        float sepiaGreen = (FragColor.r * .349) + (FragColor.g *.686) + (FragColor.b * .168);
        float sepiaBlue = (FragColor.r * .272) + (FragColor.g *.534) + (FragColor.b * .131);
        vec4 SepiaColor = vec4(sepiaRed,sepiaGreen,sepiaBlue, 1);
        FragColor = mix(FragColor, SepiaColor, 0.3);
    }

    // Red Vignette Low HP
    if (HP >= 0 && HP < 50) 
    {
        vec2 norm = vec2(normX/2-0.5, normY/2-0.5); // [-0.5,0.5]
        vec4 BloodFragColor = mix(FragColor, vec4(0.5,0.05,0,1), (abs(norm.x) + abs(norm.y)));
        float f = remapBounds(HP, 0, 50, 0.5, 1);
        FragColor = mix (BloodFragColor, FragColor, f);
    }
}
