#include "MeteoroidGame.hpp"

#include <Code/CameraComponent.hpp>
#include <Code/Entity.hpp>



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
	m_windowDimensions.x = windowWidth;
	m_windowDimensions.y = windowHeight;

	StartupGameSystems();

	m_cameraman = new Entity();
	m_entities.push_back( m_cameraman );
	CameraComponent* m_gameCam = new CameraComponent( m_cameraman,	0.0, m_windowDimensions.x,
																	0.0, m_windowDimensions.y,
																	0.0, 1.0 );
	m_worldRenderingSystem->SetActiveCamera( m_gameCam );
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
