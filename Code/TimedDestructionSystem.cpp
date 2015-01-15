#include "TimedDestructionSystem.hpp"

#include <Code/GameInterface.hpp>


//-----------------------------------------------------------------------------------------------
void TimedDestructionSystem::OnEndFrame()
{
	for( unsigned int i = 0; i < m_numComponentsInPool; ++i )
	{
		if( !m_componentPool[i].readyForDeletion )
			continue;

		this->RelinquishComponent( &m_componentPool[i] );
	}
}

//-----------------------------------------------------------------------------------------------
void TimedDestructionSystem::OnUpdate( float deltaSeconds )
{
	for( unsigned int i = 0; i < m_numComponentsInPool; ++i )
	{
		if( !m_componentPool[i].IsActive() )
			continue;

		float& secondsLeftInLifeOfEntity = m_componentPool[ i ].secondsLeftUntilDestruction;
		secondsLeftInLifeOfEntity -= deltaSeconds;

		if( secondsLeftInLifeOfEntity < 0.f )
			GameInterface::GetEntityManager().QueueEntityForFiring( m_componentPool[ i ].owner );
	}
}

//-----------------------------------------------------------------------------------------------
void TimedDestructionSystem::OnDestruction()
{
	ComponentSystem< TimedDestructionComponent >::OnDestruction();
}
