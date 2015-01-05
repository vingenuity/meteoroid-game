#include "WarpSystem.hpp"

#include <Code/Events/EventCourier.hpp>

#include "GameEvents.hpp"
#include "WarpComponent.hpp"


//-----------------------------------------------------------------------------------------------
void WarpSystem::OnAttachment( SystemManager* /*manager*/ )
{
	EventCourier::SubscribeForEvent( EVENT_EngageWarp, EventObserver::GenerateFromOneArgFunction< WarpSystem, &WarpSystem::OnWarpEngaged >( this ) );
}

//-----------------------------------------------------------------------------------------------
void WarpSystem::OnEndFrame()
{
	for( unsigned int i = 0; i < m_warpComponents.size(); ++i )
	{
		if( m_warpComponents[ i ]->IsReadyForDeletion() )
		{
			delete m_warpComponents[ i ];
			m_warpComponents.erase( m_warpComponents.begin() + i );
		}
	}
}

//-----------------------------------------------------------------------------------------------
void WarpSystem::OnDestruction()
{
	for( unsigned int i = 0; i < m_warpComponents.size(); ++i )
	{
		delete m_warpComponents[ i ];
	}
	m_warpComponents.clear();
}

//-----------------------------------------------------------------------------------------------
void WarpSystem::OnWarpEngaged( EventDataBundle& eventData )
{
	Entity* entityAttemptingToWarp = nullptr;
	eventData.GetParameterOrDie( STRING_1stEntity, entityAttemptingToWarp );

	WarpComponent* entityWarpComponent = entityAttemptingToWarp->FindAttachedComponentOfType< WarpComponent >();
	if( entityWarpComponent != nullptr )
	{
		entityAttemptingToWarp->position.x = GetRandomFloatBetweenZeroandOne() * m_warpBounds.x;
		entityAttemptingToWarp->position.y = GetRandomFloatBetweenZeroandOne() * m_warpBounds.y;
	}
}
