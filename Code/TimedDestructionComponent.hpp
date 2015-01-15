#pragma once
#ifndef INCLUDED_TIMED_DESTRUCTION_COMPONENT_HPP
#define INCLUDED_TIMED_DESTRUCTION_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Component.hpp>


//-----------------------------------------------------------------------------------------------
struct TimedDestructionComponent : public Component
{
	TimedDestructionComponent( float lifetimeSeconds = 1.f );


	//Data Members
	float secondsLeftUntilDestruction;
};


inline TimedDestructionComponent::TimedDestructionComponent( float lifetimeSeconds )
	: secondsLeftUntilDestruction( lifetimeSeconds )
{

}

#endif //INCLUDED_TIMED_DESTRUCTION_COMPONENT_HPP
