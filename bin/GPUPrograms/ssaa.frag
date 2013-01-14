in vec2 textureCoordinateVar;

out vec4 fragColor;

uniform sampler2D colorTexture;
uniform sampler2D depthNormalTexture;
uniform sampler2D backDepthTexture;
uniform sampler2D noiseTexture;
uniform float nearPlane;
uniform float farPlane;

void main()
{
	ivec2 texCoord = ivec2(gl_FragCoord.xy);
	vec4 c = texelFetch(colorTexture, texCoord, 0);
	vec4 nd = texelFetch(depthNormalTexture, texCoord, 0);
	vec4 bd = texelFetch(backDepthTexture, texCoord, 0);
	vec4 noise = texture2D(noiseTexture, textureCoordinateVar);
	
	fragColor = vec4(noise.xyz, 1);
}
