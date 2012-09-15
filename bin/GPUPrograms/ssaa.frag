uniform sampler2D depthNormalTexture;
uniform sampler2D backDepthTexture;
uniform sampler2D normalTexture;
uniform float nearPlane;
uniform float farPlane;

varying vec2 textureCoordinateVarying;

void main()
{
	vec4 nd = texture2D(depthNormalTexture, textureCoordinateVarying);
	gl_FragColor = vec4(nd.xyz,1);
}
