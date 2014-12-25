#include "MeteoroidBlueprint.hpp"

#include <Code/Graphics/MeshComponent.hpp>
#include <Code/PhysicsComponent.hpp>

#include "CollisionComponent2D.hpp"
#include "MeteoroidGame.hpp"


//-----------------------------------------------------------------------------------------------
STATIC const float MeteoroidBlueprint::MAX_METEOROID_SPEED = 30.f;
STATIC const float MeteoroidBlueprint::MAX_METEOROID_ANGULAR_SPEED = 45.f;
STATIC const float s_meteoroidMinRadiuses[ MeteoroidBlueprint::NUM_METEOROID_SIZES ] = { 10.f, 25.f, 35.f };
STATIC const float s_meteoroidSizeDeltas [ MeteoroidBlueprint::NUM_METEOROID_SIZES ] = { 10.f, 12.f, 15.f };



//-----------------------------------------------------------------------------------------------
MeteoroidBlueprint::MeteoroidBlueprint()
	: m_colliderCenter( 0.f, 0.f )
{
	Renderer* renderer = Renderer::GetRenderer();
	m_material = renderer->CreateOrGetNewMaterial( L"MeteoroidMaterial" );
	m_material->SetShaderProgram( ShaderProgram::CreateOrGetShaderProgram( "Data/Shaders/BasicNoTexture.110.vertex.glsl", "Data/Shaders/BasicNoTexture.110.fragment.glsl" ) );
	m_material->SetModelMatrixUniform( "u_modelMatrix" );
	m_material->SetViewMatrixUniform( "u_viewMatrix" );
	m_material->SetProjectionMatrixUniform( "u_projectionMatrix" );
}

//-----------------------------------------------------------------------------------------------
MeteoroidBlueprint::~MeteoroidBlueprint()
{

}

//-----------------------------------------------------------------------------------------------
void MeteoroidBlueprint::BuildEntityIntoGame( Entity& out_entity, const MeteoroidGame* game,
	const FloatVector2& atPosition )
{
	out_entity.position.x = atPosition.x;
	out_entity.position.y = atPosition.y;

	//The multiplications get us on the range -1.0 to 1.0
	out_entity.angularVelocity.yawDegreesAboutZ = ( GetRandomFloatBetweenZeroandOne() * 2.f - 0.5f ) * MAX_METEOROID_ANGULAR_SPEED;

	unsigned int meteorSize = static_cast< unsigned int >( GetRandomIntBetween( 0, NUM_METEOROID_SIZES ) );

	MeshComponent* meteorMesh = new MeshComponent( &out_entity );
	VertexData* meteorVertData = new VertexData();
	BuildMeteoroidVertexData( *meteorVertData, meteorSize );
	meteorMesh->vertexData = meteorVertData;
	meteorMesh->material = m_material;
	game->m_worldRenderingSystem->AddMeshComponent( meteorMesh );

	PhysicsComponent* meteorPhysics = new PhysicsComponent( &out_entity );
	meteorPhysics->percentAcceleratedByGravity = 0.f;
	game->m_worldPhysicsSystem->AddPhysicsComponent( meteorPhysics );

	//We want the collider radius to be in between the possible mins and maxes (it should feel better that way)
	float colliderRadius = s_meteoroidMinRadiuses[ meteorSize ] + ( 0.5f * s_meteoroidSizeDeltas[ meteorSize ] );
	CollisionComponent2D* meteorCollider = new CollisionComponent2D( &out_entity, m_colliderCenter, colliderRadius );
	game->m_worldCollisionSystem->AddCollisionComponent( meteorCollider );
}



#pragma region Private Functions
//-----------------------------------------------------------------------------------------------
struct Simple2DVertex
{
	FloatVector2 position;
	Color color;

	Simple2DVertex( ) { }

	Simple2DVertex( float vertexX, float vertexY,
		unsigned char colorR, unsigned char colorG, unsigned char colorB, unsigned char colorA )
		: position( vertexX, vertexY )
		, color( colorR, colorG, colorB, colorA )
	{ }

	Simple2DVertex( float vertexX, float vertexY, const Color& vertexColor )
		: position( vertexX, vertexY )
		, color( vertexColor )
	{ }

	Simple2DVertex( const FloatVector2& vertexPos, const Color& vertexColor )
		: position( vertexPos )
		, color( vertexColor )
	{ }
};



//-----------------------------------------------------------------------------------------------
void MeteoroidBlueprint::BuildMeteoroidVertexData( VertexData& out_vertData, unsigned int meteorSize )
{
	static const unsigned int NUM_METEOR_VERTICES = 12;
	static const Color METEOROID_COLOR( 255, 255, 255, 255 );

	Simple2DVertex* meteoroidVertexArray = new Simple2DVertex[ NUM_METEOR_VERTICES ];
	float radiansRotatedPerVertex =  ( 2.f * PI ) / NUM_METEOR_VERTICES;
	for( unsigned int i = 0; i < NUM_METEOR_VERTICES; ++i )
	{
		float meteorRadiusAtThisVert = s_meteoroidMinRadiuses[ meteorSize ] + 
			( GetRandomFloatBetweenZeroandOne() * s_meteoroidSizeDeltas[ meteorSize ] );
		meteoroidVertexArray[ i ] = Simple2DVertex( cos( radiansRotatedPerVertex * i ) * meteorRadiusAtThisVert,  
													sin( radiansRotatedPerVertex * i ) * meteorRadiusAtThisVert, 
													METEOROID_COLOR );
	}

	out_vertData.data = &meteoroidVertexArray[0];
	out_vertData.vertexSizeBytes = sizeof( Simple2DVertex );
	out_vertData.numberOfVertices = NUM_METEOR_VERTICES;
	out_vertData.attributes.push_back( VertexAttribute( Renderer::LOCATION_Vertex, 2, Renderer::TYPE_FLOAT, false, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, position.x ) ) );
	out_vertData.attributes.push_back( VertexAttribute( Renderer::LOCATION_Color,	4, Renderer::TYPE_UNSIGNED_BYTE, true, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, color.r ) ) );
	out_vertData.shape = Renderer::LINE_LOOP;

	Renderer* renderer = Renderer::GetRenderer();
	renderer->GenerateBuffer( 1, &out_vertData.bufferID );
	renderer->BufferVertexData( &out_vertData );
}
#pragma endregion //Private Functions
