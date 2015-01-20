#pragma once
#ifndef INCLUDED_WEAPON_COMPONENT_HPP
#define INCLUDED_WEAPON_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Component.hpp>


//-----------------------------------------------------------------------------------------------
struct WeaponComponent : public Component
{
	//Constructors
	WeaponComponent( float minSecsBetweenShots = 0.f );


	//Data Members
	float minSecondsBetweenShots;
};



//-----------------------------------------------------------------------------------------------
inline WeaponComponent::WeaponComponent( float minSecsBetweenShots )
	: minSecondsBetweenShots( minSecsBetweenShots )
{ }

#endif //INCLUDED_WEAPON_COMPONENT_HPP
