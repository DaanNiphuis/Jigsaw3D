uniform mat4 worldViewProjectionMatrix;

in vec4 position;

out vec2 depthVar;

void main()
{
	vec4 pos = worldViewProjectionMatrix * position;
	gl_Position = pos;
	depthVar = pos.zw;
}