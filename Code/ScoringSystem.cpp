#include "ScoringSystem.hpp"

#include <Code/Events/EventCourier.hpp>
#include <Code/GameInterface.hpp>

#include "GameEvents.hpp"


#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void ScoringSystem::OnAttachment( SystemManager* /*manager*/ )
{
	EventCourier::SubscribeForEvent( EVENT_Collision, EventObserver::GenerateFromOneArgFunction< ScoringSystem, &ScoringSystem::OnCollisionEvent >( this ) );
	EventCourier::SubscribeForEvent( EVENT_EntityDestruction, EventObserver::GenerateFromOneArgFunction< ScoringSystem, &ScoringSystem::OnEntityDestruction >( this ) );
}

//-----------------------------------------------------------------------------------------------
void ScoringSystem::OnEndFrame()
{
	for( unsigned int i = 0; i < m_numComponentsInPool; ++i )
	{
		if( !m_componentPool[i].readyForDeletion )
			continue;

		this->RelinquishComponent( &m_componentPool[i] );
	}
}

//-----------------------------------------------------------------------------------------------
void ScoringSystem::OnUpdate( float /*deltaSeconds*/ )
{
}

//-----------------------------------------------------------------------------------------------
void ScoringSystem::OnDestruction()
{
	ComponentSystem< ScoringComponent >::OnDestruction();
}
#pragma endregion //Lifecycle



#pragma region //Event Functions
//-----------------------------------------------------------------------------------------------
void ScoringSystem::OnCollisionEvent( EventDataBundle& eventData )
{
	static const size_t NUM_ENTITIES_IN_COLLISION = 2;
	Entity* entitiesInvolvedInCollision[ NUM_ENTITIES_IN_COLLISION ];
	eventData.GetParameterOrDie( STRING_1stEntity, entitiesInvolvedInCollision[0] );
	eventData.GetParameterOrDie( STRING_2ndEntity, entitiesInvolvedInCollision[1] );

	ScoringComponent* entityScoringComponent = entitiesInvolvedInCollision[0]->FindAttachedComponentOfType< ScoringComponent >();
	if( entityScoringComponent != nullptr )
		entityScoringComponent->lastCollidedEntity = entitiesInvolvedInCollision[1];

	entityScoringComponent = entitiesInvolvedInCollision[1]->FindAttachedComponentOfType< ScoringComponent >();
	if( entityScoringComponent != nullptr )
		entityScoringComponent->lastCollidedEntity = entitiesInvolvedInCollision[0];
}

//-----------------------------------------------------------------------------------------------
void ScoringSystem::OnEntityDestruction( EventDataBundle& eventData )
{
	Entity* entityBeingDestroyed = nullptr;
	eventData.GetParameterOrDie( STRING_1stEntity, entityBeingDestroyed );

	ScoringComponent* entityScoringComponent = entityBeingDestroyed->FindAttachedComponentOfType< ScoringComponent >();
	if( entityScoringComponent != nullptr && entityScoringComponent->lastCollidedEntity != nullptr )
	{
		// The entity loses points for dying (taken out because it's not fun and font can't handle negative numbers)
		//entityScoringComponent->currentScore -= entityScoringComponent->pointValue;


		//Then passes points to its killer as well as anyone who created its killer
		unsigned int scoreIncrease = entityScoringComponent->pointValue;

		Entity*& opposingEntity = entityScoringComponent->lastCollidedEntity;
		while( opposingEntity != nullptr )
		{
			ScoringComponent* opposingEntityScoring = opposingEntity->FindAttachedComponentOfType< ScoringComponent >();
			if( opposingEntityScoring != nullptr )
				opposingEntityScoring->currentScore += scoreIncrease;

			opposingEntity = opposingEntity->creator;
		}
	}
}
#pragma endregion //Event Functions
