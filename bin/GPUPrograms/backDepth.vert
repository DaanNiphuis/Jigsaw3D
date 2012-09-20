uniform mat4 worldViewProjectionMatrix;

in vec4 position;

out vec2 depthVarying;

void main()
{
	vec4 pos = worldViewProjectionMatrix * position;
	gl_Position = pos;
	depthVarying = pos.zw;
}