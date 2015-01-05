#include "GameInputSystem.hpp"

#include <Code/Events/EventCourier.hpp>
#include <Code/Input/PeripheralInterface.hpp>
#include <Code/Math/ConvertAngles.hpp>

#include "GameEvents.hpp"


//-----------------------------------------------------------------------------------------------
void GameInputSystem::OnEndFrame()
{
	for( unsigned int i = 0; i < m_inputComponents.size(); ++i )
	{
		if( m_inputComponents[ i ]->IsReadyForDeletion() )
		{
			delete m_inputComponents[ i ];
			m_inputComponents.erase( m_inputComponents.begin() + i );
		}
	}
}

//-----------------------------------------------------------------------------------------------
void GameInputSystem::OnUpdate( float /*deltaSeconds*/ )
{
	FloatVector3 entityHeading;

	for( unsigned int i = 0; i < m_inputComponents.size(); ++i )
	{
		Entity*& controlledEntity = m_inputComponents[i]->owner;
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
	}
}

//-----------------------------------------------------------------------------------------------
void GameInputSystem::OnDestruction()
{
	for( unsigned int i = 0; i < m_inputComponents.size(); ++i )
	{
		delete m_inputComponents[ i ];
	}
	m_inputComponents.clear();
}