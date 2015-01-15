#include "CleanupSystem.hpp"


//-----------------------------------------------------------------------------------------------
void CleanupSystem::OnEndFrame()
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
void CleanupSystem::OnDestruction()
{
	for( unsigned int i = 0; i < m_components.size(); ++i )
	{
		delete m_components[ i ];
	}
	m_components.clear();
}