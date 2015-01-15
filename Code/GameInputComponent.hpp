#pragma once
#ifndef INCLUDED_GAME_INPUT_COMPONENT_HPP
#define INCLUDED_GAME_INPUT_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include <map>

#include <Code/Input/Gamepad.hpp>
#include <Code/Input/Keyboard.hpp>
#include <Code/Component.hpp>
#include <Code/NamedDataBundle.hpp>


//-----------------------------------------------------------------------------------------------
struct GameInputComponent : public Component
{
	typedef unsigned char PlayerID;
	static const PlayerID PLAYER_None = 0;
	typedef NamedDataBundle InputBundle;

	// Constructors
	GameInputComponent( PlayerID player_ID = PLAYER_None );

	// Data Members
	PlayerID playerID;

	std::map< Keyboard::Key, HashedString > keyboardMappings;
	std::map< Gamepad::ButtonID, HashedString > gamepadMappings;
	InputBundle inputs;
};



//-----------------------------------------------------------------------------------------------
inline GameInputComponent::GameInputComponent( PlayerID player_ID )
	: playerID( player_ID )
{ }

#endif //INCLUDED_GAME_INPUT_COMPONENT_HPP
