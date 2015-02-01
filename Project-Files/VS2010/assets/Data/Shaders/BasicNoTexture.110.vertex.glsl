
//INPUTS:
attribute vec3 i_vertexWorldPosition;
attribute vec4 i_vertexColor;

uniform mat4 u_modelMatrix;  //= current model view matrix
uniform mat4 u_projectionMatrix; //= current projection matrix
uniform mat4 u_viewMatrix; //= current projection matrix

//OUTPUTS:
varying vec4 screenPosition;
varying vec4 worldPosition;
varying vec4 surfaceColor;


//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void main()
{
	vec4 i_vertexWorldPosition4 = vec4( i_vertexWorldPosition.x, i_vertexWorldPosition.y, i_vertexWorldPosition.z, 1.0 );
	screenPosition = u_projectionMatrix * u_viewMatrix * u_modelMatrix * i_vertexWorldPosition4;
	gl_Position = screenPosition;
	worldPosition = u_modelMatrix * i_vertexWorldPosition4;
	surfaceColor = i_vertexColor;
}