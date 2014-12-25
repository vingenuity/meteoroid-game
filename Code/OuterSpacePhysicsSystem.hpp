#pragma once
#ifndef INCLUDED_OUTER_SPACE_PHYSICS_SYSTEM_HPP
#define INCLUDED_OUTER_SPACE_PHYSICS_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include <Code/Math/FloatVector2.hpp>
#include <Code/System.hpp>

struct PhysicsComponent;


//-----------------------------------------------------------------------------------------------
class OuterSpacePhysicsSystem : public System
{
public:
	OuterSpacePhysicsSystem();

	void AddPhysicsComponent( PhysicsComponent* physicsComponent ) { m_physComponents.push_back( physicsComponent ); }

	//Lifecycle
	void OnAttachment( SystemManager* manager );
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float deltaSeconds );
	void OnDestruction();


private:
	//Data Members
	std::vector< PhysicsComponent* > m_physComponents;
};



//-----------------------------------------------------------------------------------------------
inline OuterSpacePhysicsSystem::OuterSpacePhysicsSystem()
{ }
#endif //INCLUDED_OUTER_SPACE_PHYSICS_SYSTEM_HPP
