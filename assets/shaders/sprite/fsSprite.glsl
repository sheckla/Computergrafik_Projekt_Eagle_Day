#version 400
in vec3 Position;
in vec2 Texcoord;
out vec4 FragColor;
uniform sampler2D DiffuseTexture;
void main()
{
    
    vec4 textureData = texture(DiffuseTexture, Texcoord);
    if(textureData.a > .2)
        FragColor = vec4(textureData.rgb,.5);
    else
        discard;
}