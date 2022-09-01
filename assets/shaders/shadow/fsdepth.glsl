#version 400
in vec4 Position;
out vec4 FragColor;

uniform vec3 EyePos;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	vec4 Pos = Position / Position.w;
	 float depth = LinearizeDepth(Pos.z); 
	FragColor = vec4(depth,0,0,1);
}
