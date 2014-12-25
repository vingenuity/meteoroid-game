#include "OuterSpacePhysicsSystem.hpp"

#include <Code/PhysicsComponent.hpp>



#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void OuterSpacePhysicsSystem::OnAttachment( SystemManager* /*manager*/ )
{

}

//-----------------------------------------------------------------------------------------------
void OuterSpacePhysicsSystem::OnEndFrame()
{
	for( unsigned int i = 0; i < m_physComponents.size(); ++i )
	{
		if( m_physComponents[ i ]->IsReadyForDeletion() )
		{
			delete m_physComponents[ i ];
			m_physComponents.erase( m_physComponents.begin() + i );
		}
	}
}

//-----------------------------------------------------------------------------------------------
void OuterSpacePhysicsSystem::OnUpdate( float deltaSeconds )
{
	for( unsigned int i = 0; i < m_physComponents.size(); ++i )
	{
		Entity*& physicsOwner = m_physComponents[i]->owner;
		physicsOwner->velocity += physicsOwner->acceleration * deltaSeconds;
		physicsOwner->position += physicsOwner->velocity * deltaSeconds;
	}
}

//-----------------------------------------------------------------------------------------------
void OuterSpacePhysicsSystem::OnDestruction()
{
	for( unsigned int i = 0; i < m_physComponents.size(); ++i )
	{
		delete m_physComponents[ i ];
	}
	m_physComponents.clear();
}
#pragma endregion //Lifecycle
