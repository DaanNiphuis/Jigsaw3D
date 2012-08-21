attribute vec4 position;
attribute vec3 normal;

uniform mat4 modelViewProjectionMatrix;

varying vec3 normalVarying;

void main()
{
	gl_Position = modelViewProjectionMatrix * position;
	normalVarying = normal;
};