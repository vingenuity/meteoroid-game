#pragma once
#ifndef INCLUDED_COLLISION_SYSTEM_2D
#define INCLUDED_COLLISION_SYSTEM_2D

//-----------------------------------------------------------------------------------------------
#include <vector>

#include <Code/Events/EventSubscriber.hpp>
#include <Code/System.hpp>

struct CollisionComponent2D;


//-----------------------------------------------------------------------------------------------
class CollisionSystem2D : public System, public EventSubscriber
{
public:
	CollisionSystem2D();

	void AddCollisionComponent( CollisionComponent2D* collisionComponent ) { m_collisionComponents.push_back( collisionComponent ); }

	//Lifecycle
	void OnAttachment( SystemManager* manager );
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float deltaSeconds );
	void OnDestruction();


private:
	//Data Members
	std::vector< CollisionComponent2D* > m_collisionComponents;
};



//-----------------------------------------------------------------------------------------------
inline CollisionSystem2D::CollisionSystem2D()
{
	m_collisionComponents.reserve( 50 );
}
#endif //INCLUDED_COLLISION_SYSTEM_2D
