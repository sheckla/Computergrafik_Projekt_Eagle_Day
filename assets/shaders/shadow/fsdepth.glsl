#version 400
in vec4 Position;
out vec4 FragColor;

uniform vec3 EyePos;

void main()
{
	vec4 Pos = Position / Position.w;
	FragColor = vec4(Pos.z,0,0,1);
	FragColor = vec4(1,1,1,1);
}
