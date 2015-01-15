#pragma once
#ifndef INCLUDED_WEAPON_SYSTEM_HPP
#define INCLUDED_WEAPON_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Events/EventSubscriber.hpp>
#include <Code/ComponentSystem.hpp>

#include "WeaponComponent.hpp"

class MeteoroidGame;
class MissileBlueprint;


//-----------------------------------------------------------------------------------------------
/************************************************************************************************
The weapon system is used to manage the ability of entities to shoot.

Currently, only the Ship and UFO have this ability, but it might be fun to have trap asteroids...
************************************************************************************************/
class WeaponSystem : public ComponentSystem< WeaponComponent >, EventSubscriber
{
public:
	WeaponSystem( size_t numComponentsInPool, MeteoroidGame* game );

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ );
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float /*deltaSeconds*/ ) { }
	void OnDestruction();


	//Events
	void OnWeaponFireEvent( EventDataBundle& eventData );


private:
	MeteoroidGame* m_game;
	MissileBlueprint* m_missileBlueprint;
};



//-----------------------------------------------------------------------------------------------
inline WeaponSystem::WeaponSystem( size_t numComponentsInPool, MeteoroidGame* game )
	: ComponentSystem( numComponentsInPool )
	, m_game( game )
	, m_missileBlueprint( nullptr )
{ }

#endif //INCLUDED_WEAPON_SYSTEM_HPP
