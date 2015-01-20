#pragma once
#ifndef INCLUDED_METEOROID_GAME_HPP
#define INCLUDED_METEOROID_GAME_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

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

class MeteoroidBlueprint;
class ShipBlueprint;

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
	DebugDrawingSystem2D* m_debugUIRenderingSystem;
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
	void HandleEntityDestructionOrReuse( Entity*& entity );
	bool IsGameOver() const;
	bool IsLevelComplete() const;
	void StartNewLevel();
	void StartupGameSystems();

	//Game Pointers
	Entity* m_cameraman;
	MeteoroidBlueprint* m_meteoroidBlueprint;
	ShipBlueprint* m_shipBlueprint;

	//Gameplay data
	IntVector2 m_windowDimensions;
	unsigned int m_playerLivesRemaining;
	unsigned int m_levelNumber;
	unsigned int m_numStartingAsteroidsToSpawn;
	float m_startingAsteroidsMinSpeed;
	float m_startingAsteroidsMaxSpeed;
};



//-----------------------------------------------------------------------------------------------
//This declaration is used to initialize the game and game interface simultaneously before main.
extern MeteoroidGame g_game;

//-----------------------------------------------------------------------------------------------
inline MeteoroidGame::MeteoroidGame()
	: m_collisionSystem( nullptr )
	, m_debugUIRenderingSystem( nullptr )
	, m_fracturingSystem( nullptr )
	, m_gameInputSystem( nullptr )
	, m_physicsSystem( nullptr )
	, m_scoringSystem( nullptr )
	, m_worldRenderingSystem( nullptr )
	, m_timedDestructionSystem( nullptr )
	, m_UISystem( nullptr )
	, m_weaponSystem( nullptr )
	, m_warpSystem( nullptr )

	, m_cameraman( nullptr )
	, m_meteoroidBlueprint( nullptr )
	, m_shipBlueprint( nullptr )

	, m_windowDimensions()
	, m_playerLivesRemaining( 3 )
	, m_levelNumber( 0 )
	, m_numStartingAsteroidsToSpawn( 4 )
	, m_startingAsteroidsMinSpeed( 0.f )
	, m_startingAsteroidsMaxSpeed( 20.f )
{ }
#endif //INCLUDED_METEOROID_GAME_HPP
