#pragma once
#ifndef INCLUDED_WARP_COMPONENT_HPP
#define INCLUDED_WARP_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Component.hpp>

//-----------------------------------------------------------------------------------------------
struct WarpComponent : public Component
{
	WarpComponent( float minSecondsBetweenWarps = 0.f );


	//Data Members
	float secondsNeededBetweenWarps;
	float secondsSinceLastWarp;
};



//-----------------------------------------------------------------------------------------------
inline WarpComponent::WarpComponent( float minSecondsBetweenWarps )
	: secondsNeededBetweenWarps( minSecondsBetweenWarps )
	, secondsSinceLastWarp( secondsNeededBetweenWarps )
{ }

#endif //INCLUDED_WARP_COMPONENT_HPP
