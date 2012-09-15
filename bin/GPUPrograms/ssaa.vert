attribute vec4 position;
attribute vec2 textureCoordinate;

varying vec2 textureCoordinateVarying;

void main()
{
	gl_Position = position;
	textureCoordinateVarying = textureCoordinate;
}