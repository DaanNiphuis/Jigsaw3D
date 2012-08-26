attribute vec4 position;

uniform mat4 modelViewProjectionMatrix;

varying vec4 positionVarying;

void main()
{
	vec4 pos = modelViewProjectionMatrix * position;
	gl_Position = pos;
	positionVarying = pos;
}