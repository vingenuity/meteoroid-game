#include "MissileBlueprint.hpp"

#include <Code/Graphics/MeshComponent.hpp>
#include <Code/PhysicsComponent.hpp>

#include "CollisionComponent2D.hpp"
#include "EntityTypes.h"
#include "MeteoroidGame.hpp"


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

	Renderer* renderer = Renderer::GetRenderer();
	m_material = renderer->CreateOrGetNewMaterial( L"ShipMaterial" );
	m_material->SetShaderProgram( ShaderProgram::CreateOrGetShaderProgram( "Data/Shaders/BasicNoTexture.110.vertex.glsl", "Data/Shaders/BasicNoTexture.110.fragment.glsl" ) );
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

	MeshComponent* shipMesh = new MeshComponent( &out_entity );
	shipMesh->vertexDataIsFlyweight = true;
	shipMesh->vertexData = &m_vertices;
	shipMesh->material = m_material;
	m_game->m_worldRenderingSystem->AddMeshComponent( shipMesh );

	PhysicsComponent* shipPhysics = new PhysicsComponent( &out_entity );
	shipPhysics->percentAcceleratedByGravity = 0.f;
	m_game->m_worldPhysicsSystem->AddPhysicsComponent( shipPhysics );

	CollisionComponent2D* shipCollider = new CollisionComponent2D( &out_entity, m_colliderCenter, m_colliderRadius );
	shipCollider->group = 1;
	m_game->m_worldCollisionSystem->AddCollisionComponent( shipCollider );
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
	m_vertices.attributes.push_back( VertexAttribute( Renderer::LOCATION_Vertex, 2, Renderer::TYPE_FLOAT, false, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, position.x ) ) );
	m_vertices.attributes.push_back( VertexAttribute( Renderer::LOCATION_Color, 4, Renderer::TYPE_UNSIGNED_BYTE, true, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, color.r ) ) );
	m_vertices.shape = Renderer::LINES;

	Renderer* renderer = Renderer::GetRenderer();
	renderer->GenerateBuffer( 1, &m_vertices.bufferID );
	renderer->BufferVertexData( &m_vertices );
}
