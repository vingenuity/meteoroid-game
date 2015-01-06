#include "WeaponSystem.hpp"

#include <Code/Events/EventCourier.hpp>
#include <Code/Math/ConvertAngles.hpp>

#include "GameEvents.hpp"
#include "MeteoroidGame.hpp"
#include "MissileBlueprint.hpp"
#include "WeaponComponent.hpp"


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
	for( unsigned int i = 0; i < m_weaponComponents.size(); ++i )
	{
		if( m_weaponComponents[ i ]->IsReadyForDeletion() )
		{
			delete m_weaponComponents[ i ];
			m_weaponComponents.erase( m_weaponComponents.begin() + i );
		}
	}
}

//-----------------------------------------------------------------------------------------------
void WeaponSystem::OnDestruction()
{
	delete m_missileBlueprint;

	for( unsigned int i = 0; i < m_weaponComponents.size(); ++i )
	{
		delete m_weaponComponents[ i ];
	}
	m_weaponComponents.clear();
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
 		FloatVector2 entityPosition( entity->position.x, entity->position.y );
 
 		firedBullet = new Entity();
 		m_missileBlueprint->BuildEntity( *firedBullet );
		firedBullet->position = entity->position;
		firedBullet->orientation = entity->orientation;
		ConvertEulerAnglesToVector( firedBullet->orientation, firedBullet->velocity );
		firedBullet->velocity *= 120.f;
 		m_game->AddEntity( firedBullet );
 	}
}
#pragma endregion //Events
