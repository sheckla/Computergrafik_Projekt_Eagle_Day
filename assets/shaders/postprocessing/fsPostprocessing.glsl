#version 400
in vec2 Texcoord;
in vec4 Position;

out vec4 FragColor;

// From PostProcessingBuffer, contains pre-rendered Scene!
uniform sampler2D ScreenTexture; 
uniform float ElapsedTime;
uniform float ShakeTime;
uniform float TimeMaxPostProcessing;
uniform float HP; // [0,100]

// Bools - PostProcessingEffects
uniform int Shake;
uniform int GaussianBlur;
uniform int SepiaEnabled;

// Application Width/Height
uniform float AspectWidth;
uniform float AspectHeight;

// Gaussian Blur px-offset
const float offset = 1.0 / 700.0;  

float remapBounds(in float i, in float fromMin, in float fromMax, in float toMin, in float toMax)
{
    return (i - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}

void main()
{
    float normX = (gl_FragCoord.x / AspectWidth * 2);
    float normY = (gl_FragCoord.y / AspectHeight * 2);

    // Apply Screen shake, normXY will be translated by small offset
    if (Shake == 1)
    {
        normX += cos(ShakeTime*45) / 800;
        normY += cos(ShakeTime*100) / 400;
    }

    // Simple texture-coloring from previously rendered scene (ScreenTexture)
    FragColor = texture(ScreenTexture, vec2(normX/2, normY/2));

    if (GaussianBlur == 1 && ElapsedTime > 0) {

        // Pixel Offsets
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
        float strength = 20; 
        float kernel[9] = float[](
        1.0 / strength, 2.0 / strength, 1.0 / strength,
        2.0 / strength, 4.0 / strength, 2.0 / strength,
        1.0 / strength, 2.0 / strength, 1.0 / strength  
        );

        // get bounding Pixel from current normXY from ScreenTexture
        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(ScreenTexture, vec2(normX/2, normY/2).xy + offsets[i]));
        }

        // Apply sampled pixels from ScreenTexture with GaussianKernel
        vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++)
            col += sampleTex[i] * kernel[i];

        // Mix with Gauss, remapped to ElapsedTime        
        vec4 GaussianFragColor = vec4(col, 1.0); 
        FragColor = mix(FragColor, GaussianFragColor, remapBounds(ElapsedTime, 0, TimeMaxPostProcessing, 0, 1)); 

        // Mix with Vignette, remapped to ElapsedTime
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

    // Red Vignette - Low HP
    if (HP >= 0 && HP < 50) 
    {
        vec2 norm = vec2(normX/2-0.5, normY/2-0.5); // [-0.5,0.5]
        vec4 BloodFragColor = mix(FragColor, vec4(0.5,0.05,0,1), (abs(norm.x) + abs(norm.y)));
        float f = remapBounds(HP, 0, 50, 0.5, 1);
        FragColor = mix (BloodFragColor, FragColor, f);
    }
}
