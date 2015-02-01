#version 330
//Vertex Shader - Shader language 3.30, for OpenGL 3.3

//INPUTS:
in vec3 i_vertexWorldPosition;	//= vertex position in world space (raw, from VBO)
in vec4 i_vertexColor;			//= vertex Color, should be 4 floats

uniform mat4 u_modelMatrix;  //= current model view matrix
uniform mat4 u_projectionMatrix; //= current projection matrix
uniform mat4 u_viewMatrix; //= current projection matrix

//OUTPUTS:
out vec4 screenPosition;		//= vertex position in screen space
out vec4 worldPosition;			//= vertex position in world space
out vec4 surfaceColor;			//= color of the surface


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
