#include "MeteoroidBlueprint.hpp"

#include <Code/Graphics/MeshComponent.hpp>
#include <Code/PhysicsComponent.hpp>

#include "CollisionComponent2D.hpp"
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

	MeshComponent* meteorMesh = new MeshComponent( &out_entity );
	meteorMesh->vertexData = BuildMeteoroidVertexData( hint_meteorSize );
	meteorMesh->material = m_material;
	m_game->m_worldRenderingSystem->AddMeshComponent( meteorMesh );

	PhysicsComponent* meteorPhysics = new PhysicsComponent( &out_entity );
	meteorPhysics->percentAcceleratedByGravity = 0.f;
	m_game->m_worldPhysicsSystem->AddPhysicsComponent( meteorPhysics );

	//We want the collider radius to be in between the possible mins and maxes (it should feel better that way)
	float colliderRadius = s_meteoroidMinRadiuses[ hint_meteorSize ] + ( 0.5f * s_meteoroidSizeDeltas[ hint_meteorSize ] );
	CollisionComponent2D* meteorCollider = new CollisionComponent2D( &out_entity, m_colliderCenter, colliderRadius );
	meteorCollider->group = METEOROID_COLLISION_GROUP;
	m_game->m_worldCollisionSystem->AddCollisionComponent( meteorCollider );

	FracturingComponent* meteorFracture = new FracturingComponent( &out_entity );
	meteorFracture->fracturesRemaining = hint_meteorSize;
	m_game->m_cleanupSystem->AddComponent( meteorFracture );
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
	static const Color METEOROID_COLOR( 255, 255, 255, 255 );

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
	out_vertData->attributes.push_back( VertexAttribute( Renderer::LOCATION_Vertex, 2, Renderer::TYPE_FLOAT, false, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, position.x ) ) );
	out_vertData->attributes.push_back( VertexAttribute( Renderer::LOCATION_Color,	4, Renderer::TYPE_UNSIGNED_BYTE, true, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, color.r ) ) );
	out_vertData->shape = Renderer::LINE_LOOP;

	Renderer* renderer = Renderer::GetRenderer();
	renderer->GenerateBuffer( 1, &out_vertData->bufferID );
	renderer->BufferVertexData( out_vertData );

	return out_vertData;
}
#pragma endregion //Private Functions
