#include "TimedDestructionSystem.hpp"

#include "TimedDestructionComponent.hpp"


//-----------------------------------------------------------------------------------------------
void TimedDestructionSystem::OnEndFrame()
{
	for( unsigned int i = 0; i < m_components.size(); ++i )
	{
		if( m_components[ i ]->IsReadyForDeletion() )
		{
			delete m_components[ i ];
			m_components.erase( m_components.begin() + i );
		}
	}
}

//-----------------------------------------------------------------------------------------------
void TimedDestructionSystem::OnUpdate( float deltaSeconds )
{
	for( unsigned int i = 0; i < m_components.size(); ++i )
	{
		float& secondsLeftInLifeOfEntity = m_components[ i ]->secondsLeftUntilDestruction;
		secondsLeftInLifeOfEntity -= deltaSeconds;

		if( secondsLeftInLifeOfEntity < 0.f )
			m_components[ i ]->owner->markedForDeletion = true;
	}
}

//-----------------------------------------------------------------------------------------------
void TimedDestructionSystem::OnDestruction()
{
	for( unsigned int i = 0; i < m_components.size(); ++i )
	{
		delete m_components[ i ];
	}
	m_components.clear();
}
