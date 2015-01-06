#pragma once
#ifndef INCLUDED_METEOROID_GAME_HPP
#define INCLUDED_METEOROID_GAME_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include <Code/Graphics/DebugDrawingSystem2D.hpp>
#include <Code/Graphics/PerspectiveRenderingSystem.hpp>

#include <Code/Math/FloatVector3.hpp>
#include <Code/Math/IntVector2.hpp>

#include <Code/GameInterface.hpp>

#include "CleanupSystem.hpp"
#include "CollisionSystem2D.hpp"
#include "GameInputSystem.hpp"
#include "OuterSpacePhysicsSystem.hpp"
#include "TimedDestructionSystem.hpp"
#include "WarpSystem.hpp"
#include "WeaponSystem.hpp"

struct Entity;
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

	//Entity Helper
	void AddEntity( Entity* entity ) { m_entities.push_back( entity ); }

	//Game Systems
	CleanupSystem* m_cleanupSystem;
	CollisionSystem2D* m_collisionSystem;
	DebugDrawingSystem2D* m_debugUIRenderingSystem;
	GameInputSystem* m_gameInputSystem;
	OuterSpacePhysicsSystem* m_physicsSystem;
	RenderingSystem* m_worldRenderingSystem;
	TimedDestructionSystem* m_timedDestructionSystem;
	WarpSystem* m_warpSystem;
	WeaponSystem* m_weaponSystem;


private:
	//Events
	void OnCollisionEvent( EventDataBundle& eventData );

	void HandleEntityDestructionOrReuse( Entity*& entity );
	void SpawnInitialMeteoroids();
	void StartupGameSystems();
	IntVector2 m_windowDimensions;

	//Game Pointers
	std::vector< Entity* > m_entities;
	Entity* m_cameraman;
	MeteoroidBlueprint* m_meteoroidBlueprint;
	ShipBlueprint* m_shipBlueprint;
};



//-----------------------------------------------------------------------------------------------
//This declaration is used to initialize the game and game interface simultaneously before main.
extern MeteoroidGame g_game;

//-----------------------------------------------------------------------------------------------
inline MeteoroidGame::MeteoroidGame()
	: m_windowDimensions()
	, m_cleanupSystem( nullptr )
	, m_collisionSystem( nullptr )
	, m_debugUIRenderingSystem( nullptr )
	, m_gameInputSystem( nullptr )
	, m_physicsSystem( nullptr )
	, m_worldRenderingSystem( nullptr )
	, m_timedDestructionSystem( nullptr )
	, m_weaponSystem( nullptr )
	, m_warpSystem( nullptr )
	, m_cameraman( nullptr )
	, m_meteoroidBlueprint( nullptr )
	, m_shipBlueprint( nullptr )
{ }
#endif //INCLUDED_METEOROID_GAME_HPP
