#pragma once
#ifndef INCLUDED_SCORING_SYSTEM_HPP
#define INCLUDED_SCORING_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/ComponentSystem.hpp>
#include <Code/Events/EventSubscriber.hpp>

#include "ScoringComponent.hpp"


//-----------------------------------------------------------------------------------------------
class ScoringSystem : public ComponentSystem < ScoringComponent >, EventSubscriber
{
public:
	ScoringSystem( size_t maxScoringComponentsInPool );

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ );
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float deltaSeconds );
	void OnDestruction();

private:
	//Event Functions
	void OnCollisionEvent( EventDataBundle& eventData );
	void OnEntityDestruction( EventDataBundle& eventData );
};



//-----------------------------------------------------------------------------------------------
inline ScoringSystem::ScoringSystem( size_t maxScoringComponentsInPool )
	: ComponentSystem( maxScoringComponentsInPool )
{ }

#endif //INCLUDED_SCORING_SYSTEM_HPP
