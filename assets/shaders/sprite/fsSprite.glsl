#version 400
in vec3 Position;
in vec2 Texcoord;
out vec4 FragColor;
uniform sampler2D DiffuseTexture;
uniform float transparency_value;

void main()
{
    
    vec4 textureData = texture(DiffuseTexture, Texcoord);
    if(textureData.a > .2)
        FragColor = vec4(textureData.rgb,transparency_value);
    else
        discard;
}