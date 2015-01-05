#pragma once
#ifndef INCLUDED_WEAPON_COMPONENT_HPP
#define INCLUDED_WEAPON_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Component.hpp>


//-----------------------------------------------------------------------------------------------
struct WeaponComponent : public Component
{
	//Constructors
	WeaponComponent( Entity* owningEntity, float minSecsBetweenShots );


	//Data Members
	float minSecondsBetweenShots;
};



//-----------------------------------------------------------------------------------------------
inline WeaponComponent::WeaponComponent( Entity* owningEntity, float minSecsBetweenShots )
	: Component( owningEntity )
	, minSecondsBetweenShots( minSecsBetweenShots )
{ }

#endif INCLUDED_WEAPON_COMPONENT_HPP
