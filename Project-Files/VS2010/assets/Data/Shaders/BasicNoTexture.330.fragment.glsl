#version 330
//Fragment Shader - Shader language 3.30, for OpenGL 3.3

//INPUTS:
in vec4 screenPosition;	//= position of this pixel in screen space
in vec4 worldPosition;	//= position of this pixel in world space
in vec4 surfaceColor;	//= color of the surface containing this pixel


//OUTPUTS:
out vec4 o_pixelColor; //= final color (RGBA) to report to the framebuffer (to be blended)

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void main()
{
	o_pixelColor = surfaceColor;
}