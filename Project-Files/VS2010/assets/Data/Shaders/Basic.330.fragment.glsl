#version 330
//Fragment Shader - Shader language 3.30, for OpenGL 3.3

//INPUTS:
in vec4 screenPosition;
in vec4 worldPosition;
in vec4 surfaceColor;
in vec2 textureCoordinates;

uniform sampler2D u_diffuseMap;


//OUTPUTS:
out vec4 o_pixelColor;

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void main()
{
	vec4 diffuseTexel = texture2D( u_diffuseMap, textureCoordinates );

	o_pixelColor = surfaceColor * diffuseTexel;
}