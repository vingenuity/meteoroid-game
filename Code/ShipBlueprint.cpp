#include "ShipBlueprint.hpp"

#include <Code/Graphics/MeshComponent.hpp>
#include <Code/PhysicsComponent.hpp>

#include "CollisionComponent2D.hpp"
#include "MeteoroidGame.hpp"


//-----------------------------------------------------------------------------------------------
ShipBlueprint::ShipBlueprint()
	: m_colliderCenter( 0.f, 0.f )
	, m_colliderRadius( 8.f )
{
	BuildShipVertexData();

	Renderer* renderer = Renderer::GetRenderer();
	m_material = renderer->CreateOrGetNewMaterial( L"ShipMaterial" );
	m_material->SetShaderProgram( ShaderProgram::CreateOrGetShaderProgram( "Data/Shaders/BasicNoTexture.110.vertex.glsl", "Data/Shaders/BasicNoTexture.110.fragment.glsl" ) );
	m_material->SetModelMatrixUniform( "u_modelMatrix" );
	m_material->SetViewMatrixUniform( "u_viewMatrix" );
	m_material->SetProjectionMatrixUniform( "u_projectionMatrix" );
}

//-----------------------------------------------------------------------------------------------
ShipBlueprint::~ShipBlueprint()
{

}

//-----------------------------------------------------------------------------------------------
void ShipBlueprint::BuildEntityIntoGame( Entity& out_entity, const MeteoroidGame* game,
										const FloatVector2& atPosition )
{
	out_entity.position.x = atPosition.x;
	out_entity.position.y = atPosition.y;

	MeshComponent* shipMesh = new MeshComponent( &out_entity );
	shipMesh->vertexData = &m_vertices;
	shipMesh->material = m_material;
	game->m_worldRenderingSystem->AddMeshComponent( shipMesh );

	PhysicsComponent* shipPhysics = new PhysicsComponent( &out_entity );
	shipPhysics->percentAcceleratedByGravity = 0.f;
	game->m_worldPhysicsSystem->AddPhysicsComponent( shipPhysics );

	CollisionComponent2D* shipCollider = new CollisionComponent2D( &out_entity, m_colliderCenter, m_colliderRadius );
	game->m_worldCollisionSystem->AddCollisionComponent( shipCollider );
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
void ShipBlueprint::BuildShipVertexData()
{
	static const float DIST_FROM_CENTER_TO_ENGINE_X = 2.5f;
	static const float DIST_FROM_CENTER_TO_ENGINE_Y = 2.5f;
	static const float DIST_FROM_CENTER_TO_FRONT = 10.f;
	static const float DIST_FROM_CENTER_TO_WINGTIP_X = 7.5f;
	static const float DIST_FROM_CENTER_TO_WINGTIP_Y = 7.5f;

	static const unsigned int NUM_SHIP_VERTICES = 5;
	static const Color SHIP_COLOR( 255, 255, 255, 255 );


	Simple2DVertex* shipVertexArray = new Simple2DVertex[ NUM_SHIP_VERTICES ];
	shipVertexArray[ 0] = Simple2DVertex( -DIST_FROM_CENTER_TO_ENGINE_Y	, -DIST_FROM_CENTER_TO_ENGINE_X		, SHIP_COLOR );
	shipVertexArray[ 1] = Simple2DVertex( -DIST_FROM_CENTER_TO_WINGTIP_Y	, -DIST_FROM_CENTER_TO_WINGTIP_X	, SHIP_COLOR );
	shipVertexArray[ 2] = Simple2DVertex(  DIST_FROM_CENTER_TO_FRONT		, 0.f								, SHIP_COLOR );
	shipVertexArray[ 3] = Simple2DVertex( -DIST_FROM_CENTER_TO_WINGTIP_Y	,  DIST_FROM_CENTER_TO_WINGTIP_X	, SHIP_COLOR );
	shipVertexArray[ 4] = Simple2DVertex( -DIST_FROM_CENTER_TO_ENGINE_Y	,  DIST_FROM_CENTER_TO_ENGINE_X		, SHIP_COLOR );

	m_vertices.data = &shipVertexArray[0];
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
