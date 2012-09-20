in vec2 depthVarying;
in vec3 normalVarying;

uniform float nearPlane;
uniform float farPlane;

float projToView(float z)
{
	return (2 * nearPlane * farPlane) / (z * (farPlane - nearPlane) - farPlane - nearPlane);
}

void main()
{
	gl_FragColor = vec4(normalize(normalVarying), projToView(depthVarying.x / depthVarying.y));
}