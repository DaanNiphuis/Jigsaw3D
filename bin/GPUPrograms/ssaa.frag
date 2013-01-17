in vec2 textureCoordinateVar;

out vec4 fragColor;

uniform sampler2D colorTexture;
uniform sampler2D depthNormalTexture;
uniform sampler2D backDepthTexture;
uniform sampler2D noiseTexture;
uniform float nearPlane;
uniform float farPlane;
uniform float aspectRatio;

uniform mat4 viewInv;

float linToProjDepth(float z)
{
	return (((-2*nearPlane*farPlane)/z)+farPlane+nearPlane)/(farPlane-nearPlane);
}

void main()
{
	ivec2 texCoord = ivec2(gl_FragCoord);
	vec4 c = texelFetch(colorTexture, texCoord, 0);
	vec4 nd = texelFetch(depthNormalTexture, texCoord, 0);
	vec4 bd = texelFetch(backDepthTexture, texCoord, 0);

	float zProj = linToProjDepth(nd.w);
	vec4 posView = vec4(((textureCoordinateVar*2-1))*nd.w, -nd.w, 1);
	posView.x *= aspectRatio;
	vec4 posWorld = viewInv * posView;

	/*float projZ = linToProjDepth(nd.w);
	if (projZ <= 0 && projZ >= -1)
		fragColor = vec4(1,0,1,1);
	else
		fragColor = vec4(0,1,0,1);
	return;*/

	fragColor = c;

	float occlusion = 0;

	for (int i = 0; i < 100; ++i)
	{
		ivec2 noiseCoord = ivec2(i % 10, i / 10);
		vec2 randDir = texelFetch(noiseTexture, noiseCoord, 0).xy;
		randDir *= 2;
		randDir -= 1;
		randDir *= 0.01;
				
		vec2 posScreenSample = textureCoordinateVar + randDir;
		vec4 ndSample = texture2D(depthNormalTexture, posScreenSample);
		vec4 posViewSample =  vec4((posScreenSample*2-1)*ndSample.w, -ndSample.w, 1);
		posViewSample.x *= aspectRatio;
		vec4 posWorldSample = viewInv * posViewSample;
		//fragColor = vec4(nd.w * 0.01, nd.w * 0.01, nd.w * 0.01, 1);
		//fragColor = vec4(sampleDepth * 0.01, sampleDepth * 0.01, sampleDepth * 0.01, 1);

		float dDepth = nd.w - ndSample.w;
		if (dDepth > 0 && dDepth < 4 && dot(nd.xyz, posWorldSample.xyz-posWorld.xyz)>0)
			occlusion += 0.005 * (1 - dot(nd.xyz, ndSample.xyz));

		/*occlusion += nd.w - sampleDepth;
		if (abs(nd.w-sampleDepth) < 2)
			fragColor = vec4(1,0,1,1);
		else
			fragColor = vec4(0,0,0,1);*/
	}

	fragColor *= 1 - occlusion;
}
