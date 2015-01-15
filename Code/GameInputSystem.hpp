#pragma once
#ifndef INCLUDED_GAME_INPUT_SYSTEM_HPP
#define INCLUDED_GAME_INPUT_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/ComponentSystem.hpp>

#include "GameInputComponent.hpp"


//-----------------------------------------------------------------------------------------------
class GameInputSystem : public ComponentSystem< GameInputComponent >
{
public:
	GameInputSystem( size_t maxInputComponentsInPool );

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ ) { }
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float deltaSeconds );
	void OnDestruction();
};



//-----------------------------------------------------------------------------------------------
inline GameInputSystem::GameInputSystem( size_t maxInputComponentsInPool )
	: ComponentSystem( maxInputComponentsInPool )
{ }

#endif //INCLUDED_GAME_INPUT_SYSTEM_HPP
