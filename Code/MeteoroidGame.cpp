#include "MeteoroidGame.hpp"

#include <Code/Font/BitmapFont.hpp>
#include <Code/Graphics/Material.hpp>
#include <Code/Graphics/Renderer.hpp>
#include <Code/AssertionError.hpp>
#include <Code/CameraComponent.hpp>
#include <Code/Entity.hpp>

#include "EntityTypes.h"
#include "FracturingComponent.hpp"
#include "GameEvents.hpp"
#include "MeteoroidBlueprint.hpp"
#include "NumberDisplayElement.hpp"
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

	StartupGameSystems();

	EventCourier::SubscribeForEvent( EVENT_Collision, EventObserver::GenerateFromOneArgFunction< MeteoroidGame, &MeteoroidGame::OnCollisionEvent >( this ) );

	m_cameraman = GetEntityManager().HireEntity();
	CameraComponent* m_gameCam = m_worldRenderingSystem->AcquireCameraComponent();
	m_cameraman->AttachComponent( m_gameCam );
	m_worldRenderingSystem->SetActiveCamera( m_gameCam );

	//Initialize Blueprints
	m_meteoroidBlueprint = new MeteoroidBlueprint( this );
	m_shipBlueprint = new ShipBlueprint();

	//Ship Creation
	Entity* playerShip = GetEntityManager().HireEntity();
	m_shipBlueprint->BuildEntityIntoGame( *playerShip, this, SHIP_SPAWN_POSITION );
	ScoringComponent* playerScoring = playerShip->FindAttachedComponentOfType< ScoringComponent >();

	Renderer* renderer = Renderer::GetRenderer();
	Material* uiTextMaterial = renderer->CreateOrGetNewMaterial( L"GameUITextMaterial" );
	uiTextMaterial->SetShaderProgram( ShaderProgram::CreateOrGetShaderProgram( "Data/Shaders/Basic.110.vertex.glsl", "Data/Shaders/Basic.110.fragment.glsl" ) );
	uiTextMaterial->SetModelMatrixUniform( "u_modelMatrix" );
	uiTextMaterial->SetViewMatrixUniform( "u_viewMatrix" );
	uiTextMaterial->SetProjectionMatrixUniform( "u_projectionMatrix" );

	std::string fontTextureLocation( "Data/Font/MainFont_EN_00.png" );
	BitmapFont* uiFont = new BitmapFont( "Data/Font/MainFont_EN.FontDef.xml", &fontTextureLocation, 1 );

	//UI Creation
	NumberDisplayElement* scoreDisplay = new NumberDisplayElement( &playerScoring->currentScore, 6, uiFont, uiTextMaterial, false );
	scoreDisplay->position.x = 50.f;
	scoreDisplay->position.y = 1030.f;
	m_UISystem->ConnectUIElement( scoreDisplay );

	StartNewLevel();

	m_explosionSound = AudioInterface::GetOrLoadSound( "Data/Audio/boom.wav" );
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoUpdate( float deltaSeconds )
{
	m_gameInputSystem->OnUpdate( deltaSeconds );

	m_warpSystem->OnUpdate( deltaSeconds );
	m_weaponSystem->OnUpdate( deltaSeconds );
	m_timedDestructionSystem->OnUpdate( deltaSeconds );
	m_physicsSystem->OnUpdate( deltaSeconds );
	m_collisionSystem->OnUpdate( deltaSeconds );
	m_fracturingSystem->OnUpdate( deltaSeconds );
	m_scoringSystem->OnUpdate( deltaSeconds );

	if( IsLevelComplete() )
		StartNewLevel();

	m_worldRenderingSystem->OnUpdate( deltaSeconds );
	m_UISystem->OnUpdate( deltaSeconds );
	m_debugUIRenderingSystem->OnUpdate( deltaSeconds );
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoRender() const
{
	Renderer* renderer = Renderer::GetRenderer();

	renderer->ClearColorBuffer();
	renderer->ClearDepthBuffer();

	m_gameInputSystem->OnRender();

	m_warpSystem->OnRender();
	m_weaponSystem->OnRender();
	m_timedDestructionSystem->OnRender();
	m_physicsSystem->OnRender();
	m_collisionSystem->OnRender();
	m_fracturingSystem->OnRender();
	m_scoringSystem->OnRender();

	m_worldRenderingSystem->OnRender();
	m_UISystem->OnRender();
	m_debugUIRenderingSystem->OnRender();
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoAtEndOfFrame()
{
	GameInterface::DoAtEndOfFrame();

	m_gameInputSystem->OnEndFrame();

	m_warpSystem->OnEndFrame();
	m_weaponSystem->OnEndFrame();
	m_timedDestructionSystem->OnEndFrame();
	m_physicsSystem->OnEndFrame();
	m_collisionSystem->OnEndFrame();
	m_fracturingSystem->OnEndFrame();
	m_scoringSystem->OnEndFrame();

	m_worldRenderingSystem->OnEndFrame();
	m_UISystem->OnEndFrame();
	m_debugUIRenderingSystem->OnEndFrame();
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoBeforeEngineDestruction()
{
	GameInterface::DoBeforeEngineDestruction();

	//Afterwards, the systems may clean up their components
	m_collisionSystem->OnDestruction();
	delete m_collisionSystem;

	m_debugUIRenderingSystem->OnDestruction();
	delete m_debugUIRenderingSystem;

	m_fracturingSystem->OnDestruction();
	delete m_fracturingSystem;

	m_gameInputSystem->OnDestruction();
	delete m_gameInputSystem;

	m_physicsSystem->OnDestruction();
	delete m_physicsSystem;

	m_scoringSystem->OnDestruction();
	delete m_scoringSystem;

	m_worldRenderingSystem->OnDestruction();
	delete m_worldRenderingSystem;

	m_timedDestructionSystem->OnDestruction();
	delete m_timedDestructionSystem;

	m_UISystem->OnDestruction();
	delete m_UISystem;

	m_warpSystem->OnDestruction();
	delete m_warpSystem;

	m_weaponSystem->OnDestruction();
	delete m_weaponSystem;

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



#pragma region Helpers
//-----------------------------------------------------------------------------------------------
void MeteoroidGame::HandleEntityDestructionOrReuse( Entity*& entity )
{
	EventDataBundle eventData;
	eventData.SetParameter( STRING_1stEntity, entity );
	EventCourier::SendEvent( EVENT_EntityDestruction, eventData );

	switch( entity->typeID )
	{
	case TYPEID_Ship:
		entity->position.x = -500.f;
		entity->position.y = -500.f;
		entity->velocity = FloatVector3( 0.f, 0.f, 0.f );
		entity->acceleration = FloatVector3( 0.f, 0.f, 0.f );

		--m_playerLivesRemaining;
		if( IsGameOver() )
		{
			//Change our game mode
		}
		else
		{
			entity->position.x = SHIP_SPAWN_POSITION.x;
			entity->position.y = SHIP_SPAWN_POSITION.y;
		}
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
					spawnedMeteor = GetEntityManager().HireEntity();
					m_meteoroidBlueprint->hint_spawnPosition = spawnPosition;
					m_meteoroidBlueprint->hint_meteorSize = fracture->fracturesRemaining;
					m_meteoroidBlueprint->BuildEntity( *spawnedMeteor );
				}
			}
			GetEntityManager().QueueEntityForFiring( entity );
		}
		break;
	default:
		GetEntityManager().QueueEntityForFiring( entity );
		break;
	}
	AudioInterface::PlaySoundThroughEmitter( m_explosionSound );
}

//-----------------------------------------------------------------------------------------------
bool MeteoroidGame::IsGameOver() const
{
	return ( m_playerLivesRemaining <= 0 );
}

//-----------------------------------------------------------------------------------------------
bool MeteoroidGame::IsLevelComplete() const
{
	unsigned int meteoroidCount = 0;
	ConstEntityIterator entity = m_activeEntityManager->GetPoolStart();
	for( ; entity != m_activeEntityManager->GetPoolEnd(); ++entity )
	{
		if( entity->IsHired() && entity->typeID == TYPEID_Meteoroid )
			++meteoroidCount;
	}
	return ( meteoroidCount <= 0 );
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::StartNewLevel()
{
	Entity* spawnedMeteor = nullptr;
	FloatVector2 spawnPosition;

	for( unsigned int i = 0; i < m_numStartingAsteroidsToSpawn; ++i )
	{
		spawnedMeteor = GetEntityManager().HireEntity();
		spawnPosition.x = GetRandomFloatBetweenZeroandOne() * m_windowDimensions.x;
		spawnPosition.y = GetRandomFloatBetweenZeroandOne() * m_windowDimensions.y;
		m_meteoroidBlueprint->hint_spawnPosition = spawnPosition;
		m_meteoroidBlueprint->hint_meteorSize = static_cast< unsigned int >( GetRandomIntBetween( 0, MeteoroidBlueprint::NUM_METEOROID_SIZES ) );
		m_meteoroidBlueprint->BuildEntity( *spawnedMeteor );
	}

	++m_levelNumber;
	m_numStartingAsteroidsToSpawn += 2;
	m_startingAsteroidsMinSpeed += 10.f;
	m_startingAsteroidsMaxSpeed += 20.f;
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::StartupGameSystems()
{
	m_collisionSystem = new CollisionSystem2D( 100 );
	m_collisionSystem->OnAttachment( nullptr );

	m_debugUIRenderingSystem = new DebugDrawingSystem2D( 0.f, static_cast<float>( m_windowDimensions.x ), 
		0.f, static_cast<float>( m_windowDimensions.y ) );
	m_debugUIRenderingSystem->OnAttachment( nullptr );

	m_gameInputSystem = new GameInputSystem( 1 );
	m_gameInputSystem->OnAttachment( nullptr );

	m_fracturingSystem = new FracturingSystem( 100 );
	m_fracturingSystem->OnAttachment( nullptr );

	m_physicsSystem = new OuterSpacePhysicsSystem( 100 );
	m_physicsSystem->OnAttachment( nullptr );

	m_scoringSystem = new ScoringSystem( 100 );
	m_scoringSystem->OnAttachment( nullptr );

	m_worldRenderingSystem = new RenderingSystem2D( 1, 100 );
	m_worldRenderingSystem->OnAttachment( nullptr );

	m_timedDestructionSystem = new TimedDestructionSystem( 100 );
	m_timedDestructionSystem->OnAttachment( nullptr );

	m_UISystem = new UISystem();
	m_UISystem->OnAttachment( nullptr );

	m_warpSystem = new WarpSystem( 1, FloatVector2( (float)m_windowDimensions.x, (float)m_windowDimensions.y ) );
	m_warpSystem->OnAttachment( nullptr );

	m_weaponSystem = new WeaponSystem( 1, this );
	m_weaponSystem->OnAttachment( nullptr );
}
#pragma endregion //Helpers
