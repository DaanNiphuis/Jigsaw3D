in vec4 position;
in vec2 textureCoordinate;

out vec2 textureCoordinateVar;

void main()
{
	gl_Position = position;
	textureCoordinateVar = textureCoordinate;
}