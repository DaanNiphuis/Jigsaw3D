attribute vec4 position;

uniform mat4 worldViewProjectionMatrix;

varying vec4 positionVarying;

void main()
{
	vec4 pos = worldViewProjectionMatrix * position;
	gl_Position = pos;
	positionVarying = pos;
}