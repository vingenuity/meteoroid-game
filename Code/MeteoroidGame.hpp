#pragma once
#ifndef INCLUDED_METEOROID_GAME_HPP
#define INCLUDED_METEOROID_GAME_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include <Code/Audio/AudioInterface.hpp>
#include <Code/Graphics/DebugDrawingSystem2D.hpp>
#include <Code/Math/FloatVector3.hpp>
#include <Code/Math/IntVector2.hpp>
#include <Code/GameInterface.hpp>

#include "CollisionSystem2D.hpp"
#include "FracturingSystem.hpp"
#include "GameInputSystem.hpp"
#include "OuterSpacePhysicsSystem.hpp"
#include "RenderingSystem2D.hpp"
#include "ScoringSystem.hpp"
#include "TimedDestructionSystem.hpp"
#include "UISystem.hpp"
#include "WarpSystem.hpp"
#include "WeaponSystem.hpp"

class Framebuffer;
struct FrameElement;
class MeteoroidBlueprint;
class ShipBlueprint;

//-----------------------------------------------------------------------------------------------
struct PlayerData
{
	PlayerData()
		: input( nullptr )
		, score( nullptr )
		, livesRemaining( 0 )
	{ }

	// Data Members
	GameInputComponent* input;
	ScoringComponent* score;
	int livesRemaining;
};

//-----------------------------------------------------------------------------------------------
class MeteoroidGame : public GameInterface
{
public:
	MeteoroidGame();
	~MeteoroidGame() { }

	//Lifecycle
	virtual void DoBeforeEngineInitialization() { }
	virtual void DoBeforeFirstFrame( unsigned int windowWidth, unsigned int windowHeight );

	void DoUpdate( float deltaSeconds );
	void DoRender() const;
	void DoAtEndOfFrame();

	virtual void DoBeforeEngineDestruction();
	virtual void DoAfterEngineDestruction() { }


	//Game Systems
	CollisionSystem2D* m_collisionSystem;
	FracturingSystem* m_fracturingSystem;
	GameInputSystem* m_gameInputSystem;
	OuterSpacePhysicsSystem* m_physicsSystem;
	RenderingSystem2D* m_worldRenderingSystem;
	ScoringSystem* m_scoringSystem;
	TimedDestructionSystem* m_timedDestructionSystem;
	UISystem* m_UISystem;
	WarpSystem* m_warpSystem;
	WeaponSystem* m_weaponSystem;


private:
	//Events
	void OnCollisionEvent( EventDataBundle& eventData );

	//Helpers
	void CreateFramebuffer();
	void CreateAttractModeUI();
	void CreateGameModeUI( ScoringComponent* player1ScoreComponent, ScoringComponent* player2ScoreComponent );
	void HandleEntityDestructionOrReuse( Entity*& entity );
	bool IsGameOver() const;
	bool IsLevelComplete() const;
	static void SetPillarboxIfNeeded( const IntVector2& windowDimensions, const IntVector2& gameDimensions );
	void SpawnShip( unsigned int playerIndex );
	void StartNewLevel();
	void StartupGameSystems();

	//Game Pointers
	AudioInterface::SoundID m_backgroundMusic;
	Entity* m_cameraman;
	Framebuffer* m_framebuffer;
	Material* m_framebufferMaterial;
	VertexData* m_framebufferVertices;
	MeteoroidBlueprint* m_meteoroidBlueprint;
	ShipBlueprint* m_shipBlueprint;
	BitmapFont* m_uiFont;

	// Window/World Data
	static const IntVector2 WORLD_DIMENSIONS;
	IntVector2 m_windowDimensions;
	enum Mode
	{
		MODE_Startup,
		MODE_Game,
		MODE_Attract,
		NUMBER_OF_MODES
	};
	Mode m_currentMode;

	// Game Mode Data
	static const FloatVector2 DEAD_SHIP_POSITION;
	static const FloatVector2 SHIP_SPAWN_POSITION;
	static const unsigned int STARTING_LIFE_COUNT;
	int m_levelNumber;
	unsigned int m_numStartingAsteroidsToSpawn;
	float m_startingAsteroidsMinSpeed;
	float m_startingAsteroidsMaxSpeed;
	Entity* m_playerShip;
	static const unsigned int MAX_NUMBER_OF_PLAYERS = 2;
	PlayerData m_player[ MAX_NUMBER_OF_PLAYERS ];
	unsigned int m_activePlayerIndex;

	// Attract Mode Data
	enum AttractFrame
	{
		FRAME_PressToPlay,
		FRAME_Credits,
		NUMBER_OF_ATTRACT_FRAMES,
		FRAME_None = 255
	};
	FrameElement* m_masterAttractFrame;
	FrameElement* m_attractFrames[ NUMBER_OF_ATTRACT_FRAMES ];
	AttractFrame m_currentAttractFrame;
	float m_secondsSinceLastFrameChange;
	static const float SECONDS_BETWEEN_FRAME_CHANGES;

	//Stuff that should probably go into some component
	AudioInterface::SoundID m_explosionSound;
};



//-----------------------------------------------------------------------------------------------
//This declaration is used to initialize the game and game interface simultaneously before main.
extern MeteoroidGame g_game;

//-----------------------------------------------------------------------------------------------
inline MeteoroidGame::MeteoroidGame()
	: m_collisionSystem( nullptr )
	, m_fracturingSystem( nullptr )
	, m_gameInputSystem( nullptr )
	, m_physicsSystem( nullptr )
	, m_scoringSystem( nullptr )
	, m_worldRenderingSystem( nullptr )
	, m_timedDestructionSystem( nullptr )
	, m_UISystem( nullptr )
	, m_weaponSystem( nullptr )
	, m_warpSystem( nullptr )

	, m_backgroundMusic( 0 )
	, m_cameraman( nullptr )
	, m_framebuffer( nullptr )
	, m_framebufferMaterial( nullptr )
	, m_framebufferVertices( nullptr )
	, m_meteoroidBlueprint( nullptr )
	, m_shipBlueprint( nullptr )
	, m_uiFont( nullptr )

	, m_windowDimensions()
	, m_currentMode( MODE_Startup )

	, m_levelNumber( 0 )
	, m_playerShip( nullptr )
	, m_numStartingAsteroidsToSpawn( 4 )
	, m_startingAsteroidsMinSpeed( 0.f )
	, m_startingAsteroidsMaxSpeed( 20.f )
	, m_activePlayerIndex( 0 )

	, m_masterAttractFrame( nullptr )
	, m_currentAttractFrame( FRAME_None )
	, m_secondsSinceLastFrameChange( 0.f )
{ }
#endif //INCLUDED_METEOROID_GAME_HPP
