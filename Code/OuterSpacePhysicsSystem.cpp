#include "OuterSpacePhysicsSystem.hpp"

#include <Code/AssertionError.hpp>
#include <Code/PhysicsComponent.hpp>


//-----------------------------------------------------------------------------------------------
STATIC const float OuterSpacePhysicsSystem::SPACE_DRAG_FACTOR = 0.8f;

#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void OuterSpacePhysicsSystem::OnAttachment( SystemManager* /*manager*/ )
{

}

//-----------------------------------------------------------------------------------------------
void OuterSpacePhysicsSystem::OnEndFrame()
{
	for( unsigned int i = 0; i < m_numComponentsInPool; ++i )
	{
		if( !m_componentPool[i].readyForDeletion )
			continue;

		this->RelinquishComponent( &m_componentPool[i] );
	}
}

//-----------------------------------------------------------------------------------------------
void OuterSpacePhysicsSystem::OnUpdate( float deltaSeconds )
{
	for( unsigned int i = 0; i < m_numComponentsInPool; ++i )
	{
		if( !m_componentPool[i].IsActive() )
			continue;

		Entity*& physicsOwner = m_componentPool[i].owner;
		FATAL_ASSERTION( physicsOwner != nullptr, "Physics System Error",
						 "An active physics component was found with no attached entity!" );

		physicsOwner->acceleration *= SPACE_DRAG_FACTOR;
		physicsOwner->velocity += physicsOwner->acceleration * deltaSeconds;
		physicsOwner->position += physicsOwner->velocity * deltaSeconds;

		physicsOwner->orientation.rollDegreesAboutX	 += physicsOwner->angularVelocity.rollDegreesAboutX	 * deltaSeconds;
		physicsOwner->orientation.pitchDegreesAboutY += physicsOwner->angularVelocity.pitchDegreesAboutY * deltaSeconds;
		physicsOwner->orientation.yawDegreesAboutZ	 += physicsOwner->angularVelocity.yawDegreesAboutZ   * deltaSeconds;
	}
}

//-----------------------------------------------------------------------------------------------
void OuterSpacePhysicsSystem::OnDestruction()
{
	ComponentSystem< PhysicsComponent >::OnDestruction();
}
#pragma endregion //Lifecycle
