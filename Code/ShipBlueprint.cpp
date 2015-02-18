#include "ShipBlueprint.hpp"

#include <Code/Audio/AudioInterface.hpp>
#include <Code/Graphics/CachingShaderLoader.hpp>
#include <Code/Graphics/Material.hpp>
#include <Code/Graphics/MeshComponent.hpp>
#include <Code/PhysicsComponent.hpp>

#include "CollisionSystem2D.hpp"
#include "EntityTypes.h"
#include "GameInputComponent.hpp"
#include "MeteoroidGame.hpp"
#include "WarpComponent.hpp"
#include "WeaponComponent.hpp"


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
ShipBlueprint::ShipBlueprint()
	: m_colliderCenter( 0.f, 0.f )
	, m_colliderRadius( 8.f )
	, m_vertices( NUM_SHIP_VERTICES, sizeof( Simple2DVertex ) )
{
	BuildShipVertexData();

	m_material = RendererInterface::CreateOrGetNewMaterial( L"ShipMaterial" );
	CachingShaderLoader* shaderLoader = RendererInterface::GetShaderLoader();
	ShaderPipeline* basicPipeline = nullptr;
	if( shaderLoader->SupportsLanguage( LANGUAGE_GLSL ) )
		basicPipeline = shaderLoader->CreateOrGetShaderProgramFromFiles( "Shaders/BasicNoTexture.110.vertex.glsl", "Shaders/BasicNoTexture.110.fragment.glsl" );
	else
		basicPipeline = shaderLoader->CreateOrGetShaderProgramFromFiles( "Shaders/BasicNoTexture.vertex.cg", "Shaders/BasicNoTexture.fragment.cg" );
	m_material->SetShaderPipeline( basicPipeline );
}

//-----------------------------------------------------------------------------------------------
ShipBlueprint::~ShipBlueprint()
{

}

//-----------------------------------------------------------------------------------------------
void ShipBlueprint::BuildEntityIntoGame( Entity& out_entity, const MeteoroidGame* game,
										const FloatVector2& atPosition )
{
	out_entity.typeID = TYPEID_Ship;
	out_entity.position.x = atPosition.x;
	out_entity.position.y = atPosition.y;

	MeshComponent* shipMesh = game->m_worldRenderingSystem->AcquireMeshComponent();
	shipMesh->vertexDataIsFlyweight = true;
	shipMesh->vertexData = &m_vertices;
	shipMesh->material = m_material;
	out_entity.AttachComponent( shipMesh );

	PhysicsComponent* shipPhysics = game->m_physicsSystem->AcquireComponent();
	shipPhysics->percentAcceleratedByGravity = 0.f;
	out_entity.AttachComponent( shipPhysics );

	CollisionComponent2D* shipCollider = game->m_collisionSystem->AcquireComponent();
	shipCollider->group = SHIP_COLLISION_GROUP;
	shipCollider->SetColliderToCircle( m_colliderCenter, m_colliderRadius );
	out_entity.AttachComponent( shipCollider );

	GameInputComponent* playerInput = game->m_gameInputSystem->AcquireComponent();
	playerInput->playerID = 0;
	out_entity.AttachComponent( playerInput );

	WarpComponent* warpEngine = game->m_warpSystem->AcquireComponent();
	warpEngine->secondsSinceLastWarp = warpEngine->secondsNeededBetweenWarps = 2.f;
	out_entity.AttachComponent( warpEngine );

	WeaponComponent* missilePort = game->m_weaponSystem->AcquireComponent();
	missilePort->firingSound = AudioInterface::GetOrLoadSound( "Audio/laser.wav" );
	missilePort->minSecondsBetweenShots = 0.75f;
	out_entity.AttachComponent( missilePort );

	ScoringComponent* shipScore = game->m_scoringSystem->AcquireComponent();
	shipScore->pointValue = 1000;
	out_entity.AttachComponent( shipScore );
}



#pragma region Private Functions
//-----------------------------------------------------------------------------------------------
void ShipBlueprint::BuildShipVertexData()
{
	static const float DIST_FROM_CENTER_TO_ENGINE_X = 2.5f;
	static const float DIST_FROM_CENTER_TO_ENGINE_Y = 2.5f;
	static const float DIST_FROM_CENTER_TO_FRONT = 10.f;
	static const float DIST_FROM_CENTER_TO_WINGTIP_X = 7.5f;
	static const float DIST_FROM_CENTER_TO_WINGTIP_Y = 7.5f;

	static const Color SHIP_COLOR( 154, 196, 237, 255 );


	Simple2DVertex* shipVertexArray = reinterpret_cast< Simple2DVertex* >( m_vertices.data );
	shipVertexArray[ 0] = Simple2DVertex( -DIST_FROM_CENTER_TO_ENGINE_Y	, -DIST_FROM_CENTER_TO_ENGINE_X		, SHIP_COLOR );
	shipVertexArray[ 1] = Simple2DVertex( -DIST_FROM_CENTER_TO_WINGTIP_Y	, -DIST_FROM_CENTER_TO_WINGTIP_X	, SHIP_COLOR );
	shipVertexArray[ 2] = Simple2DVertex(  DIST_FROM_CENTER_TO_FRONT		, 0.f								, SHIP_COLOR );
	shipVertexArray[ 3] = Simple2DVertex( -DIST_FROM_CENTER_TO_WINGTIP_Y	,  DIST_FROM_CENTER_TO_WINGTIP_X	, SHIP_COLOR );
	shipVertexArray[ 4] = Simple2DVertex( -DIST_FROM_CENTER_TO_ENGINE_Y	,  DIST_FROM_CENTER_TO_ENGINE_X		, SHIP_COLOR );

	m_vertices.data = &shipVertexArray[0];
	m_vertices.vertexSizeBytes = sizeof( Simple2DVertex );
	m_vertices.numberOfVertices = NUM_SHIP_VERTICES;
	m_vertices.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Vertex, 2, RendererInterface::TYPE_FLOAT, false, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, position.x ) ) );
	m_vertices.attributes.push_back( VertexAttribute( RendererInterface::DEFAULT_NAME_Color, 4, RendererInterface::TYPE_UNSIGNED_BYTE, true, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, color.r ) ) );
	m_vertices.shape = RendererInterface::LINE_LOOP;

	RendererInterface::GenerateBuffer( 1, &m_vertices.bufferID );
	RendererInterface::BufferVertexData( &m_vertices );
}
#pragma endregion //Private Functions
