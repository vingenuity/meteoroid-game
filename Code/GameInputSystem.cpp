#include "GameInputSystem.hpp"

#include <Code/Events/EventCourier.hpp>
#include <Code/Input/PeripheralInterface.hpp>
#include <Code/Math/ConvertAngles.hpp>
#include <Code/DebuggerInterface.hpp>

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


		Keyboard* keyboard = PeripheralInterface::GetKeyboard();
		TouchScreen* touchscreen = PeripheralInterface::GetTouchscreen();
		unsigned int numScreenTouches = touchscreen->GetNumberOfTouches();

		if( numScreenTouches == 1 )
		{
			FloatVector3 touchPosition( touchscreen->GetTouchPointAtIndex( 0 ).location );
			FloatVector3 nextEntityHeading = touchPosition - controlledEntity->position;
			controlledEntity->orientation.yawDegreesAboutZ = ConvertRadiansToDegrees( ConvertVectorToAngleRadians( FloatVector2( nextEntityHeading.x, nextEntityHeading.y ) ) );
		}

		ConvertEulerAnglesToVector( controlledEntity->orientation, entityHeading );

		if( keyboard->KeyIsPressedOrHeld( Keyboard::A ) || keyboard->KeyIsPressedOrHeld( Keyboard::D ) )
			controlledEntity->angularVelocity.yawDegreesAboutZ = ( keyboard->KeyIsPressedOrHeld( Keyboard::A ) - keyboard->KeyIsPressedOrHeld( Keyboard::D ) ) * 160.f;
		else
			controlledEntity->angularVelocity.yawDegreesAboutZ = 0.f;

		if( keyboard->KeyIsPressedOrHeld( Keyboard::W ) )
			controlledEntity->acceleration += entityHeading * ( ( keyboard->KeyIsPressedOrHeld( Keyboard::W ) + numScreenTouches ) * 20.f );

		Gamepad* gamepad = PeripheralInterface::GetGamepadAtIndex( 0 );
		if( ( gamepad != nullptr ) && ( gamepad->IsAxisPressedOrHeld( 0 ) || gamepad->IsAxisPressedOrHeld( 1 ) ) )
		{
			FloatVector2 gamepad2DAxis( gamepad->GetRawAxisValue( 0 ), gamepad->GetRawAxisValue( 1 ) );
			controlledEntity->orientation.yawDegreesAboutZ = ConvertRadiansToDegrees( ConvertVectorToAngleRadians( gamepad2DAxis ) );
			controlledEntity->acceleration = entityHeading * FloatVector3( gamepad2DAxis ).CalculateNorm() * 100.f;
		}

		if( keyboard->KeyIsPressed( Keyboard::S ) || ( numScreenTouches == 2 ) || ( ( gamepad != nullptr ) && gamepad->IsButtonPressed( 1 ) ) )
		{
			EventDataBundle warpData;
			warpData.SetParameter( STRING_1stEntity, controlledEntity );
			EventCourier::SendEvent( EVENT_EngageWarp, warpData );
		}

		if( keyboard->KeyIsPressed( Keyboard::SPACEBAR ) || ( touchscreen->FindNumberOfStartedTouches() == 1 ) || ( ( gamepad != nullptr ) && gamepad->IsButtonPressed( 0 ) ) )
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