#version 400
const int MAX_LIGHTS=14;
uniform vec3 LightPos;
uniform vec3 LightColor;

// in from vs
in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;

// Phong Values
uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

uniform sampler2D DiffuseTexture;
uniform samplerCube CubeMapTexture;
uniform sampler2D ShadowMapTexture[MAX_LIGHTS];
uniform mat4 ShadowMapMat[MAX_LIGHTS];
uniform float PhongDiff; // Phong Component Divisor

uniform int IgnoreLight;

// Booleans
uniform int ShadowOnly; 
uniform int Cubemapping;

out vec4 FragColor;

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

// sources: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
//          http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/
float shadowAmount(int LightIndex, vec3 LightDir, float cosTheta) {

	float shadow = 0.0f;  							
	vec4 ShadowMapPos = ShadowMapMat[LightIndex] * vec4(Position, 1);		// Position in ShadowMapMat
	ShadowMapPos.xyz /= ShadowMapPos.w;                                     // perspective division
	ShadowMapPos.xy = ShadowMapPos.xy*0.5 + 0.5; 							// NormScreenCords[-1,1] to TexCoords [0,1]
    
    // Discard if no shadow possible
    if (ShadowMapPos.z > 1.0) {
        shadow = 0;
        return shadow;
    }
												
	// PCF (percentage closer filtering)
    float bias = 0.05*tan(acos(cosTheta)); 
	int smoothRange = 5, i = LightIndex;						
	vec2 texelSize = 1.0 / textureSize(ShadowMapTexture[i], 0);		
	for(int x = -smoothRange; x <= smoothRange; x++){	
		for(int y = -smoothRange; y <= smoothRange; y++) {	
			vec2 shadowMapFragXY = ShadowMapPos.xy + vec2(x, y) * texelSize;		
			vec4 shadowMapFragXYDepth = texture(ShadowMapTexture[i], shadowMapFragXY);	
			if(shadowMapFragXYDepth.r + bias < ShadowMapPos.z) shadow += 1.0;       	
		}    
	} 
	shadow /= (smoothRange * smoothRange);				// normalize the shadow value
	return shadow;
}


// https://learnopengl.com/Lighting/Light-casters
void main()
{
    vec4 DiffTex = texture( DiffuseTexture, Texcoord); // texture
    if(DiffTex.a <0.3f) {
        FragColor = vec4(1,0,0,1);
        discard;
    }
    vec3 N = normalize(Normal);             // normal on the surface
    vec3 L = normalize(LightPos-Position);  // dir-vec: pos->lightPos
    vec3 E = normalize(EyePos-Position);    // dir-vec: pos->EyePos
    vec3 R = reflect(-L,N);                 // reflection from L on surface

    vec3 DiffuseComponent = vec3(0,0,0);
    vec3 SpecularComponent = vec3(0,0,0);
    vec3 ReflectComponent = vec3(0,0,0);
    vec3 Reflection = vec3(0,0,0);
    float visibility = 0.0f;
    for (int i = 0; i < LightCount; i++) {
        if (i != IgnoreLight) {
        float Attenuation = 1.0f;            // light intensity drop-off
        vec3 Pos_To_LightPos = lights[i].Position - Position;       // basically just L for current light
        float Pos_To_LightPos_distance = length(Pos_To_LightPos);   // length of L
        L = normalize(lights[i].Position - Position);

        // Directional Light (light source distance = infinity -> rays are parallel)
        if (lights[i].Type == 1)
        {
            L = normalize(-lights[i].Direction); // common notation for dir-light = negated, light points to pos!
            // keep Attenuation at 1.0, no intensity drop-off!
            //L = lights[i].Direction;
            float cosTheta = sat(dot(N, L));
            visibility += 1.0f - shadowAmount(i, L, cosTheta);
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
            Attenuation *= 1.0f - sat(cutoff);
            visibility += 1.0f - shadowAmount(i, L, sat(dot(N, L)));
        }

        vec3 H = normalize(L + E); 
        vec3 LightColor = lights[i].Color * Attenuation; // adjust for light drop-off

        // CubeMap SpecularComponent
        vec3 o = normalize(Position - EyePos);
        vec3 p = reflect(o, normalize(Normal));
        Reflection = texture(CubeMapTexture, p).xyz / PhongDiff;

        ReflectComponent += LightColor * texture(CubeMapTexture, p).xyz;
        DiffuseComponent += LightColor * DiffuseColor * sat(dot(N,L)) / PhongDiff; 
        SpecularComponent += LightColor * SpecularColor * pow(sat(dot(H,N)), SpecularExp) / PhongDiff; 
        }
    }

    // Phong Components after calculating Shadows for each light
    vec3 diff = DiffuseComponent * visibility;
    vec3 spec = SpecularComponent * visibility;
    vec3 ambient = AmbientColor * DiffTex.rgb;
    vec3 phongColor = ambient + diff + spec;
    FragColor = vec4(phongColor, DiffTex.a);

    // CubeMap Reflection mixing
    if (Cubemapping == 1) 
    {
        float SpecularFactor = (SpecularColor.r + SpecularColor.g + SpecularColor.b) / 3;
        SpecularFactor = sat(SpecularFactor);
        vec3 mixed = mix(Reflection, phongColor, SpecularFactor);
        FragColor = vec4(mixed, DiffTex.a);
    }

    // Shadow Only Display
    if (ShadowOnly == 1 && visibility < 1) {
        FragColor = vec4(phongColor, (1 - visibility) * 0.1);
    } else if (ShadowOnly == 1) 
    {
        FragColor = vec4(1,0,0,0);
    }

    
}
