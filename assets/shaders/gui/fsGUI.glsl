#version 400
in vec2 Texcoord;
in vec4 Position;

out vec4 FragColor;

uniform sampler2D Tex;
uniform vec3 Color;

void main()
{
    float normX = (gl_FragCoord.x / 1920.0f * 2);
    float normY = (gl_FragCoord.y / 1080.0f * 2);
    FragColor = texture(Tex, Texcoord);
    if (FragColor.w > 0.1 && Color.r != -1) {
        FragColor = vec4(Color, 1);
    }
    //FragColor = mix(vec4(0,1,0,1), FragColor, 0.9);
}