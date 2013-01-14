in vec2 depthVar;
in vec3 normalVar;

out vec4 fragColor;

uniform float nearPlane;
uniform float farPlane;

float projToLinDepth(float z)
{
	return -(2 * nearPlane * farPlane) / (z * (farPlane - nearPlane) - farPlane - nearPlane);
}

void main()
{
	fragColor = vec4(normalize(normalVar), projToLinDepth(depthVar.x / depthVar.y));
}