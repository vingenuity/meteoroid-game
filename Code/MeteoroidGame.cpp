#include "MeteoroidGame.hpp"

#include <Code/Graphics/Renderer.hpp>
#include <Code/AssertionError.hpp>
#include <Code/CameraComponent.hpp>
#include <Code/Entity.hpp>

#include "ShipBlueprint.hpp"


//-----------------------------------------------------------------------------------------------
MeteoroidGame g_game; //This initializes the game and the game interface for the engine simultaneously.

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

	delete m_shipBlueprint;
}



#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
VIRTUAL void MeteoroidGame::DoBeforeFirstFrame( unsigned int windowWidth, unsigned int windowHeight )
{
	m_windowDimensions.x = windowWidth;
	m_windowDimensions.y = windowHeight;

	StartupGameSystems();

	m_cameraman = new Entity();
	m_entities.push_back( m_cameraman );
	CameraComponent* m_gameCam = new CameraComponent( m_cameraman,	0.0, m_windowDimensions.x,
																	0.0, m_windowDimensions.y,
																	-1.0, 1.0 );
	m_worldRenderingSystem->SetActiveCamera( m_gameCam );

	//Initialize Blueprints
	m_shipBlueprint = new ShipBlueprint();

	//Ship Creation
	static const FloatVector2 SPAWN_POSITION( 400.f, 400.f );
	Entity* playerShip = new Entity();
	m_shipBlueprint->BuildEntityIntoGame( *playerShip, this, SPAWN_POSITION );
	playerShip->velocity.y = -6.f;
	m_entities.push_back( playerShip );
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
	Renderer* renderer = Renderer::GetRenderer();

	renderer->ClearColorBuffer();
	renderer->ClearDepthBuffer();

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
	m_worldPhysicsSystem = new OuterSpacePhysicsSystem();
	m_worldPhysicsSystem->OnAttachment( nullptr );

	m_worldRenderingSystem = new PerspectiveRenderingSystem( 45.0, (double)m_windowDimensions.x/m_windowDimensions.y, 0.1, 1000 );
	m_worldRenderingSystem->OnAttachment( nullptr );

	m_debugUIRenderingSystem = new DebugDrawingSystem2D( 0.f, m_windowDimensions.x, 
														 0.f, m_windowDimensions.y );
	m_debugUIRenderingSystem->OnAttachment( nullptr );
}
