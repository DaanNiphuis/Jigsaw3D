attribute vec3 position;
attribute vec4 color;
attribute vec3 normal;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

varying vec3 worldPosVarying;
varying vec4 colorVarying;
varying vec3 normalVarying;

void main()
{
	worldPosVarying = (modelMatrix * vec4(position,1)).xyz;
	colorVarying = color;
	normalVarying = normal;
	gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}
