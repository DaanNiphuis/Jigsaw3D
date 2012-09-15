varying vec2 depthVarying;
varying vec3 normalVarying;

uniform float nearPlane;
uniform float farPlane;

vec4 encode(float v) 
{
	vec4 enc = vec4(1.0, 255.0, 65025.0, 160581375.0) * v;
	enc = fract(enc);
	enc -= enc.yzww * vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);
	return enc;
}

float projToView(float z)
{
	return (2 * nearPlane * farPlane) / (z * (farPlane - nearPlane) - farPlane - nearPlane);
}

void main()
{
	gl_FragColor = vec4(normalize(normalVarying), projToView(depthVarying.x / depthVarying.y));
}