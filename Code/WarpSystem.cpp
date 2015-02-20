#include "WarpSystem.hpp"

#include <Code/Events/EventCourier.hpp>

#include "GameEvents.hpp"
#include "WarpComponent.hpp"


//-----------------------------------------------------------------------------------------------
void WarpSystem::OnAttachment( SystemManager* /*manager*/ )
{
	EventCourier::SubscribeForEvent( EVENT_EngageWarp, EventObserver::GenerateFromOneArgFunction< WarpSystem, &WarpSystem::OnWarpEngaged >( this ) );
	m_warpSound = AudioInterface::GetOrLoadSound( "Audio/warp.wav" );
}

//-----------------------------------------------------------------------------------------------
void WarpSystem::OnEndFrame()
{
	for( unsigned int i = 0; i < m_numComponentsInPool; ++i )
	{
		if( !m_componentPool[i].readyForDeletion )
			continue;

		this->RelinquishComponent( &m_componentPool[i] );
	}
}

//-----------------------------------------------------------------------------------------------
void WarpSystem::OnDestruction()
{
	ComponentSystem< WarpComponent >::OnDestruction();
}

//-----------------------------------------------------------------------------------------------
void WarpSystem::OnWarpEngaged( EventDataBundle& eventData )
{
	Entity* entityAttemptingToWarp = nullptr;
	eventData.GetParameterOrDie( STRING_1stEntity, entityAttemptingToWarp );

	WarpComponent* entityWarpComponent = entityAttemptingToWarp->FindAttachedComponentOfType< WarpComponent >();
	if( entityWarpComponent != nullptr )
	{
		AudioInterface::PlaySoundThroughEmitter( m_warpSound );
		entityAttemptingToWarp->position.x = GetRandomFloatBetweenZeroandOne() * m_warpBounds.x;
		entityAttemptingToWarp->position.y = GetRandomFloatBetweenZeroandOne() * m_warpBounds.y;
	}
}
