attribute vec3 position;
attribute vec4 color;
attribute vec3 normal;

uniform mat4 modelViewProjectionMatrix;

varying vec4 positionVarying;
varying vec4 colorVarying;
varying vec3 normalVarying;

void main()
{
	positionVarying = modelViewProjectionMatrix * vec4(position, 1);
	colorVarying = color;
	normalVarying = normal;
	gl_Position = positionVarying;
};