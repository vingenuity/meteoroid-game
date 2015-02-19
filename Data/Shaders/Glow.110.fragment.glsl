precision mediump float;

const float gaussianValues[ 5 ] = float[ 5 ]( 1, 4, 6, 4, 1 );

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
	vec4 diffuseTexelSum = vec4( 0.0, 0.0, 0.0, 0.0 );
	for( int i = -2; i <= 2; ++i )
	{
		for( int j = -2; j <= 2; ++j )
		{
			diffuseTexelSum += ( gaussianValues[ i + 2 ] * gaussianValues[ j + 2 ] ) / 36 * texture2D( u_diffuseMap, textureCoordinates + vec2( i * 0.001, j * 0.001 ) );
		}
	}

	gl_FragColor = surfaceColor * diffuseTexelSum;
}