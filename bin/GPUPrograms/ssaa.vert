in vec4 position;
in vec2 textureCoordinate;

out vec2 textureCoordinateVarying;

void main()
{
	gl_Position = position;
	textureCoordinateVarying = textureCoordinate;
}