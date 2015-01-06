#pragma once
#ifndef INCLUDED_WEAPON_SYSTEM_HPP
#define INCLUDED_WEAPON_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include <Code/Events/EventSubscriber.hpp>
#include <Code/System.hpp>

class MeteoroidGame;
class MissileBlueprint;
struct WeaponComponent;


//-----------------------------------------------------------------------------------------------
/************************************************************************************************
The weapon system is used to manage the ability of entities to shoot.

Currently, only the Ship and UFO have this ability, but it might be fun to have trap asteroids...
************************************************************************************************/
class WeaponSystem : public System, EventSubscriber
{
public:
	WeaponSystem( MeteoroidGame* game );

	void AddWeaponComponent( WeaponComponent* weapon ) { m_weaponComponents.push_back( weapon ); }

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ );
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float /*deltaSeconds*/ ) { }
	void OnDestruction();


	//Events
	void OnWeaponFireEvent( EventDataBundle& eventData );


private:
	std::vector< WeaponComponent* > m_weaponComponents;
	MeteoroidGame* m_game;
	MissileBlueprint* m_missileBlueprint;
};



//-----------------------------------------------------------------------------------------------
inline WeaponSystem::WeaponSystem( MeteoroidGame* game )
	: m_game( game )
	, m_missileBlueprint( nullptr )
{ }

#endif //INCLUDED_WEAPON_SYSTEM_HPP
