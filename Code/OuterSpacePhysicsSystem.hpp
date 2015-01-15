#pragma once
#ifndef INCLUDED_OUTER_SPACE_PHYSICS_SYSTEM_HPP
#define INCLUDED_OUTER_SPACE_PHYSICS_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Math/FloatVector2.hpp>
#include <Code/ComponentSystem.hpp>
#include <Code/PhysicsComponent.hpp>


//-----------------------------------------------------------------------------------------------
class OuterSpacePhysicsSystem : public ComponentSystem< PhysicsComponent >
{
	static const float SPACE_DRAG_FACTOR;

public:
	OuterSpacePhysicsSystem( size_t maxPhysicsComponents );

	//Lifecycle
	void OnAttachment( SystemManager* manager );
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float deltaSeconds );
	void OnDestruction();
};



//-----------------------------------------------------------------------------------------------
inline OuterSpacePhysicsSystem::OuterSpacePhysicsSystem( size_t maxPhysicsComponents )
	: ComponentSystem( maxPhysicsComponents )
{ }
#endif //INCLUDED_OUTER_SPACE_PHYSICS_SYSTEM_HPP
