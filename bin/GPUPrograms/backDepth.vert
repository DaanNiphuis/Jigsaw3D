attribute vec4 position;

uniform mat4 worldViewProjectionMatrix;

varying vec2 depthVarying;

void main()
{
	vec4 pos = worldViewProjectionMatrix * position;
	gl_Position = pos;
	depthVarying = pos.zw;
}