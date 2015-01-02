#pragma once
#ifndef INCLUDED_GAMEPLAY_COMPONENT_HPP
#define INCLUDED_GAMEPLAY_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Component.hpp>


//-----------------------------------------------------------------------------------------------
struct GameplayComponent : public Component
{
	GameplayComponent( Entity* owningEntity );

	//Data Members
};



//-----------------------------------------------------------------------------------------------
inline GameplayComponent::GameplayComponent( Entity* owningEntity )
	: Component( owningEntity )
{ }
#endif //INCLUDED_GAMEPLAY_COMPONENT_HPP
