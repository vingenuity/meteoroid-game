#pragma once
#ifndef INCLUDED_COLLISION_SYSTEM_2D
#define INCLUDED_COLLISION_SYSTEM_2D

//-----------------------------------------------------------------------------------------------
#include <Code/Events/EventSubscriber.hpp>
#include <Code/ComponentSystem.hpp>

#include "CollisionComponent2D.hpp"


//-----------------------------------------------------------------------------------------------
class CollisionSystem2D : public ComponentSystem< CollisionComponent2D >, public EventSubscriber
{
public:
	CollisionSystem2D( unsigned int numCollidersInPool );

	//Lifecycle
	void OnAttachment( SystemManager* manager );
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float deltaSeconds );
	void OnDestruction();

};



//-----------------------------------------------------------------------------------------------
inline CollisionSystem2D::CollisionSystem2D( unsigned int numCollidersInPool )
	: ComponentSystem( numCollidersInPool )
{
}
#endif //INCLUDED_COLLISION_SYSTEM_2D
