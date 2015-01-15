#include "GameInputSystem.hpp"

#include <Code/Events/EventCourier.hpp>
#include <Code/Input/PeripheralInterface.hpp>
#include <Code/Math/ConvertAngles.hpp>

#include "GameEvents.hpp"


//-----------------------------------------------------------------------------------------------
void GameInputSystem::OnEndFrame()
{
	for( unsigned int i = 0; i < m_numComponentsInPool; ++i )
	{
		if( !m_componentPool[i].readyForDeletion )
			continue;

		this->RelinquishComponent( &m_componentPool[i] );
	}
}

//-----------------------------------------------------------------------------------------------
void GameInputSystem::OnUpdate( float /*deltaSeconds*/ )
{
	FloatVector3 entityHeading;

	for( unsigned int i = 0; i < m_numComponentsInPool; ++i )
	{
		if( !m_componentPool[i].IsActive() )
			continue;

		Entity*& controlledEntity = m_componentPool[i].owner;
		if( controlledEntity == nullptr )
			continue;

		ConvertEulerAnglesToVector( controlledEntity->orientation, entityHeading );

		Keyboard* keyboard = PeripheralInterface::GetKeyboard();
		controlledEntity->angularVelocity.yawDegreesAboutZ = ( keyboard->KeyIsPressedOrHeld( Keyboard::A ) - keyboard->KeyIsPressedOrHeld( Keyboard::D ) ) * 160.f;

		controlledEntity->acceleration += entityHeading * ( keyboard->KeyIsPressedOrHeld( Keyboard::W ) * 20.f );

		if( keyboard->KeyIsPressed( Keyboard::S ) )
		{
			EventDataBundle warpData;
			warpData.SetParameter( STRING_1stEntity, controlledEntity );
			EventCourier::SendEvent( EVENT_EngageWarp, warpData );
		}

		if( keyboard->KeyIsPressed( Keyboard::SPACEBAR ) )
		{
			EventDataBundle fireData;
			fireData.SetParameter( STRING_1stEntity, controlledEntity );
			EventCourier::SendEvent( EVENT_FireWeapon, fireData );
		}
	}
}

//-----------------------------------------------------------------------------------------------
void GameInputSystem::OnDestruction()
{
	ComponentSystem< GameInputComponent >::OnDestruction();
}