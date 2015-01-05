#include "MeteoroidGame.hpp"

#include <Code/Graphics/Renderer.hpp>
#include <Code/AssertionError.hpp>
#include <Code/CameraComponent.hpp>
#include <Code/Entity.hpp>

#include "EntityTypes.h"
#include "FracturingComponent.hpp"
#include "GameEvents.hpp"
#include "MeteoroidBlueprint.hpp"
#include "ShipBlueprint.hpp"


//-----------------------------------------------------------------------------------------------
MeteoroidGame g_game; //This initializes the game and the game interface for the engine simultaneously.

static const FloatVector2 SHIP_SPAWN_POSITION( 640.f, 360.f );


#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
VIRTUAL void MeteoroidGame::DoBeforeFirstFrame( unsigned int windowWidth, unsigned int windowHeight )
{
	m_windowDimensions.x = windowWidth;
	m_windowDimensions.y = windowHeight;
	
	EventCourier::SubscribeForEvent( EVENT_Collision, EventObserver::GenerateFromOneArgFunction< MeteoroidGame, &MeteoroidGame::OnCollisionEvent >( this ) );

	StartupGameSystems();

	m_cameraman = new Entity();
	m_entities.push_back( m_cameraman );
	CameraComponent* m_gameCam = new CameraComponent( m_cameraman,	0.0, m_windowDimensions.x,
																	0.0, m_windowDimensions.y,
																	-1.0, 1.0 );
	m_worldRenderingSystem->SetActiveCamera( m_gameCam );

	//Initialize Blueprints
	m_meteoroidBlueprint = new MeteoroidBlueprint( this );
	m_shipBlueprint = new ShipBlueprint();

	//Ship Creation
	Entity* playerShip = new Entity();
	m_shipBlueprint->BuildEntityIntoGame( *playerShip, this, SHIP_SPAWN_POSITION );
	m_entities.push_back( playerShip );

	SpawnInitialMeteoroids();
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoUpdate( float deltaSeconds )
{
	m_gameInputSystem->OnUpdate( deltaSeconds );
	m_worldPhysicsSystem->OnUpdate( deltaSeconds );
	m_worldCollisionSystem->OnUpdate( deltaSeconds );
	m_worldRenderingSystem->OnUpdate( deltaSeconds );
	m_debugUIRenderingSystem->OnUpdate( deltaSeconds );
	m_warpSystem->OnUpdate( deltaSeconds );
	m_cleanupSystem->OnUpdate( deltaSeconds );
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoRender() const
{
	Renderer* renderer = Renderer::GetRenderer();

	renderer->ClearColorBuffer();
	renderer->ClearDepthBuffer();

	m_gameInputSystem->OnRender();
	m_worldPhysicsSystem->OnRender();
	m_worldCollisionSystem->OnRender();
	m_worldRenderingSystem->OnRender();
	m_debugUIRenderingSystem->OnRender();
	m_warpSystem->OnRender();
	m_cleanupSystem->OnRender();
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoAtEndOfFrame()
{
	for( unsigned int i = 0; i < m_entities.size(); ++i )
	{
		if( m_entities[ i ]->markedForDeletion )
		{
			delete m_entities[ i ];
			m_entities.erase( m_entities.begin() + i );
			--i;
		}
	}

	m_gameInputSystem->OnEndFrame();
	m_worldPhysicsSystem->OnEndFrame();
	m_worldCollisionSystem->OnEndFrame();
	m_worldRenderingSystem->OnEndFrame();
	m_debugUIRenderingSystem->OnEndFrame();
	m_warpSystem->OnEndFrame();
	m_cleanupSystem->OnEndFrame();
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoBeforeEngineDestruction()
{
	//Entities MUST be cleaned up first! (they want to mark components as dead for general cleanup)
	for( unsigned int i = 0; i < m_entities.size(); ++i )
	{
		delete m_entities[ i ];
	}
	m_entities.clear();


	//Afterwards, the systems may clean up their components
	m_cleanupSystem->OnDestruction();
	delete m_cleanupSystem;

	m_warpSystem->OnDestruction();
	delete m_warpSystem;

	m_debugUIRenderingSystem->OnDestruction();
	delete m_debugUIRenderingSystem;

	m_gameInputSystem->OnDestruction();
	delete m_gameInputSystem;

	m_worldCollisionSystem->OnDestruction();
	delete m_worldCollisionSystem;

	m_worldPhysicsSystem->OnDestruction();
	delete m_worldPhysicsSystem;

	m_worldRenderingSystem->OnDestruction();
	delete m_worldRenderingSystem;

	delete m_meteoroidBlueprint;
	delete m_shipBlueprint;
}
#pragma endregion //Lifecycle



#pragma region Events
//-----------------------------------------------------------------------------------------------
void MeteoroidGame::OnCollisionEvent( EventDataBundle& eventData )
{
	//Warn if strange event name received?
	Entity* firstEntity;
	eventData.GetParameterOrDie( STRING_1stEntity, firstEntity );
	HandleEntityDestructionOrReuse( firstEntity );

	Entity* secondEntity;
	eventData.GetParameterOrDie( STRING_2ndEntity, secondEntity );
	HandleEntityDestructionOrReuse( secondEntity );
}
#pragma endregion //Events



//-----------------------------------------------------------------------------------------------
void MeteoroidGame::HandleEntityDestructionOrReuse( Entity*& entity )
{
	EventDataBundle eventData;
	eventData.SetParameter( STRING_1stEntity, entity );
	EventCourier::SendEvent( EVENT_EntityDestruction, eventData );

	switch( entity->typeID )
	{
	case TYPEID_Ship:
		entity->position.x = SHIP_SPAWN_POSITION.x;
		entity->position.y = SHIP_SPAWN_POSITION.y;
		entity->velocity = FloatVector3( 0.f, 0.f, 0.f );
		entity->acceleration = FloatVector3( 0.f, 0.f, 0.f );
		break;
	case TYPEID_Meteoroid:
		{
			FracturingComponent* fracture = entity->FindAttachedComponentOfType< FracturingComponent >();
			if( ( fracture != nullptr ) && ( fracture->fracturesRemaining != 0 ) )
			{
				--fracture->fracturesRemaining;

				Entity* spawnedMeteor = nullptr;
				FloatVector2 spawnPosition( entity->position.x, entity->position.y );

				for( unsigned int i = 0; i < 4; ++i )
				{
					spawnedMeteor = new Entity();
					m_meteoroidBlueprint->hint_spawnPosition = spawnPosition;
					m_meteoroidBlueprint->hint_meteorSize = fracture->fracturesRemaining;
					m_meteoroidBlueprint->BuildEntity( *spawnedMeteor );
					m_entities.push_back( spawnedMeteor );
				}
			}
			entity->markedForDeletion = true;
		}
		break;
	default:
		entity->markedForDeletion = true;
		break;
	}
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::SpawnInitialMeteoroids()
{
	Entity* spawnedMeteor = nullptr;
	FloatVector2 spawnPosition;

	for( unsigned int i = 0; i < 15; ++i )
	{
		spawnedMeteor = new Entity();
		spawnPosition.x = GetRandomFloatBetweenZeroandOne() * m_windowDimensions.x;
		spawnPosition.y = GetRandomFloatBetweenZeroandOne() * m_windowDimensions.y;
		m_meteoroidBlueprint->hint_spawnPosition = spawnPosition;
		m_meteoroidBlueprint->hint_meteorSize = static_cast< unsigned int >( GetRandomIntBetween( 0, MeteoroidBlueprint::NUM_METEOROID_SIZES ) );
		m_meteoroidBlueprint->BuildEntity( *spawnedMeteor );
		m_entities.push_back( spawnedMeteor );
	}
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::StartupGameSystems()
{
	m_cleanupSystem = new CleanupSystem();
	m_cleanupSystem->OnAttachment( nullptr );

	m_gameInputSystem = new GameInputSystem();
	m_gameInputSystem->OnAttachment( nullptr );

	m_debugUIRenderingSystem = new DebugDrawingSystem2D( 0.f, static_cast<float>( m_windowDimensions.x ), 
		0.f, static_cast<float>( m_windowDimensions.y ) );
	m_debugUIRenderingSystem->OnAttachment( nullptr );

	m_warpSystem = new WarpSystem( FloatVector2( (float)m_windowDimensions.x, (float)m_windowDimensions.y ) );
	m_warpSystem->OnAttachment( nullptr );

	m_worldCollisionSystem = new CollisionSystem2D();
	m_worldCollisionSystem->OnAttachment( nullptr );

	m_worldPhysicsSystem = new OuterSpacePhysicsSystem();
	m_worldPhysicsSystem->OnAttachment( nullptr );

	m_worldRenderingSystem = new PerspectiveRenderingSystem( 45.0, (double)m_windowDimensions.x/m_windowDimensions.y, 0.1, 1000 );
	m_worldRenderingSystem->OnAttachment( nullptr );
}
