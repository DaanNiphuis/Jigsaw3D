uniform mat4 worldViewProjectionMatrix;
uniform mat4 worldMatrix;

in vec4 position;
in vec3 normal;

out vec2 depthVarying;
out vec3 normalVarying;

void main()
{
	vec4 pos = worldViewProjectionMatrix * position;
	gl_Position = pos;
	depthVarying = pos.zw;
	normalVarying = mat3(worldMatrix) * normal;	
}