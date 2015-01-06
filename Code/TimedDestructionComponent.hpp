#pragma once
#ifndef INCLUDED_TIMED_DESTRUCTION_COMPONENT_HPP
#define INCLUDED_TIMED_DESTRUCTION_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Component.hpp>


//-----------------------------------------------------------------------------------------------
struct TimedDestructionComponent : public Component
{
	TimedDestructionComponent( Entity* owningEntity, float lifetimeSeconds );


	//Data Members
	float secondsLeftUntilDestruction;
};



//-----------------------------------------------------------------------------------------------
inline TimedDestructionComponent::TimedDestructionComponent( Entity* owningEntity, float lifetimeSeconds )
	: Component( owningEntity )
	, secondsLeftUntilDestruction( lifetimeSeconds )
{ }

#endif //INCLUDED_TIMED_DESTRUCTION_COMPONENT_HPP
