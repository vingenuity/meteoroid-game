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
#include "OuterSpacePhysicsSystem.hpp"

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

	//Game Systems
	CleanupSystem* m_cleanupSystem;
	DebugDrawingSystem2D* m_debugUIRenderingSystem;
	CollisionSystem2D* m_worldCollisionSystem;
	OuterSpacePhysicsSystem* m_worldPhysicsSystem;
	RenderingSystem* m_worldRenderingSystem;


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
	, m_debugUIRenderingSystem( nullptr )
	, m_worldCollisionSystem( nullptr )
	, m_worldPhysicsSystem( nullptr )
	, m_worldRenderingSystem( nullptr )
	, m_cameraman( nullptr )
	, m_meteoroidBlueprint( nullptr )
	, m_shipBlueprint( nullptr )
{ }
#endif //INCLUDED_METEOROID_GAME_HPP
