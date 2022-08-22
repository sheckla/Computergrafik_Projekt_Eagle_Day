#version 400
in vec2 Texcoord;
in vec4 Position;

out vec4 FragColor;

uniform sampler2D ScreenTexture;
uniform int GaussianBlur;
uniform float ElapsedTime;
uniform float TimeMaxPostProcessing;

const float offset = 1.0 / 300.0;  

float remapBounds(in float i, in float fromMin, in float fromMax, in float toMin, in float toMax)
{
    return (i - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}

void main()
{
    float normX = (gl_FragCoord.x / 1920.0f * 2);
    float normY = (gl_FragCoord.y / 1080.0f * 2);
    FragColor = texture(ScreenTexture, vec2(normX/2, normY/2));


    
    if (GaussianBlur == 1 && ElapsedTime > 0) {
        //FragColor.w = FragColor.w/2;

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

        float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
        );
        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(ScreenTexture, vec2(normX/2, normY/2).st + offsets[i]));
        }
        vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++)
            col += sampleTex[i] * kernel[i];
        
        vec4 GaussianFragColor = vec4(col, 1.0); 
        FragColor = mix(FragColor, GaussianFragColor, remapBounds(ElapsedTime, 0, TimeMaxPostProcessing, 0, 1));
        vec2 norm = vec2(normX/2-0.5, normY/2-0.5); // [-0.5,0.5]
        vec4 darkness = vec4(0,0,0,1);
        FragColor = mix(FragColor, vec4(0,0,0,1), (abs(norm.x) + abs(norm.y)) * remapBounds(ElapsedTime, 0, TimeMaxPostProcessing, 0, 1));
        //FragColor = vec4(norm, 0, 1);
    }

    float sepiaRed = (FragColor.r * .393) + (FragColor.g *.769) + (FragColor.b * .189);
    float sepiaGreen = (FragColor.r * .349) + (FragColor.g *.686) + (FragColor.b * .168);
    float sepiaBlue = (FragColor.r * .272) + (FragColor.g *.534) + (FragColor.b * .131);
    vec4 SepiaColor = vec4(sepiaRed,sepiaGreen,sepiaBlue, 1);
    FragColor = mix(FragColor, SepiaColor, 0.3);


/*     float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    ); */


}
