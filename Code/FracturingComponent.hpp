#pragma once
#ifndef INCLUDED_FRACTURING_COMPONENT_HPP
#define INCLUDED_FRACTURING_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Component.hpp>


//-----------------------------------------------------------------------------------------------
struct FracturingComponent : public Component
{
	FracturingComponent();

	//Data Members
	unsigned int fracturesRemaining;
};



//-----------------------------------------------------------------------------------------------
inline FracturingComponent::FracturingComponent()
	: fracturesRemaining( 0 )
{ }
#endif //INCLUDED_FRACTURING_COMPONENT_HPP