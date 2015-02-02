#include "CollisionSystem2D.hpp"

#include <Code/DebuggerInterface.hpp>

#include "CollisionComponent2D.hpp"
#include "GameEvents.hpp"


#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void CollisionSystem2D::OnAttachment( SystemManager* /*manager*/ )
{
}

//-----------------------------------------------------------------------------------------------
void CollisionSystem2D::OnEndFrame()
{
	for( unsigned int i = 0; i < m_numComponentsInPool; ++i )
	{
		if( !m_componentPool[i].readyForDeletion )
			continue;

		this->RelinquishComponent( &m_componentPool[i] );
	}
}

//-----------------------------------------------------------------------------------------------
void CollisionSystem2D::OnUpdate( float /*deltaSeconds*/ )
{
	for( unsigned int i = 0; i < m_numComponentsInPool; ++i )
	{
		CollisionComponent2D& firstCollider = m_componentPool[i];
		if( !firstCollider.IsActive() )
			continue;
		Entity*& firstEntity = firstCollider.owner;

		//Wrap the entity if they are offscreen
		if( ( firstEntity->position.x - firstCollider.collider.circle.radius ) > m_worldDimensions.x )
			firstEntity->position.x = 0.f - firstCollider.collider.circle.radius;
		else if( ( firstEntity->position.x + firstCollider.collider.circle.radius ) < 0.f )
			firstEntity->position.x = m_worldDimensions.x + firstCollider.collider.circle.radius;

		if( ( firstEntity->position.y - firstCollider.collider.circle.radius ) > m_worldDimensions.y )
			firstEntity->position.y = 0.f - firstCollider.collider.circle.radius;
		else if( ( firstEntity->position.y + firstCollider.collider.circle.radius ) < 0.f )
			firstEntity->position.y = m_worldDimensions.y + firstCollider.collider.circle.radius;

		//TODO: show debug information using the debug renderer

		for( unsigned int j = i + 1; j < m_numComponentsInPool; ++j )
		{
			CollisionComponent2D& secondCollider = m_componentPool[j];
			if( !secondCollider.IsActive() )
				continue;
			Entity*& secondEntity = secondCollider.owner;

  			if( secondCollider.group == firstCollider.group )
				continue;

			float xDistance = ( firstEntity->position.x + firstCollider.collider.circle.centerX ) - ( secondEntity->position.x + secondCollider.collider.circle.centerX );
			float yDistance = ( firstEntity->position.y + firstCollider.collider.circle.centerY ) - ( secondEntity->position.y + secondCollider.collider.circle.centerY );
			float squaredDistance = ( xDistance * xDistance ) + ( yDistance * yDistance );
			float combinedColliderRadius = firstCollider.collider.circle.radius + secondCollider.collider.circle.radius;

			if( squaredDistance < ( combinedColliderRadius * combinedColliderRadius ) )
			{
				EventDataBundle collisionData;
				collisionData.SetParameter( STRING_1stEntity, firstEntity );
				collisionData.SetParameter( STRING_2ndEntity, secondEntity );
				EventCourier::SendEvent( EVENT_Collision, collisionData );
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
void CollisionSystem2D::OnDestruction()
{
	ComponentSystem< CollisionComponent2D >::OnDestruction();
}
#pragma endregion //Lifecycle
