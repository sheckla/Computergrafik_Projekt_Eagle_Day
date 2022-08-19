#version 400
in vec2 Texcoord;
in vec4 Position;

out vec4 FragColor;

uniform sampler2D Tex;
uniform vec3 Color;
uniform int ConstantColorMode; // bool
uniform int IsGUITex; // bool

void main()
{
    FragColor = vec4(Color, 1);

    if (IsGUITex == 1) {
        float normX = (gl_FragCoord.x / 1920.0f * 2);
        float normY = (gl_FragCoord.y / 1080.0f * 2);
        FragColor = texture(Tex, Texcoord);
        if (ConstantColorMode == 1) {
            FragColor = vec4(Color, FragColor.w);
        }
    }
}
