#version 400
in vec2 Texcoord;
in vec4 Position;

out vec4 FragColor;

uniform sampler2D Tex;
uniform vec3 Color;

// bools
uniform int ConstantColorMode; 
uniform int IsGUITex; 
uniform int MouseoverHighlight; 
uniform int MouseInside; 
uniform int IsFont;

// x, y, width, height
vec4 FontData = vec4(388, 184, 30, 30);

void main()
{
    FragColor = vec4(Color, 1);


    if (IsGUITex == 1) {
        float normX = (gl_FragCoord.x / 1920.0f * 2);
        float normY = (gl_FragCoord.y / 1080.0f * 2);
        FragColor = texture(Tex, Texcoord);

        if (IsFont == 1) {
            //FragColor = texture(Tex, vec2(388, 184));
        }

        if (ConstantColorMode == 1) {
            FragColor = vec4(Color, FragColor.w);
        }
        if (MouseoverHighlight == 1 && MouseInside == 1) {
            vec4 darkness = vec4(0,0,0,1);
            FragColor = vec4(mix(FragColor.xyz, darkness.xyz, 0.3), FragColor.w);
        }
    }
}
