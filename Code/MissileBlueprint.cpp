#include "MissileBlueprint.hpp"

#include <Code/Graphics/MeshComponent.hpp>
#include <Code/PhysicsComponent.hpp>

#include "CollisionSystem2D.hpp"
#include "EntityTypes.h"
#include "MeteoroidGame.hpp"
#include "TimedDestructionSystem.hpp"


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
MissileBlueprint::MissileBlueprint( MeteoroidGame* const game )
	: m_colliderCenter( 0.f, 0.f )
	, m_colliderRadius( 8.f )
	, m_game( game )
	, m_vertices( NUM_MISSILE_VERTICES, sizeof( Simple2DVertex ) )
{
	BuildMissileVertexData();

	m_material = RendererInterface::CreateOrGetNewMaterial( L"ShipMaterial" );
	m_material->SetShaderProgram( ShaderProgram::CreateOrGetShaderProgram( "Shaders/BasicNoTexture.110.vertex.glsl", "Shaders/BasicNoTexture.110.fragment.glsl" ) );
	m_material->SetModelMatrixUniform( "u_modelMatrix" );
	m_material->SetViewMatrixUniform( "u_viewMatrix" );
	m_material->SetProjectionMatrixUniform( "u_projectionMatrix" );
}

//-----------------------------------------------------------------------------------------------
MissileBlueprint::~MissileBlueprint()
{

}

//-----------------------------------------------------------------------------------------------
void MissileBlueprint::BuildEntity( Entity& out_entity )
{
	out_entity.typeID = TYPEID_Bullet;

 	MeshComponent* missileMesh = m_game->m_worldRenderingSystem->AcquireMeshComponent();
 	missileMesh->vertexDataIsFlyweight = true;
 	missileMesh->vertexData = &m_vertices;
 	missileMesh->material = m_material;
	out_entity.AttachComponent( missileMesh );

	PhysicsComponent* spacePhysics = m_game->m_physicsSystem->AcquireComponent();
	spacePhysics->percentAcceleratedByGravity = 0.f;
	out_entity.AttachComponent( spacePhysics );

	CollisionComponent2D* missileCollider = m_game->m_collisionSystem->AcquireComponent();
	missileCollider->group = 1;
	missileCollider->SetColliderToCircle( m_colliderCenter, m_colliderRadius );
	out_entity.AttachComponent( missileCollider );

	TimedDestructionComponent* missileFailsafe = m_game->m_timedDestructionSystem->AcquireComponent();
	missileFailsafe->secondsLeftUntilDestruction = 1.f;
	out_entity.AttachComponent( missileFailsafe );

	ScoringComponent* missileScore = m_game->m_scoringSystem->AcquireComponent();
	missileScore->pointValue = 1;
	out_entity.AttachComponent( missileScore );
}


//-----------------------------------------------------------------------------------------------
void MissileBlueprint::BuildMissileVertexData()
{
	static const Color MISSILE_COLOR( 255, 255, 255, 255 );

	Simple2DVertex* shipVertexArray = reinterpret_cast< Simple2DVertex* >( m_vertices.data );
	shipVertexArray[ 0] = Simple2DVertex(  0.f	, 0.f	, MISSILE_COLOR );
	shipVertexArray[ 1] = Simple2DVertex( -8.f	, 0.f	, MISSILE_COLOR );
	
	m_vertices.data = &shipVertexArray[0];
	m_vertices.vertexSizeBytes = sizeof( Simple2DVertex );
	m_vertices.numberOfVertices = NUM_MISSILE_VERTICES;
	m_vertices.attributes.push_back( VertexAttribute( RendererInterface::LOCATION_Vertex, 2, RendererInterface::TYPE_FLOAT, false, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, position.x ) ) );
	m_vertices.attributes.push_back( VertexAttribute( RendererInterface::LOCATION_Color, 4, RendererInterface::TYPE_UNSIGNED_BYTE, true, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, color.r ) ) );
	m_vertices.shape = RendererInterface::LINES;

	RendererInterface::GenerateBuffer( 1, &m_vertices.bufferID );
	RendererInterface::BufferVertexData( &m_vertices );
}
