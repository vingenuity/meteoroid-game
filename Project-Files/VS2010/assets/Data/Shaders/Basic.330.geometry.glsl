#version 330
//Geometry Shader - Shader language 3.30, for OpenGL 3.3

//INPUTS:
layout( triangles ) in;
in vec4 screenPosition[];
in vec4 worldPosition[];
in vec4 surfaceColor[];
in vec2 textureCoordinates[];

//OUTPUTS:
layout( triangle_strip, max_vertices = 3 ) out;
out vec4 screenPos;
out vec4 worldPos;
out vec4 surfaceColors;
out vec2 textureCoords;


//-----------------------------------------------------------------------------------------------
void main()
{
	for( int i = 0; i < 3; ++i )
	{
		gl_Position			= gl_in[ i ].gl_Position;
		screenPos			= screenPosition[ i ];
		worldPos			= worldPosition[ i ];
		surfaceColors		= surfaceColor[ i ];
		textureCoords		= textureCoordinates[ i ];
		EmitVertex();
	}
	EndPrimitive();
}
