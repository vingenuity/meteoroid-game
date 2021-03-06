#pragma once
#ifndef INCLUDED_TIMED_DESTRUCTION_SYSTEM_HPP
#define INCLUDED_TIMED_DESTRUCTION_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/ComponentSystem.hpp>

#include "TimedDestructionComponent.hpp"


//-----------------------------------------------------------------------------------------------
class TimedDestructionSystem : public ComponentSystem< TimedDestructionComponent >
{
public:
	TimedDestructionSystem( size_t maxTDComponents ) : ComponentSystem( maxTDComponents ) { }

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ ) { }
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float deltaSeconds );
	void OnDestruction();
};

#endif //INCLUDED_TIMED_DESTRUCTION_SYSTEM_HPP
