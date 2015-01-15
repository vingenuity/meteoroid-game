#include "WeaponSystem.hpp"

#include <Code/Events/EventCourier.hpp>
#include <Code/Math/ConvertAngles.hpp>
#include <Code/GameInterface.hpp>

#include "GameEvents.hpp"
#include "MeteoroidGame.hpp"
#include "MissileBlueprint.hpp"
#include "WeaponComponent.hpp"

#include <Code/DebuggerInterface.hpp>


#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void WeaponSystem::OnAttachment( SystemManager* /*manager*/ )
{
	EventCourier::SubscribeForEvent( EVENT_FireWeapon, EventObserver::GenerateFromOneArgFunction< WeaponSystem, &WeaponSystem::OnWeaponFireEvent >( this ) );

	m_missileBlueprint = new MissileBlueprint( m_game );
}

//-----------------------------------------------------------------------------------------------
void WeaponSystem::OnEndFrame()
{
	for( unsigned int i = 0; i < m_numComponentsInPool; ++i )
	{
		if( !m_componentPool[i].readyForDeletion )
			continue;

		this->RelinquishComponent( &m_componentPool[i] );
	}
}

//-----------------------------------------------------------------------------------------------
void WeaponSystem::OnDestruction()
{
	delete m_missileBlueprint;

	ComponentSystem< WeaponComponent >::OnDestruction();
}
#pragma endregion //Lifecycle



#pragma region Events
//-----------------------------------------------------------------------------------------------
void WeaponSystem::OnWeaponFireEvent( EventDataBundle& eventData )
{
	Entity* entity;
	eventData.GetParameterOrDie( STRING_1stEntity, entity );

 	WeaponComponent* weapon = entity->FindAttachedComponentOfType< WeaponComponent >();
 	if( weapon != nullptr )
 	{
 		Entity* firedBullet = nullptr;
 
  		firedBullet = GameInterface::GetEntityManager().HireEntity();
		firedBullet->creator = entity;
 		m_missileBlueprint->BuildEntity( *firedBullet );
		firedBullet->position = entity->position;
		firedBullet->orientation = entity->orientation;
		ConvertEulerAnglesToVector( firedBullet->orientation, firedBullet->velocity );
		firedBullet->velocity.z = 0.f;
		firedBullet->velocity *= 120.f;
 	}
}
#pragma endregion //Events
