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
	CollisionSystem2D( unsigned int numCollidersInPool, const FloatVector2& worldDimensions );

	//Lifecycle
	void OnAttachment( SystemManager* manager );
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float deltaSeconds );
	void OnDestruction();

	FloatVector2 m_worldDimensions;
};



//-----------------------------------------------------------------------------------------------
inline CollisionSystem2D::CollisionSystem2D( unsigned int numCollidersInPool, const FloatVector2& worldDimensions )
	: ComponentSystem( numCollidersInPool )
	, m_worldDimensions( worldDimensions )
{
}
#endif //INCLUDED_COLLISION_SYSTEM_2D
