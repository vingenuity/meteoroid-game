#pragma once
#ifndef INCLUDED_GAME_EVENTS_HPP
#define INCLUDED_GAME_EVENTS_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Events/EventCourier.hpp>


//-----------------------------------------------------------------------------------------------
static const EventName EVENT_Collision = HashedString( "CollisionEvent" );
static const EventName EVENT_EntityDestruction = HashedString( "EntityDestruction" );


//-----------------------------------------------------------------------------------------------
static const HashedString STRING_1stCollider = HashedString( "1stCollider" );
static const HashedString STRING_2ndCollider = HashedString( "2ndCollider" );
static const HashedString STRING_1stEntity = HashedString( "1stEntity" );
static const HashedString STRING_2ndEntity = HashedString( "2ndEntity" );

#endif //INCLUDED_GAME_EVENTS_HPP
