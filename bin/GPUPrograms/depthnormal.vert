uniform mat4 worldViewProjectionMatrix;
uniform mat4 worldMatrix;

in vec4 position;
in vec3 normal;

out vec2 depthVar;
out vec3 normalVar;

void main()
{
	vec4 pos = worldViewProjectionMatrix * position;
	gl_Position = pos;
	depthVar = pos.zw;
	normalVar = mat3(worldMatrix) * normal;	
}