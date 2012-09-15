varying vec4 positionVarying;

vec4 encode(float v) 
{
	vec4 enc = vec4(1.0, 255.0, 65025.0, 160581375.0) * v;
	enc = fract(enc);
	enc -= enc.yzww * vec4(1.0/255.0,1.0/255.0,1.0/255.0,0.0);
	return enc;
}

void main()
{
	gl_FragColor = encode((positionVarying.z / positionVarying.w)*0.5+0.5);
}