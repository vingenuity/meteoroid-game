#include "MeteoroidGame.hpp"

#include <Code/Font/CachingFontLoader.hpp>
#include <Code/Graphics/Material.hpp>
#include <Code/Graphics/MeshGeneration3D.hpp>
#include <Code/Graphics/RendererInterface.hpp>
#include <Code/Input/PeripheralInterface.hpp>
#include <Code/AssertionError.hpp>
#include <Code/CameraComponent.hpp>
#include <Code/ColorConstants.hpp>
#include <Code/EngineMacros.hpp>
#include <Code/Entity.hpp>

#include "EntityTypes.h"
#include "FracturingComponent.hpp"
#include "GameEvents.hpp"
#include "FrameElement.hpp"
#include "LabelElement.hpp"
#include "MeteoroidBlueprint.hpp"
#include "NumberDisplayElement.hpp"
#include "ShipBlueprint.hpp"


//-----------------------------------------------------------------------------------------------
STATIC const FloatVector2	MeteoroidGame::DEAD_SHIP_POSITION( -500.f, -500.f );
STATIC const IntVector2		MeteoroidGame::WORLD_DIMENSIONS( 700, 700 );
STATIC const FloatVector2	MeteoroidGame::SHIP_SPAWN_POSITION( WORLD_DIMENSIONS.x * 0.5f, WORLD_DIMENSIONS.y * 0.5f );
STATIC const float			MeteoroidGame::SECONDS_BETWEEN_FRAME_CHANGES = 15.f;
STATIC const unsigned int	MeteoroidGame::STARTING_LIFE_COUNT = 3;

//-----------------------------------------------------------------------------------------------
MeteoroidGame g_game; //This initializes the game and the game interface for the engine simultaneously.

