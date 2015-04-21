#include "MeteoroidBlueprint.hpp"

#include <Code/Graphics/CachingShaderLoader.hpp>
#include <Code/Graphics/Material.hpp>
#include <Code/Graphics/MeshComponent.hpp>
#include <Code/PhysicsComponent.hpp>

#include "CollisionSystem2D.hpp"
#include "EntityTypes.h"
#include "FracturingComponent.hpp"
#include "MeteoroidGame.hpp"


//-----------------------------------------------------------------------------------------------
STATIC const float MeteoroidBlueprint::MAX_METEOROID_SPEED = 30.f;
STATIC const float MeteoroidBlueprint::MAX_METEOROID_ANGULAR_SPEED = 45.f;
STATIC const float s_meteoroidMinRadiuses[ MeteoroidBlueprint::NUM_METEOROID_SIZES ] = { 10.f, 25.f, 35.f };
STATIC const float s_meteoroidSizeDeltas [ MeteoroidBlueprint::NUM_METEOROID_SIZES ] = { 10.f, 12.f, 15.f };



//-----------------------------------------------------------------------------------------------
MeteoroidBlueprint::MeteoroidBlueprint( MeteoroidGame* const game )
	: m_game( game )
	, m_colliderCenter( 0.f, 0.f )
{
	m_material = RendererInterface::CreateOrGetNewMaterial( L"MeteoroidMaterial" );
	CachingShaderLoader* shaderLoader = RendererInterface::GetShaderLoader();
	ShaderPipeline* basicPipeline = shaderLoader->CreateOrGetPipelineFromVSL( "Shaders/BasicNoTexture.vsl" );
	m_material->SetShaderPipeline( basicPipeline );
	m_material->SetLineWidth( 2.f );

	m_vertices[ 0 ] = BuildMeteoroidVertexData( 0 );
	m_vertices[ 1 ] = BuildMeteoroidVertexData( 1 );
	m_vertices[ 2 ] = BuildMeteoroidVertexData( 2 );
}

//-----------------------------------------------------------------------------------------------
MeteoroidBlueprint::~MeteoroidBlueprint()
{
	delete m_vertices[ 0 ];
	delete m_vertices[ 1 ];
	delete m_vertices[ 2 ];
}

//-----------------------------------------------------------------------------------------------
void MeteoroidBlueprint::BuildEntity( Entity& out_entity )
{
	out_entity.typeID = TYPEID_Meteoroid;
	out_entity.blueprint = this;

	out_entity.position.x = hint_spawnPosition.x;
	out_entity.position.y = hint_spawnPosition.y;

	float initialVelocityAngleRadians = GetRandomFloatBetweenZeroandOne() * 2.f * PI;
	float initialVelocityMagnitude = GetRandomFloatBetweenZeroandOne() * MAX_METEOROID_SPEED;
	FloatVector2 initialVelocity = ConvertAngleToUnitCirclePosition( initialVelocityAngleRadians ) * initialVelocityMagnitude;
	out_entity.velocity.x = initialVelocity.x;
	out_entity.velocity.y = initialVelocity.y;

	//The multiplication and subtraction get us on the range -1.0 to 1.0
	out_entity.angularVelocity.yawDegreesAboutZ = ( GetRandomFloatBetweenZeroandOne() * 2.f - 1.f ) * MAX_METEOROID_ANGULAR_SPEED;

	MeshComponent* meteorMesh = m_game->m_worldRenderingSystem->AcquireMeshComponent();
	meteorMesh->vertexDataIsFlyweight = true;
	meteorMesh->vertexData = m_vertices[ hint_meteorSize ];
	meteorMesh->material = m_material;
	out_entity.AttachComponent( meteorMesh );

	PhysicsComponent* meteorPhysics = m_game->m_physicsSystem->AcquireComponent();
	meteorPhysics->percentAcceleratedByGravity = 0.f;
	out_entity.AttachComponent( meteorPhysics );

	//We want the collider radius to be in between the possible mins and maxes (it should feel better that way)
	float colliderRadius = s_meteoroidMinRadiuses[ hint_meteorSize ] + ( 0.5f * s_meteoroidSizeDeltas[ hint_meteorSize ] );
	CollisionComponent2D* meteorCollider = m_game->m_collisionSystem->AcquireComponent();
	meteorCollider->group = METEOROID_COLLISION_GROUP;
	meteorCollider->SetColliderToCircle( m_colliderCenter, colliderRadius );
	out_entity.AttachComponent( meteorCollider );

	FracturingComponent* meteorFracture = m_game->m_fracturingSystem->AcquireComponent();
	meteorFracture->fracturesRemaining = hint_meteorSize;
	out_entity.AttachComponent( meteorFracture );

	ScoringComponent* meteorScore = m_game->m_scoringSystem->AcquireComponent();
	meteorScore->pointValue = 100;
	out_entity.AttachComponent( meteorScore );
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
VertexData* MeteoroidBlueprint::BuildMeteoroidVertexData( unsigned int meteorSize )
{
	static const unsigned int NUM_METEOR_VERTICES = 12;
	static const Color METEOROID_COLOR( 230, 230, 135, 255 );

	VertexData* out_vertData = new VertexData( NUM_METEOR_VERTICES, sizeof( Simple2DVertex ) );
	Simple2DVertex* meteoroidVertexArray = reinterpret_cast< Simple2DVertex* >( out_vertData->data );
	float radiansRotatedPerVertex =  ( 2.f * PI ) / NUM_METEOR_VERTICES;
	for( unsigned int i = 0; i < NUM_METEOR_VERTICES; ++i )
	{
		float meteorRadiusAtThisVert = s_meteoroidMinRadiuses[ meteorSize ] + 
			( GetRandomFloatBetweenZeroandOne() * s_meteoroidSizeDeltas[ meteorSize ] );
		meteoroidVertexArray[ i ] = Simple2DVertex( cos( radiansRotatedPerVertex * i ) * meteorRadiusAtThisVert,  
													sin( radiansRotatedPerVertex * i ) * meteorRadiusAtThisVert, 
													METEOROID_COLOR );
	}

	out_vertData->vertexSizeBytes = sizeof( Simple2DVertex );
	out_vertData->numberOfVertices = NUM_METEOR_VERTICES;
	out_vertData->attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Vertex, 2, RendererInterface::TYPE_FLOAT, false, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, position.x ) ) );
	out_vertData->attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Color,	4, RendererInterface::TYPE_UNSIGNED_BYTE, true, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, color.r ) ) );
	out_vertData->shape = RendererInterface::LINE_LOOP;

	RendererInterface::GenerateBuffer( 1, &out_vertData->bufferID );
	RendererInterface::BufferVertexData( out_vertData );

	return out_vertData;
}
#pragma endregion //Private Functions
