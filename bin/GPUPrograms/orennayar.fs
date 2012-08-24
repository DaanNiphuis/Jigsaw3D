varying vec4 positionVarying;
varying vec4 colorVarying;
varying vec3 normalVarying;

uniform vec3 lightCol;
uniform vec3 lightDir;
uniform vec3 lightAmbient;
uniform vec3 cameraPos;
uniform float roughness;
uniform float albedo;

const float PI = 3.141592653589793f;

void main()
{
	vec3 p = positionVarying.xyz / positionVarying.w;

	vec3 n = normalize(normalVarying);
	vec3 l = normalize(-lightDir);
	vec3 v = normalize(cameraPos - p);

	float variance = roughness * roughness;
	float A = 1 - (variance/(variance+0.33f)) * 0.5f;
	float B = ((variance*variance) / (variance*variance+0.09f)) * 0.45f;
	float ldn = dot(l, n);
	float vdn = dot(v, n);
	float theta_i = acos(ldn);
	float theta_r = acos(vdn);
	float alpha = max(theta_i, theta_r);
	float beta = min(theta_i, theta_r);
	float gamma = dot(v-n*vdn, l-n*ldn);
	float Li = 1;
	vec3 Lr = albedo/PI * max(0,ldn) * (A + (B * max(0, gamma) * sin(alpha) * tan(beta))) * lightCol;
	vec3 light = Lr + lightAmbient;
	gl_FragColor.xyz = colorVarying.xyz * light;
	gl_FragColor.w = colorVarying.w;
}