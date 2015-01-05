#pragma once
#ifndef INCLUDED_GAME_INPUT_SYSTEM_HPP
#define INCLUDED_GAME_INPUT_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>
#include <Code/System.hpp>

#include "GameInputComponent.hpp"


//-----------------------------------------------------------------------------------------------
class GameInputSystem : public System
{
public:
	GameInputSystem() { }

	void AddInputComponent( GameInputComponent* component ) { m_inputComponents.push_back( component ); }

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ ) { }
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float deltaSeconds );
	void OnDestruction();


private:
	//Data Members
	std::vector< GameInputComponent* > m_inputComponents;
};

#endif //INCLUDED_GAME_INPUT_SYSTEM_HPP
