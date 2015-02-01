precision mediump float;

//INPUTS:
varying vec4 screenPosition;
varying vec4 worldPosition;
varying vec4 surfaceColor;
varying vec2 textureCoordinates;

uniform sampler2D u_diffuseMap;


//OUTPUTS:
//varying vec4 o_pixelColor;

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void main()
{
	vec4 diffuseTexel = texture2D( u_diffuseMap, textureCoordinates );

	gl_FragColor = surfaceColor * diffuseTexel;
}