#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
VIRTUAL void MeteoroidGame::DoBeforeFirstFrame( unsigned int windowWidth, unsigned int windowHeight )
{
	m_windowDimensions.x = windowWidth;
	m_windowDimensions.y = windowHeight;
	SetPillarboxIfNeeded( m_windowDimensions, WORLD_DIMENSIONS );
	RendererInterface::EnableFeature( RendererInterface::COLOR_BLENDING );
	RendererInterface::SetAlphaBlendingFunction( RendererInterface::SOURCE_ALPHA, RendererInterface::ONE_MINUS_SOURCE_ALPHA );

	StartupGameSystems();

	CreateFramebuffer();

	EventCourier::SubscribeForEvent( EVENT_Collision, EventObserver::GenerateFromOneArgFunction< MeteoroidGame, &MeteoroidGame::OnCollisionEvent >( this ) );

	m_backgroundMusic = AudioInterface::GetOrLoadSound( "Audio/music-theme.wav" );
	AudioInterface::PlaySoundThroughEmitter( m_backgroundMusic, AudioInterface::ANY_EMITTER, true );

	//Framebuffer Creation
	m_framebufferVertices = new VertexData();
	GenerateTexturedPlane( *m_framebufferVertices, FloatVector3( 375.f, 375.f, 0.f ),
		FloatVector3( 0.f, 0.f, 1.f ), (float)WORLD_DIMENSIONS.x, (float)WORLD_DIMENSIONS.y,
		FloatVector2( 0.f, 0.f ), FloatVector2( 1.f, 1.f ) );
	RendererInterface::GenerateBuffer( 1, &m_framebufferVertices->bufferID );
	RendererInterface::BufferVertexData( m_framebufferVertices );

	m_framebufferMaterial = RendererInterface::CreateOrGetNewMaterial( L"FramebufferMaterial" );
	CachingShaderLoader* shaderLoader = RendererInterface::GetShaderLoader();
	ShaderPipeline* framebufferPipeline = nullptr;
	if( shaderLoader->SupportsLanguage( LANGUAGE_GLSL ) )
		framebufferPipeline = shaderLoader->CreateOrGetShaderProgramFromFiles( "Shaders/Basic.110.vertex.glsl", "Shaders/Glow.110.fragment.glsl" );
	else
		framebufferPipeline = shaderLoader->CreateOrGetShaderProgramFromFiles( "Shaders/Basic.vertex.cg", "Shaders/Glow.fragment.cg" );
	m_framebufferMaterial->SetShaderPipeline( framebufferPipeline );
	m_framebufferMaterial->SetTextureUniform( "u_diffuseMap", 0, m_framebuffer->GetAttachedColorTexture( 0 ) );

	m_cameraman = GetEntityManager().HireEntity();
	CameraComponent* m_gameCam = m_worldRenderingSystem->AcquireCameraComponent();
	m_gameCam->SetOrthographicProjection( 0.0, WORLD_DIMENSIONS.x, 0.0, WORLD_DIMENSIONS.y, 0.0, 1.0 );
	m_cameraman->AttachComponent( m_gameCam );
	m_worldRenderingSystem->SetActiveCamera( m_gameCam );

	//Initialize Blueprints
	m_meteoroidBlueprint = new MeteoroidBlueprint( this );
	m_shipBlueprint = new ShipBlueprint();

	//Player Data
	m_player[ 0 ].input = m_gameInputSystem->AcquireComponent();
	m_player[ 0 ].score = m_scoringSystem->AcquireComponent();
	m_player[ 0 ].score->pointValue = 1000;
	m_player[ 1 ].input = m_gameInputSystem->AcquireComponent();
	m_player[ 1 ].score = m_scoringSystem->AcquireComponent();
	m_player[ 1 ].score->pointValue = 1000;

	//Ship Creation
	m_playerShip = GetEntityManager().HireEntity();
	m_shipBlueprint->BuildEntityIntoGame( *m_playerShip, this, DEAD_SHIP_POSITION );

	CreateGameModeUI( m_player[ 0 ].score, m_player[ 1 ].score );
	CreateAttractModeUI();

	m_currentMode = MODE_Attract;
	StartNewLevel();

	m_explosionSound = AudioInterface::GetOrLoadSound( "Audio/boom.wav" );
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

	if( m_currentMode == MODE_Attract )
	{
		Gamepad* gamepad1 = PeripheralInterface::GetGamepadAtIndex( 0 );
		Gamepad* gamepad2 = PeripheralInterface::GetGamepadAtIndex( 1 );
		Keyboard* keyboard = PeripheralInterface::GetKeyboard();

		if( keyboard->KeyIsPressed( Keyboard::NUMBER_1 ) ||
			keyboard->KeyIsPressed( Keyboard::NUMBER_2 ) ||
			( gamepad1 != nullptr && gamepad1->IsButtonPressed( 0 ) ) ||
			( gamepad2 != nullptr && gamepad2->IsButtonPressed( 0 ) ) )
		{
			m_currentMode = MODE_Game;
			m_masterAttractFrame->isVisible = false;

			if( keyboard->KeyIsPressed( Keyboard::NUMBER_1 ) ||
				( gamepad1 != nullptr && gamepad1->IsButtonPressed( 0 ) ) )
			{
				m_player[ 0 ].livesRemaining = STARTING_LIFE_COUNT;
				m_player[ 0 ].score->currentScore = 0;
			}
			else if(keyboard->KeyIsPressed( Keyboard::NUMBER_2 ) ||
				( gamepad2 != nullptr && gamepad2->IsButtonPressed( 0 ) ) )
			{

				m_player[ 0 ].livesRemaining = STARTING_LIFE_COUNT;
				m_player[ 0 ].score->currentScore = 0;

				m_player[ 1 ].livesRemaining = STARTING_LIFE_COUNT;
				m_player[ 1 ].score->currentScore = 0;
			}

			m_activePlayerIndex = 0;
			SpawnShip( m_activePlayerIndex );
		}

		if( m_secondsSinceLastFrameChange > SECONDS_BETWEEN_FRAME_CHANGES )
		{
			m_attractFrames[ m_currentAttractFrame ]->isVisible = false;
			m_currentAttractFrame = static_cast< AttractFrame >( ( m_currentAttractFrame + 1 ) % NUMBER_OF_ATTRACT_FRAMES );
			m_attractFrames[ m_currentAttractFrame ]->isVisible = true;
			m_secondsSinceLastFrameChange = 0.f;
		}
		m_secondsSinceLastFrameChange += deltaSeconds;
	}
	else //( m_currentMode == MODE_Game )
	{
		if( IsLevelComplete() )
			StartNewLevel();
	}

	m_worldRenderingSystem->OnUpdate( deltaSeconds );
	m_UISystem->OnUpdate( deltaSeconds );
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoRender() const
{
	//World pass
	RendererInterface::UseFrameBuffer( *m_framebuffer );
	RendererInterface::ClearColorBuffer();
	RendererInterface::ClearDepthBuffer();
	RendererInterface::SetViewport( 0, 0, WORLD_DIMENSIONS.x, WORLD_DIMENSIONS.y );

	m_gameInputSystem->OnRender();

	m_warpSystem->OnRender();
	m_weaponSystem->OnRender();
	m_timedDestructionSystem->OnRender();
	m_physicsSystem->OnRender();
	m_collisionSystem->OnRender();
	m_fracturingSystem->OnRender();
	m_scoringSystem->OnRender();

	m_worldRenderingSystem->OnRender();

	RendererInterface::UseDefaultFramebuffer();
	RendererInterface::ClearColorBuffer();
	RendererInterface::ClearDepthBuffer();
	SetPillarboxIfNeeded( m_windowDimensions, WORLD_DIMENSIONS );
	RendererInterface::SetOrthographicProjection( 0.0, (double)WORLD_DIMENSIONS.x, 0.0, (double)WORLD_DIMENSIONS.y, -1.0, 1.0 );

	RendererInterface::ApplyMaterial( m_framebufferMaterial );
	RendererInterface::BindVertexDataToShader( m_framebufferVertices, m_framebufferMaterial->pipeline );

	RendererInterface::RenderVertexArray( m_framebufferVertices->shape, 0, m_framebufferVertices->numberOfVertices );

	RendererInterface::UnbindVertexDataFromShader( m_framebufferVertices, m_framebufferMaterial->pipeline );
	RendererInterface::RemoveMaterial( m_framebufferMaterial );

	m_UISystem->OnRender();

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
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::DoBeforeEngineDestruction()
{
	GameInterface::DoBeforeEngineDestruction();
	delete m_framebuffer;
	delete m_framebufferVertices;

	//Afterwards, the systems may clean up their components
	m_collisionSystem->OnDestruction();
	delete m_collisionSystem;

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
void MeteoroidGame::CreateFramebuffer()
{
	m_framebuffer = new Framebuffer( RendererInterface::CreateFramebufferObject( Framebuffer::TARGET_FOR_READING_AND_WRITING ) );

	Texture* colorTexture = RendererInterface::GetTextureManager()->CreateFramebufferColorTexture( WORLD_DIMENSIONS.x, WORLD_DIMENSIONS.y, RendererInterface::ARGB );
	RendererInterface::AttachTextureToFramebufferColorOutputSlot( colorTexture, *m_framebuffer, 0 );

#if !defined( PLATFORM_ANDROID ) //Something about framebuffers isn't meshing with the lower version Android devices
	Texture* depthTexture = RendererInterface::GetTextureManager()->CreateFramebufferDepthTexture( WORLD_DIMENSIONS.x, WORLD_DIMENSIONS.y );
	RendererInterface::AttachTextureToFramebufferDepthOutput( depthTexture, *m_framebuffer );
#endif

	RendererInterface::CheckIfFramebufferIsReady( *m_framebuffer );
	RendererInterface::UseDefaultFramebuffer();
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::CreateAttractModeUI()
{
	// Material Setup
	CachingShaderLoader* shaderLoader = RendererInterface::GetShaderLoader();

	Material* uiMaterial = RendererInterface::CreateOrGetNewMaterial( L"GameUIFlatMaterial" );
	ShaderPipeline* flatPipeline = nullptr;
	if( shaderLoader->SupportsLanguage( LANGUAGE_GLSL ) )
		flatPipeline = shaderLoader->CreateOrGetShaderProgramFromFiles( "Shaders/BasicNoTexture.110.vertex.glsl", "Shaders/BasicNoTexture.110.fragment.glsl" );
	else
		flatPipeline = shaderLoader->CreateOrGetShaderProgramFromFiles( "Shaders/BasicNoTexture.vertex.cg", "Shaders/BasicNoTexture.fragment.cg" );
	uiMaterial->SetShaderPipeline( flatPipeline );
	uiMaterial->SetLineWidth( 2.f );

	Material* uiTextMaterial = RendererInterface::CreateOrGetNewMaterial( L"GameUITextMaterial" );
	ShaderPipeline* textPipeline = nullptr;
	if( shaderLoader->SupportsLanguage( LANGUAGE_GLSL ) )
		textPipeline = shaderLoader->CreateOrGetShaderProgramFromFiles( "Shaders/Basic.110.vertex.glsl", "Shaders/Basic.110.fragment.glsl" );
	else
		textPipeline = shaderLoader->CreateOrGetShaderProgramFromFiles( "Shaders/Basic.vertex.cg", "Shaders/Basic.fragment.cg" );
	uiTextMaterial->SetShaderPipeline( textPipeline );

	m_uiFont = RendererInterface::GetFontLoader()->GetOrLoadFontFromXML( "Font/hyperspace.fnt", "Font/" );
	uiTextMaterial->SetTextureUniform( "u_diffuseMap", 0, m_uiFont->GetTextureAtlas( 0 ) );

	// Attract Mode Frame
	FrameElement* attractModeFrame = new FrameElement( uiMaterial, COLOR_Clear, COLOR_Clear );
	m_UISystem->ConnectUIElement( attractModeFrame );

	LabelElement* gameTitleLabel = new LabelElement( "Meteoroids", m_uiFont, 60, uiTextMaterial );
	gameTitleLabel->position.x = 60.f;
	gameTitleLabel->position.y = 375.f;
	attractModeFrame->InsertUIElement( gameTitleLabel );

	FrameElement* pressToStartFrame = new FrameElement( uiMaterial, COLOR_Clear, COLOR_Clear );
	pressToStartFrame->isVisible = false;
	m_attractFrames[ FRAME_PressToPlay ] = pressToStartFrame;

	LabelElement* pressToStartLabel = new LabelElement( "Press 1 or 2 to play!", m_uiFont, 30, uiTextMaterial );
	pressToStartLabel->position.x = 0.f;
	pressToStartLabel->position.y = 0.f;
	pressToStartFrame->InsertUIElement( pressToStartLabel );

	pressToStartFrame->position.y = 250.f;
	attractModeFrame->InsertUIElement( pressToStartFrame );


	FrameElement* creditsFrame = new FrameElement( uiMaterial, COLOR_Clear, COLOR_Clear );
	creditsFrame->isVisible = false;
	m_attractFrames[ FRAME_Credits ] = creditsFrame;

	static const unsigned int CREDITS_TEXT_HEIGHT = 28;
	static const float CREDITS_TEXT_SPACING = CREDITS_TEXT_HEIGHT + 10.f;

	LabelElement* creditsLabel = new LabelElement( "credits:", m_uiFont, 30, uiTextMaterial );
	creditsLabel->position.x = 250.f;
	creditsLabel->position.y =  ( CREDITS_TEXT_SPACING * 8.f ) + 10.f;
	creditsFrame->InsertUIElement( creditsLabel );

	LabelElement* developerCredit = new LabelElement( "developer:  vincent kocks", m_uiFont, CREDITS_TEXT_HEIGHT, uiTextMaterial );
	developerCredit->position.x = 0.f;
	developerCredit->position.y = CREDITS_TEXT_SPACING * 7.f;
	creditsFrame->InsertUIElement( developerCredit );

	LabelElement* fontCredit = new LabelElement( "font by:      pixel sagas", m_uiFont, CREDITS_TEXT_HEIGHT, uiTextMaterial );
	fontCredit->position.x = 0.f;
	fontCredit->position.y = CREDITS_TEXT_SPACING * 6.f;
	creditsFrame->InsertUIElement( fontCredit );

	LabelElement* soundsByCredit = new LabelElement( "audio by:", m_uiFont, CREDITS_TEXT_HEIGHT, uiTextMaterial );
	soundsByCredit->position.x = 0.f;
	soundsByCredit->position.y = CREDITS_TEXT_SPACING * 5.f;
	creditsFrame->InsertUIElement( soundsByCredit );

	LabelElement* soundCredit1 = new LabelElement( "                  levelclearer", m_uiFont, CREDITS_TEXT_HEIGHT, uiTextMaterial );
	soundCredit1->position.x = 0.f;
	soundCredit1->position.y = CREDITS_TEXT_SPACING * 4.f;
	creditsFrame->InsertUIElement( soundCredit1 );

	LabelElement* soundCredit2 = new LabelElement( "                  noirenex", m_uiFont, CREDITS_TEXT_HEIGHT, uiTextMaterial );
	soundCredit2->position.x = 0.f;
	soundCredit2->position.y = CREDITS_TEXT_SPACING * 3.f;
	creditsFrame->InsertUIElement( soundCredit2 );

	LabelElement* soundCredit3 = new LabelElement( "                  jobro", m_uiFont, CREDITS_TEXT_HEIGHT, uiTextMaterial );
	soundCredit3->position.x = 0.f;
	soundCredit3->position.y = CREDITS_TEXT_SPACING * 2.f;
	creditsFrame->InsertUIElement( soundCredit3 );

	LabelElement* soundCredit4 = new LabelElement( "                  gameaudio", m_uiFont, CREDITS_TEXT_HEIGHT, uiTextMaterial );
	soundCredit4->position.x = 0.f;
	soundCredit4->position.y = CREDITS_TEXT_SPACING * 1.f;
	creditsFrame->InsertUIElement( soundCredit4 );

	creditsFrame->position.x = 30.f;
	creditsFrame->position.y = 0.f;
	attractModeFrame->InsertUIElement( creditsFrame );

	attractModeFrame->position.x = ( UISystem::UI_LAYOUT_DIMENSIONS.x * 0.5f ) - ( attractModeFrame->width  * 0.5f );
	attractModeFrame->position.y = ( UISystem::UI_LAYOUT_DIMENSIONS.y * 0.5f ) - ( attractModeFrame->height * 0.5f );
	pressToStartFrame->isVisible = true;
	m_currentAttractFrame = FRAME_PressToPlay;
	m_masterAttractFrame = attractModeFrame;
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::CreateGameModeUI( ScoringComponent* player1ScoreComponent, ScoringComponent* player2ScoreComponent )
{
	// Material Setup
	CachingShaderLoader* shaderLoader = RendererInterface::GetShaderLoader();

	Material* uiMaterial = RendererInterface::CreateOrGetNewMaterial( L"GameUIFlatMaterial" );
	ShaderPipeline* flatPipeline = nullptr;
	if( shaderLoader->SupportsLanguage( LANGUAGE_GLSL ) )
		flatPipeline = shaderLoader->CreateOrGetShaderProgramFromFiles( "Shaders/BasicNoTexture.110.vertex.glsl", "Shaders/BasicNoTexture.110.fragment.glsl" );
	else
		flatPipeline = shaderLoader->CreateOrGetShaderProgramFromFiles( "Shaders/BasicNoTexture.vertex.cg", "Shaders/BasicNoTexture.fragment.cg" );
	uiMaterial->SetShaderPipeline( flatPipeline );
	uiMaterial->SetLineWidth( 2.f );

	Material* uiTextMaterial = RendererInterface::CreateOrGetNewMaterial( L"GameUITextMaterial" );
	ShaderPipeline* textPipeline = nullptr;
	if( shaderLoader->SupportsLanguage( LANGUAGE_GLSL ) )
		textPipeline = shaderLoader->CreateOrGetShaderProgramFromFiles( "Shaders/Basic.110.vertex.glsl", "Shaders/Basic.110.fragment.glsl" );
	else
		textPipeline = shaderLoader->CreateOrGetShaderProgramFromFiles( "Shaders/Basic.vertex.cg", "Shaders/Basic.fragment.cg" );
	uiTextMaterial->SetShaderPipeline( textPipeline );

	m_uiFont = RendererInterface::GetFontLoader()->GetOrLoadFontFromXML( "Font/hyperspace.fnt", "Font/" );
	uiTextMaterial->SetTextureUniform( "u_diffuseMap", 0, m_uiFont->GetTextureAtlas( 0 ) );


	// Player 1 Stats
	FrameElement* player1StatFrame = new FrameElement( uiMaterial, COLOR_Clear, COLOR_Clear );
	m_UISystem->ConnectUIElement( player1StatFrame );

	LabelElement* playerLabel1 = new LabelElement( "P1", m_uiFont, 60, uiTextMaterial );
	playerLabel1->position.x = 0.f;
	playerLabel1->position.y = 0.f;
	player1StatFrame->InsertUIElement( playerLabel1 );

	NumberDisplayElement* scoreDisplay1 = new NumberDisplayElement( &player1ScoreComponent->currentScore, 6, m_uiFont, 30, uiTextMaterial, false );
	scoreDisplay1->position.x = 100.f;
	scoreDisplay1->position.y = 35.f;
	player1StatFrame->InsertUIElement( scoreDisplay1 );

	NumberDisplayElement* lifeDisplay1 = new NumberDisplayElement( &m_player[ 0 ].livesRemaining, 6, m_uiFont, 30, uiTextMaterial );
	lifeDisplay1->position.x = 100.f;
	lifeDisplay1->position.y = 0.f;
	player1StatFrame->InsertUIElement( lifeDisplay1 );

	player1StatFrame->position.x = 0.f;
	player1StatFrame->position.y = UISystem::UI_LAYOUT_DIMENSIONS.y - player1StatFrame->height;


	// Player 2 Stats
	FrameElement* player2StatFrame = new FrameElement( uiMaterial, COLOR_Clear, COLOR_Clear );
	m_UISystem->ConnectUIElement( player2StatFrame );

	LabelElement* playerLabel2 = new LabelElement( "P2", m_uiFont, 60, uiTextMaterial );
	playerLabel2->position.x = 0.f;
	playerLabel2->position.y = 0.f;
	player2StatFrame->InsertUIElement( playerLabel2 );

	NumberDisplayElement* scoreDisplay2 = new NumberDisplayElement( &player2ScoreComponent->currentScore, 6, m_uiFont, 30, uiTextMaterial, false );
	scoreDisplay2->position.x = 100.f;
	scoreDisplay2->position.y = 35.f;
	player2StatFrame->InsertUIElement( scoreDisplay2 );

	NumberDisplayElement* lifeDisplay2 = new NumberDisplayElement( &m_player[ 1 ].livesRemaining, 6, m_uiFont, 30, uiTextMaterial );
	lifeDisplay2->position.x = 100.f;
	lifeDisplay2->position.y = 0.f;
	player2StatFrame->InsertUIElement( lifeDisplay2 );

	player2StatFrame->position.x = UISystem::UI_LAYOUT_DIMENSIONS.x - player2StatFrame->width;
	player2StatFrame->position.y = UISystem::UI_LAYOUT_DIMENSIONS.y - player2StatFrame->height;
}

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

		--m_player[ m_activePlayerIndex ].livesRemaining;
		if( IsGameOver() )
		{
			//Change our game mode
			m_currentMode = MODE_Attract;
			m_masterAttractFrame->isVisible = true;

			m_playerShip->DetachComponent( m_player[ m_activePlayerIndex ].input );
			m_playerShip->DetachComponent( m_player[ m_activePlayerIndex ].score );
		}
		else
		{
			m_playerShip->DetachComponent( m_player[ m_activePlayerIndex ].input );
			m_playerShip->DetachComponent( m_player[ m_activePlayerIndex ].score );

			// Since we only have two players, we can do this trick
			unsigned int nextPlayerIndex = 1 - m_activePlayerIndex;

			if( m_player[ nextPlayerIndex ].livesRemaining > 0 )
				m_activePlayerIndex = nextPlayerIndex; 
			SpawnShip( m_activePlayerIndex );
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

				for( unsigned int i = 0; i < 2; ++i )
				{
					spawnedMeteor = GetEntityManager().HireEntity();
					m_meteoroidBlueprint->hint_spawnPosition = spawnPosition;
					m_meteoroidBlueprint->hint_meteorSize = fracture->fracturesRemaining;
					m_meteoroidBlueprint->BuildEntity( *spawnedMeteor );
					spawnedMeteor->velocity.x = ( GetRandomFloatBetweenZeroandOne() - 0.5f ) * 120.f;
					spawnedMeteor->velocity.y = ( GetRandomFloatBetweenZeroandOne() - 0.5f ) * 120.f;
					spawnedMeteor->angularVelocity.yawDegreesAboutZ = ( GetRandomFloatBetweenZeroandOne() - 0.5f ) * 240.f;
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
	return ( ( m_player[ 0 ].livesRemaining <= 0 ) && ( m_player[ 1 ].livesRemaining <= 0 ) );
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
STATIC void MeteoroidGame::SetPillarboxIfNeeded( const IntVector2& windowDimensions, const IntVector2& gameDimensions )
{
	// This is your target virtual resolution for the game, the size you built your game to
	float targetAspectRatio = static_cast< float >( gameDimensions.x / gameDimensions.y );

	// figure out the largest area that fits in this resolution at the desired aspect ratio
	int viewportWidth = gameDimensions.x;
	int viewportHeight = static_cast< int >( viewportWidth / targetAspectRatio + 0.5f );

	if ( viewportHeight != windowDimensions.y )
	{
		//It doesn't fit our height, we must switch to pillarbox then
		viewportHeight = windowDimensions.y;
		viewportWidth = static_cast< int >( viewportHeight * targetAspectRatio + 0.5f );
	}

	// set up the new viewport centered in the backbuffer
	int viewportStartX = ( windowDimensions.x / 2 ) - ( viewportWidth / 2 );
	int viewportStartY = ( windowDimensions.y / 2 ) - ( viewportHeight / 2 );

	RendererInterface::SetViewport( viewportStartX, viewportStartY, viewportWidth, viewportHeight );
}

//-----------------------------------------------------------------------------------------------
void MeteoroidGame::SpawnShip( unsigned int playerIndex )
{
	m_playerShip->position.x = SHIP_SPAWN_POSITION.x;
	m_playerShip->position.y = SHIP_SPAWN_POSITION.y;

	m_playerShip->AttachComponent( m_player[ playerIndex ].input );
	m_playerShip->AttachComponent( m_player[ playerIndex ].score );
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
	FloatVector2 worldDimensionsAsFloat = FloatVector2( (const float)WORLD_DIMENSIONS.x, (const float)WORLD_DIMENSIONS.y );

	m_collisionSystem = new CollisionSystem2D( 100, worldDimensionsAsFloat );
	m_collisionSystem->OnAttachment( nullptr );

	m_gameInputSystem = new GameInputSystem( MAX_NUMBER_OF_PLAYERS );
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

	m_UISystem = new UISystem( m_windowDimensions );
	m_UISystem->OnAttachment( nullptr );

	m_warpSystem = new WarpSystem( 1, worldDimensionsAsFloat );
	m_warpSystem->OnAttachment( nullptr );

	m_weaponSystem = new WeaponSystem( 1, this );
	m_weaponSystem->OnAttachment( nullptr );
}
#pragma endregion //Helpers
