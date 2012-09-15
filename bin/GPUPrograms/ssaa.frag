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

float projToView(float z)
{
	return (nearPlane*farPlane) / (z*farPlane-z*nearPlane-farPlane);
}

void main()
{
	//vec2 texCoord = (positionVarying.xy / positionVarying.w) * 0.5 + 0.5;
	//float d = projToView(decode(texture2D(depthTexture, texCoord)));
	
	float d = projToView(decode(texelFetch(depthTexture, ivec2(gl_FragCoord.xy), 0)));
	d *= -0.01;
	gl_FragColor = vec4(d,d,d,1);
}
