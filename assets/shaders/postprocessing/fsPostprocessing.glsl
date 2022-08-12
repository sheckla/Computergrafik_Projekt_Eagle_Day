#version 400
in vec2 Texcoord;
in vec4 Position;

out vec4 FragColor;

uniform sampler2D ScreenTexture;

const float offset = 1.0 / 300.0;  

void main()
{
    float normX = (gl_FragCoord.x / 1920.0f * 2);
    float normY = (gl_FragCoord.y / 1080.0f * 2);
    FragColor = texture(ScreenTexture, vec2(normX/2, normY/2));
    //FragColor = mix(vec4(0,1,0,1), FragColor, 0.9);
    //FragColor = vec4(vec3(1.0 - texture(ScreenTexture, vec2(normX/2, normY/2))), 1.0);

    
/*     vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    ); */

/*     float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    ); */

/*     float kernel[9] = float[](
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
    
    
    FragColor = vec4(col, 1.0); */
}
