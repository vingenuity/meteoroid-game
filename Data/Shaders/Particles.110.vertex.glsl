
//INPUTS:
attribute vec3 i_vertexWorldPosition;	//= vertex position in world space (raw, from VBO)
attribute vec4 i_vertexColor;			//= vertex Color, should be 4 floats
attribute vec2 i_textureCoordinates;	//= texture Coordinates for texture 0

uniform mat4 u_modelMatrix;  //= current model view matrix
uniform mat4 u_projectionMatrix; //= current projection matrix
uniform mat4 u_viewMatrix; //= current projection matrix

//OUTPUTS:
varying vec4 screenPosition;		//= vertex position in screen space
varying vec4 worldPosition;			//= vertex position in world space
varying vec4 surfaceColor;			//= color of the surface
varying vec2 textureCoordinates;	//= UV coordinates on the texture


//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void main()
{
	vec4 i_vertexWorldPosition4 = vec4( i_vertexWorldPosition.x, i_vertexWorldPosition.y, i_vertexWorldPosition.z, 1.0 );
	screenPosition = u_projectionMatrix * u_viewMatrix * u_modelMatrix * i_vertexWorldPosition4;
	gl_Position = screenPosition;
	worldPosition = u_modelMatrix * i_vertexWorldPosition4;
	surfaceColor = i_vertexColor / 255;
	textureCoordinates = 1 / i_textureCoordinates;
}
