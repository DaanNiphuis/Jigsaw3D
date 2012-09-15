attribute vec4 position;
attribute vec3 normal;

uniform mat4 worldViewProjectionMatrix;
uniform mat4 worldMatrix;

varying vec2 depthVarying;
varying vec3 normalVarying;

void main()
{
	vec4 pos = worldViewProjectionMatrix * position;
	gl_Position = pos;
	depthVarying = pos.zw;
	normalVarying = mat3(worldMatrix) * normal;	
}