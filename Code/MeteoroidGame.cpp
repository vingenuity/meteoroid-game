#include "MeteoroidGame.hpp"

#include <Code/Graphics/MeshComponent.hpp>
#include <Code/AssertionError.hpp>
#include <Code/CameraComponent.hpp>
#include <Code/Entity.hpp>

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
MeteoroidGame::~MeteoroidGame()
{
	//Entities MUST be cleaned up first! (they want to mark components as dead for general cleanup)
	for( unsigned int i = 0; i < m_entities.size(); ++i )
	{
		delete m_entities[ i ];
	}
	m_entities.clear();


	//Afterwards, the systems may clean up their components
	m_worldPhysicsSystem->OnDestruction();
	delete m_worldPhysicsSystem;

	m_worldRenderingSystem->OnDestruction();
	delete m_worldRenderingSystem;

	m_debugUIRenderingSystem->OnDestruction();
	delete m_debugUIRenderingSystem;
}



#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
VIRTUAL void MeteoroidGame::DoBeforeFirstFrame( unsigned int windowWidth, unsigned int windowHeight )
{
	Renderer* renderer = Renderer::GetRenderer();

	m_windowDimensions.x = windowWidth;
	m_windowDimensions.y = windowHeight;

	StartupGameSystems();

	m_cameraman = new Entity();
	m_entities.push_back( m_cameraman );
	CameraComponent* m_gameCam = new CameraComponent( m_cameraman,	0.0, m_windowDimensions.x,
																	0.0, m_windowDimensions.y,
																	0.0, 1.0 );
	m_worldRenderingSystem->SetActiveCamera( m_gameCam );


	//Ship Creation
	Entity* playerShip = new Entity();
	playerShip->position.x = 400.f;
	playerShip->position.y = 400.f;
	m_entities.push_back( playerShip );

	MeshComponent* m_shipMesh = new MeshComponent( playerShip );
	playerShip->attachedComponents.push_back( m_shipMesh );
	m_worldRenderingSystem->AddMeshComponent( m_shipMesh );

	m_shipMesh->material = renderer->CreateOrGetNewMaterial( L"ShipMaterial" );
	m_shipMesh->material->SetShaderProgram( ShaderProgram::CreateOrGetShaderProgram( "Data/Shaders/BasicNoTexture.110.vertex.glsl", "Data/Shaders/BasicNoTexture.110.fragment.glsl" ) );
	m_shipMesh->material->SetModelMatrixUniform( "u_modelMatrix" );
	m_shipMesh->material->SetViewMatrixUniform( "u_viewMatrix" );
	m_shipMesh->material->SetProjectionMatrixUniform( "u_projectionMatrix" );

	static const unsigned int NUM_SHIP_VERTICES = 5;
	static const float DIST_FROM_CENTER_TO_ENGINE_X = 2.5f;
	static const float DIST_FROM_CENTER_TO_ENGINE_Y = 2.5f;
	static const float DIST_FROM_CENTER_TO_FRONT = 10.f;
	static const float DIST_FROM_CENTER_TO_WINGTIP_X = 7.5f;
	static const float DIST_FROM_CENTER_TO_WINGTIP_Y = 7.5f;
	static const Color SHIP_COLOR( 255, 255, 255, 255 );
	Simple2DVertex* boxVertexArray = new Simple2DVertex[ NUM_SHIP_VERTICES ];
	boxVertexArray[ 0] = Simple2DVertex( -DIST_FROM_CENTER_TO_ENGINE_Y	, -DIST_FROM_CENTER_TO_ENGINE_X		, SHIP_COLOR );
	boxVertexArray[ 1] = Simple2DVertex( -DIST_FROM_CENTER_TO_WINGTIP_Y	, -DIST_FROM_CENTER_TO_WINGTIP_X	, SHIP_COLOR );
	boxVertexArray[ 2] = Simple2DVertex(  DIST_FROM_CENTER_TO_FRONT		, 0.f								, SHIP_COLOR );
	boxVertexArray[ 3] = Simple2DVertex( -DIST_FROM_CENTER_TO_WINGTIP_Y	,  DIST_FROM_CENTER_TO_WINGTIP_X	, SHIP_COLOR );
	boxVertexArray[ 4] = Simple2DVertex( -DIST_FROM_CENTER_TO_ENGINE_Y	,  DIST_FROM_CENTER_TO_ENGINE_X		, SHIP_COLOR );

	VertexData& shipVertData = m_shipMesh->vertexData;
	shipVertData.data = &boxVertexArray[0];
	shipVertData.vertexSizeBytes = sizeof( Simple2DVertex );
	shipVertData.numberOfVertices = NUM_SHIP_VERTICES;
	shipVertData.attributes.push_back( VertexAttribute( Renderer::LOCATION_Vertex, 2, Renderer::TYPE_FLOAT, false, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, position.x ) ) );
	shipVertData.attributes.push_back( VertexAttribute( Renderer::LOCATION_Color, 4, Renderer::TYPE_UNSIGNED_BYTE, true, sizeof( Simple2DVertex ), offsetof( Simple2DVertex, color.r ) ) );
	shipVertData.shape = Renderer::LINE_LOOP;

	renderer->GenerateBuffer( 1, &shipVertData.bufferID );
	renderer->BufferVertexData( &shipVertData );
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoUpdate( float deltaSeconds )
{
	m_worldPhysicsSystem->OnUpdate( deltaSeconds );
	m_worldRenderingSystem->OnUpdate( deltaSeconds );
	m_debugUIRenderingSystem->OnUpdate( deltaSeconds );
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoRender() const
{
	m_worldPhysicsSystem->OnRender();
	m_worldRenderingSystem->OnRender();
	m_debugUIRenderingSystem->OnRender();
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoAtEndOfFrame()
{
	m_worldPhysicsSystem->OnEndFrame();
	m_worldRenderingSystem->OnEndFrame();
	m_debugUIRenderingSystem->OnEndFrame();
}
#pragma endregion //Lifecycle



//-----------------------------------------------------------------------------------------------
void MeteoroidGame::StartupGameSystems()
{
	m_worldPhysicsSystem = new TerrestrialPhysicsSystem( FloatVector3( 0.f, 0.f, 0.f ) );
	m_worldPhysicsSystem->OnAttachment( nullptr );

	m_worldRenderingSystem = new PerspectiveRenderingSystem( 45.0, 1280.0/720.0, 0.1, 1000 );
	m_worldRenderingSystem->OnAttachment( nullptr );

	m_debugUIRenderingSystem = new DebugDrawingSystem2D( 0.f, 1280.f, 0.f, 720.f );
	m_debugUIRenderingSystem->OnAttachment( nullptr );
}
