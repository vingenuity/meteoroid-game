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
#include <Code/TerrestrialPhysicsSystem.hpp>

struct Entity;


//-----------------------------------------------------------------------------------------------
class MeteoroidGame : public GameInterface
{
public:
	MeteoroidGame();
	~MeteoroidGame();

	//Lifecycle
	virtual void DoBeforeEngineInitialization() { }
	virtual void DoBeforeFirstFrame( unsigned int windowWidth, unsigned int windowHeight );

	void DoUpdate( float deltaSeconds );
	void DoRender() const;
	void DoAtEndOfFrame();

	virtual void DoBeforeEngineDestruction() { }
	virtual void DoAfterEngineDestruction() { }

	//Game Systems
	TerrestrialPhysicsSystem* m_worldPhysicsSystem;
	RenderingSystem* m_worldRenderingSystem;
	DebugDrawingSystem2D* m_debugUIRenderingSystem;


private:
	void StartupGameSystems();
	IntVector2 m_windowDimensions;

	//Game Pointers
	std::vector< Entity* > m_entities;
	Entity* m_cameraman;
};



//-----------------------------------------------------------------------------------------------
MeteoroidGame g_game; //This initializes the game and the game interface for the engine simultaneously.

//-----------------------------------------------------------------------------------------------
inline MeteoroidGame::MeteoroidGame()
	: m_windowDimensions()
	, m_worldPhysicsSystem( nullptr )
	, m_worldRenderingSystem( nullptr )
	, m_debugUIRenderingSystem( nullptr )
	, m_cameraman( nullptr )
{ }
#endif //INCLUDED_METEOROID_GAME_HPP