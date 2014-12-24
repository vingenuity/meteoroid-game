precision mediump float;

//INPUTS:
varying vec4 screenPosition;	//= position of this pixel in screen space
varying vec4 worldPosition;	//= position of this pixel in world space
varying vec4 surfaceColor;	//= color of the surface containing this pixel


//OUTPUTS:
//out vec4 o_pixelColor; //= final color (RGBA) to report to the framebuffer (to be blended)

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void main()
{
	gl_FragColor = surfaceColor;
}