#include "MeteoroidBlueprint.hpp"

#include <Code/Graphics/MeshComponent.hpp>
#include <Code/PhysicsComponent.hpp>

#include "CollisionComponent2D.hpp"
#include "MeteoroidGame.hpp"


//-----------------------------------------------------------------------------------------------
MeteoroidBlueprint::MeteoroidBlueprint()
	: m_colliderCenter( 0.f, 0.f )
	, m_colliderRadius( 9.f )
{
	BuildMeteoroidVertexData();

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

	MeshComponent* meteorMesh = new MeshComponent( &out_entity );
	meteorMesh->vertexData = &m_vertices;
	meteorMesh->material = m_material;
	game->m_worldRenderingSystem->AddMeshComponent( meteorMesh );

	PhysicsComponent* meteorPhysics = new PhysicsComponent( &out_entity );
	meteorPhysics->percentAcceleratedByGravity = 0.f;
	game->m_worldPhysicsSystem->AddPhysicsComponent( meteorPhysics );

	CollisionComponent2D* meteorCollider = new CollisionComponent2D( &out_entity, m_colliderCenter, m_colliderRadius );
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
void MeteoroidBlueprint::BuildMeteoroidVertexData()
{
	static const float RADIUS_LARGE = 10.f;

	static const unsigned int NUM_SHIP_VERTICES = 12;
	static const Color METEOROID_COLOR( 255, 255, 255, 255 );


	Simple2DVertex* meteoroidVertexArray = new Simple2DVertex[ NUM_SHIP_VERTICES ];
	meteoroidVertexArray[ 0] = Simple2DVertex( 0.f,  RADIUS_LARGE, METEOROID_COLOR );
	meteoroidVertexArray[ 1] = Simple2DVertex( 0.f, -RADIUS_LARGE, METEOROID_COLOR );

	m_vertices.data = &meteoroidVertexArray[0];
	m_vertices.vertexSizeBytes = sizeof( Simple2DVertex );
	m_vertices.numberOfVertices = NUM_SHIP_VERTICES;
	m_vertices.attributes.push_back( VertexAttribute( Renderer::LOCATION_Vertex, 2, Renderer::TYPE_FLOAT, false, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, position.x ) ) );
	m_vertices.attributes.push_back( VertexAttribute( Renderer::LOCATION_Color, 4, Renderer::TYPE_UNSIGNED_BYTE, true, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, color.r ) ) );
	m_vertices.shape = Renderer::LINE_LOOP;

	Renderer* renderer = Renderer::GetRenderer();
	renderer->GenerateBuffer( 1, &m_vertices.bufferID );
	renderer->BufferVertexData( &m_vertices );
}
#pragma endregion //Private Functions
