in vec2 depthVar;

out vec4 fragColor;

uniform float nearPlane;
uniform float farPlane;

float projToView(float z)
{
	return (2 * nearPlane * farPlane) / (z * (farPlane - nearPlane) - farPlane - nearPlane);
}

void main()
{
	fragColor = projToView(depthVar.x / depthVar.y);
}