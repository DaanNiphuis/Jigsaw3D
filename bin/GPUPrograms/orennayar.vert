in vec3 position;
in vec4 color;
in vec3 normal;

out vec3 worldPosVar;
out vec4 colorVar;
out vec3 normalVar;

uniform mat4 worldViewProjectionMatrix;
uniform mat4 worldMatrix;

void main()
{
	worldPosVar = (worldMatrix * vec4(position,1)).xyz;
	colorVar = color;
	normalVar = normal;
	gl_Position = worldViewProjectionMatrix * vec4(position, 1.0);
}
