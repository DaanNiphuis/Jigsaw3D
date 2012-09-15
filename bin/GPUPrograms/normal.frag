varying vec3 normalVarying;

void main()
{
	gl_FragColor = vec4(normalize(normalVarying) * 0.5 + 0.5, 1);
}