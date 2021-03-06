in vec3 worldPosVar;
in vec4 colorVar;
in vec3 normalVar;

out vec4 fragColor;

uniform vec3 lightCol;
uniform vec3 lightDir;
uniform vec3 lightAmbient;
uniform vec3 cameraPos;
uniform float roughness;
uniform float albedo;

const float PI = 3.141592653589793;

void main()
{
	vec3 p = worldPosVar;

	vec3 n = normalize(normalVar);
	vec3 l = normalize(-lightDir);
	vec3 v = normalize(p - cameraPos);

	float variance = roughness * roughness;
	float A = 1.0 - (variance/(variance+0.33)) * 0.5;
	float B = ((variance*variance) / (variance*variance+0.09)) * 0.45;
	float ldn = dot(l, n);
	float vdn = dot(v, n);
	float theta_i = acos(ldn);
	float theta_r = acos(vdn);
	float alpha = max(theta_i, theta_r);
	float beta = min(theta_i, theta_r);
	float gamma = dot(v-n*vdn, l-n*ldn);
	float Li = 1.0;
	vec3 Lr = albedo/PI * max(0.0,ldn) * (A + (B * max(0.0, gamma) * sin(alpha) * tan(beta))) * lightCol;
	vec3 light = Lr + lightAmbient;
	
	fragColor = vec4(colorVar.xyz * light, colorVar.w);
}