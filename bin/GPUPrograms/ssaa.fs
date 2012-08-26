varying vec4 positionVarying;

uniform sampler2D tex;

float decode(vec4 rgba) 
{
    return dot(rgba, vec4(1.0, 1.0/255.0, 1.0/65025.0, 1.0/160581375.0));
}

void main()
{
	vec2 texCoord = (positionVarying.xy / positionVarying.w) * 0.5 + 0.5;
	gl_FragColor = texture2D(tex, texCoord);
}