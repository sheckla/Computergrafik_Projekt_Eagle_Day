#version 400

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;

out vec4 FragColor;

uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;
uniform sampler2D DiffuseTexture;

// Exercise 2
const int MAX_LIGHTS=14;
struct Light
{
	int Type;
	vec3 Color;
	vec3 Position;
	vec3 Direction;
	vec3 Attenuation;
	vec3 SpotRadius;
	int ShadowIndex;
};

uniform Lights // uniform, globally saved on graphics card (doesn't change from one shader invocation to the next)
{
	int LightCount;
	Light lights[MAX_LIGHTS];
};

float sat( in float a)
{
    return clamp(a, 0.0, 1.0);
}
// https://learnopengl.com/Lighting/Light-casters
void main()
{
    vec4 DiffTex = texture( DiffuseTexture, Texcoord); // texture
    if(DiffTex.a <0.3f) discard;
    vec3 N = normalize(Normal);             // normal on the surface
    vec3 L = normalize(LightPos-Position);  // dir-vec: pos->lightPos
    vec3 E = normalize(EyePos-Position);    // dir-vec: pos->EyePos
    vec3 R = reflect(-L,N);                 // reflection from L on surface

    
    vec3 AllLightColor = vec3(0,0,0);       // total accumulative color value for upcoming for-loop
    for (int i = 0; i < LightCount; i++) {
        float Attenuation = 1.0;            // light intensity drop-off
        vec3 Pos_To_LightPos = lights[i].Position - Position;       // basically just L for current light
        float Pos_To_LightPos_distance = length(Pos_To_LightPos);   // length of L

        // Directional Light (light source distance = infinity -> rays are parallel)
        if (lights[i].Type == 1)
        {
            L = normalize(-lights[i].Direction); // common notation for dir-light = negated, light points to pos!
            // keep Attenuation at 1.0, no intensity drop-off!
        }
        // Point light (scatters in all directions from given points)
        else if (lights[i].Type == 0) {
            L = normalize(Pos_To_LightPos);
            // Intensity=1.0/( Attenuation.x + Attenuation.y*Dist + Attenuation.z*Dist*Dist). Dist
            Attenuation /= length(lights[i].Attenuation.x + lights[i].Attenuation.y*Pos_To_LightPos_distance + 
                            lights[i].Attenuation.z * Pos_To_LightPos_distance * Pos_To_LightPos_distance);
        }
        // Spot light (shoots in specific direction)
        else if (lights[i].Type == 2) {
            L = normalize(Pos_To_LightPos);
            Attenuation /= lights[i].Attenuation.x + lights[i].Attenuation.y*Pos_To_LightPos_distance + 
                            lights[i].Attenuation.z * Pos_To_LightPos_distance * Pos_To_LightPos_distance;
            // See lecture 3: 1-saturate(sigma-pi/po-pi)
            float sigma = dot(normalize(L), normalize(-lights[i].Direction)); // returns COSINE of the angle between the two vec (DEG) (0-180) (negate dir)
            float phi_i = lights[i].SpotRadius.x; // inner radius (RAD)
            float phi_o = lights[i].SpotRadius.y; // outer radius (RAD)
            float cutoff = (acos(sigma)-phi_i) / (phi_o - phi_i); // acos() [0,pi] to get RAD from COSINE, math libs use RAD most of the times
            Attenuation *= 1-sat(cutoff);
        }


        vec3 LightColor = lights[i].Color * Attenuation; // adjust for light drop-off
        vec3 H = normalize(L + E); // Exercise 1 - just normalize() it lol

        vec3 DiffuseComponent = LightColor * DiffuseColor * sat(dot(N,L)); // current light diff-comp
        // https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
        vec3 SpecularComponent = LightColor * SpecularColor * pow(sat(dot(H,N)), SpecularExp); // Exercise 1, current light spec-comp
        AllLightColor += DiffuseComponent * DiffTex.rbg + SpecularComponent; // add current resulting LightColor to TotalLightColor to sum up all Light-Source colors
    }

    FragColor = vec4(AllLightColor + AmbientColor * DiffTex.rgb ,DiffTex.a);
}
