attribute vec3 position;
attribute vec4 color;
attribute vec3 normal;

uniform mat4 worldViewProjectionMatrix;
uniform mat4 worldMatrix;

varying vec3 worldPosVarying;
varying vec4 colorVarying;
varying vec3 normalVarying;

void main()
{
	worldPosVarying = (worldMatrix * vec4(position,1)).xyz;
	colorVarying = color;
	normalVarying = normal;
	gl_Position = worldViewProjectionMatrix * vec4(position, 1.0);
}
