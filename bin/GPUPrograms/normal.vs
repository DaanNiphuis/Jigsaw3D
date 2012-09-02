attribute vec4 position;
attribute vec3 normal;

uniform mat4 worldViewProjectionMatrix;
uniform mat4 worldMatrix;

varying vec3 normalVarying;

void main()
{
	gl_Position = worldViewProjectionMatrix * position;
	normalVarying = mat3(worldMatrix) * normal;
}