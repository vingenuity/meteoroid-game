precision mediump float;

//-----------------------------------------------------------------------------------------------
//	BLOOM CALCULATION
//-----------------------------------------------------------------------------------------------
/* The PS3's graphics hardware cannot index into non-texture arrays.
 * Thus, we are going to calculate Pascal's numbers every time we need them. Ick.
 * This code uses the identity: C(n,k+1) = C(n,k) * (n-k) / (k+1).
 * Thanks, http://stackoverflow.com/questions/15580291/how-to-efficiently-calculate-a-row-in-pascals-triangle! */
float CalculatePascalNumberOnRow4( int zeroIndexInRow )
{
	//assert( zeroIndexInRow < rowNumber );

	int pascalNumber = 1;
	for( int index = 0; index < 4; ++index )
	{
		pascalNumber = pascalNumber * ( 4 - index ) / ( index + 1 );
	}
	return float( pascalNumber );
}

//-----------------------------------------------------------------------------------------------
const float BLOOM_DENOMINATOR = 24.0;
const int MAX_TEXEL_STEP = 2;
const float TEXTURE_COORD_STEP_SIZE = 0.003;

vec4 CalculateBloomedTexelColorAt( vec2 textureCoordinate, sampler2D textureMap )
{
	vec4 additiveTexelColor = vec4( 0.0, 0.0, 0.0, 0.0 );

	for( int xStep = -MAX_TEXEL_STEP; xStep <= MAX_TEXEL_STEP; ++xStep )
	{
		float xBloomNumerator = CalculatePascalNumberOnRow4( xStep + MAX_TEXEL_STEP );
		for( int yStep = -MAX_TEXEL_STEP; yStep <= MAX_TEXEL_STEP; ++yStep )
		{
			float bloomMultiplier = ( xBloomNumerator * CalculatePascalNumberOnRow4( yStep + MAX_TEXEL_STEP ) ) / BLOOM_DENOMINATOR;
			vec2 offsetTextureCoordinate = textureCoordinate + ( vec2( xStep , yStep ) * TEXTURE_COORD_STEP_SIZE );

			additiveTexelColor += bloomMultiplier * texture2D( textureMap, offsetTextureCoordinate );
		}
	}
	return additiveTexelColor;
}



//-----------------------------------------------------------------------------------------------
//	MAIN
//-----------------------------------------------------------------------------------------------
//INPUTS:
varying vec4 screenPosition;
varying vec4 worldPosition;
varying vec4 surfaceColor;
varying vec2 textureCoordinates;

//UNIFORMS:
uniform sampler2D u_diffuseMap;

//OUTPUTS:
//vec4 gl_FragColor;

void main()
{
	gl_FragColor = surfaceColor * ( texture2D( u_diffuseMap, textureCoordinates ) + CalculateBloomedTexelColorAt( textureCoordinates, u_diffuseMap ) );
}