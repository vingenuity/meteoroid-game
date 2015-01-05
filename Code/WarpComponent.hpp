#pragma once
#ifndef INCLUDED_WARP_COMPONENT_HPP
#define INCLUDED_WARP_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Component.hpp>

//-----------------------------------------------------------------------------------------------
struct WarpComponent : public Component
{
	WarpComponent( Entity* owningEntity, float minSecondsBetweenWarps );


	//Data Members
	float secondsNeededBetweenWarps;
	float secondsSinceLastWarp;
};



//-----------------------------------------------------------------------------------------------
inline WarpComponent::WarpComponent( Entity* owningEntity, float minSecondsBetweenWarps )
	: Component( owningEntity )
	, secondsNeededBetweenWarps( minSecondsBetweenWarps )
	, secondsSinceLastWarp( secondsNeededBetweenWarps )
{ }

#endif //INCLUDED_WARP_COMPONENT_HPP
