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
	typedef NamedDataBundle InputBundle;

	// Constructors
	GameInputComponent( Entity* owningEntity, PlayerID player_ID = 0 );

	// Data Members
	PlayerID playerID;

	std::map< Keyboard::Key, HashedString > keyboardMappings;
	std::map< Gamepad::ButtonID, HashedString > gamepadMappings;
	InputBundle inputs;
};



//-----------------------------------------------------------------------------------------------
inline GameInputComponent::GameInputComponent( Entity* owningEntity, PlayerID player_ID )
	: Component( owningEntity )
	, playerID( player_ID )
{ }

#endif //INCLUDED_GAME_INPUT_COMPONENT_HPP
