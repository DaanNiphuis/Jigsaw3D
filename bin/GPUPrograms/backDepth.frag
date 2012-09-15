varying vec2 depthVarying;

uniform float nearPlane;
uniform float farPlane;

float projToView(float z)
{
	return (2 * nearPlane * farPlane) / (z * (farPlane - nearPlane) - farPlane - nearPlane);
}

void main()
{
	gl_FragColor = projToView(depthVarying.x / depthVarying.y);
}