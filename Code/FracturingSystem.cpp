#include "FracturingSystem.hpp"

#include <Code/GameInterface.hpp>


//-----------------------------------------------------------------------------------------------
void FracturingSystem::OnEndFrame()
{
	for( unsigned int i = 0; i < m_numComponentsInPool; ++i )
	{
		if( !m_componentPool[i].readyForDeletion )
			continue;

		this->RelinquishComponent( &m_componentPool[i] );
	}
}

//-----------------------------------------------------------------------------------------------
void FracturingSystem::OnDestruction()
{
	ComponentSystem< FracturingComponent >::OnDestruction();
}