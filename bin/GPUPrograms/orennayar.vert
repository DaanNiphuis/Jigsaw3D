uniform mat4 worldViewProjectionMatrix;
uniform mat4 worldMatrix;

in vec3 position;
in vec4 color;
in vec3 normal;

out vec3 worldPosVarying;
out vec4 colorVarying;
out vec3 normalVarying;

void main()
{
	worldPosVarying = (worldMatrix * vec4(position,1)).xyz;
	colorVarying = color;
	normalVarying = normal;
	gl_Position = worldViewProjectionMatrix * vec4(position, 1.0);
}
