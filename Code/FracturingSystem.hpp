#pragma once
#ifndef INCLUDED_FRACTURING_SYSTEM_HPP
#define INCLUDED_FRACTURING_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/ComponentSystem.hpp>

#include "FracturingComponent.hpp"


//-----------------------------------------------------------------------------------------------
class FracturingSystem : public ComponentSystem< FracturingComponent >
{
public:
	FracturingSystem( size_t maxTDComponents ) : ComponentSystem( maxTDComponents ) { }

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ ) { }
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float /*deltaSeconds*/ ) { }
	void OnDestruction();
};

#endif //INCLUDED_FRACTURING_SYSTEM_HPP
