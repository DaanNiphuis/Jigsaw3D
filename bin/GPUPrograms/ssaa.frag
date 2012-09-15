varying vec4 positionVarying;

uniform sampler2D depthNormalTexture;
uniform sampler2D backDepthTexture;
uniform sampler2D normalTexture;
uniform float nearPlane;
uniform float farPlane;

void main()
{
	vec4 nd = texelFetch(depthNormalTexture, ivec2(gl_FragCoord.xy), 0);
	gl_FragColor = vec4(nd.xyz,1);
}
