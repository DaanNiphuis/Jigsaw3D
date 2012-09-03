varying vec4 positionVarying;

uniform sampler2D depthTexture;
uniform sampler2D backDepthTexture;
uniform sampler2D normalTexture;
uniform float nearPlane;
uniform float farPlane;

float decode(vec4 rgba) 
{
	return dot(rgba, vec4(1.0, 1.0/255.0, 1.0/65025.0, 1.0/160581375.0));
}

float projToWorld(float z)
{
	return (2.0*nearPlane*farPlane) / (z*(farPlane-nearPlane)-farPlane-nearPlane);
}

void main()
{
	vec2 texCoord = (positionVarying.xy / positionVarying.w) * 0.5 + 0.5;
	float d = decode(texture2D(depthTexture, texCoord));
	gl_FragColor = vec4(d,d,d,1);
}
