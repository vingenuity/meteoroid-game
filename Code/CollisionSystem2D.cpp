#include "CollisionSystem2D.hpp"

#include <Code/DebuggerInterface.hpp>

#include "CollisionComponent2D.hpp"
#include "GameEvents.hpp"


#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void CollisionSystem2D::OnAttachment( SystemManager* /*manager*/ )
{
	EventCourier::SubscribeForEvent( EVENT_Collision, EventObserver::GenerateFromOneArgFunction< CollisionSystem2D, &CollisionSystem2D::OnCollisionEvent >( this ) );
}

//-----------------------------------------------------------------------------------------------
void CollisionSystem2D::OnEndFrame()
{
	for( unsigned int i = 0; i < m_collisionComponents.size(); ++i )
	{
		if( m_collisionComponents[ i ]->IsReadyForDeletion() )
		{
			delete m_collisionComponents[ i ];
			m_collisionComponents.erase( m_collisionComponents.begin() + i );
		}
	}
}

//-----------------------------------------------------------------------------------------------
void CollisionSystem2D::OnUpdate( float /*deltaSeconds*/ )
{
	for( unsigned int i = 0; i < m_collisionComponents.size(); ++i )
	{
		CollisionComponent2D*& firstCollider = m_collisionComponents[i];
		Entity*& firstEntity = firstCollider->owner;

		//TODO: show debug information using the debug renderer

		for( unsigned int j = i + 1; j < m_collisionComponents.size(); ++j )
		{
			CollisionComponent2D*& secondCollider = m_collisionComponents[j];
			Entity*& secondEntity = secondCollider->owner;

			if( secondCollider->group == firstCollider->group )
				continue;

			float xDistance = ( firstEntity->position.x + firstCollider->collider.circle.centerX ) - ( secondEntity->position.x + secondCollider->collider.circle.centerX );
			float yDistance = ( firstEntity->position.y + firstCollider->collider.circle.centerY ) - ( secondEntity->position.y + secondCollider->collider.circle.centerY );
			float squaredDistance = ( xDistance * xDistance ) + ( yDistance * yDistance );
			float combinedColliderRadius = firstCollider->collider.circle.radius + secondCollider->collider.circle.radius;

			if( squaredDistance < ( combinedColliderRadius * combinedColliderRadius ) )
			{
				EventDataBundle collisionData;
				EventCourier::SendEvent( EVENT_Collision, collisionData );
			}
		}

		//Wrap the entity if they are offscreen
		if( ( firstEntity->position.x - firstCollider->collider.circle.radius ) > 1280.f )
			firstEntity->position.x = 0.f - firstCollider->collider.circle.radius;
		else if( ( firstEntity->position.x + firstCollider->collider.circle.radius ) < 0.f )
			firstEntity->position.x = 1280.f + firstCollider->collider.circle.radius;

		if( ( firstEntity->position.y - firstCollider->collider.circle.radius ) > 720.f )
			firstEntity->position.y = 0.f - firstCollider->collider.circle.radius;
		else if( ( firstEntity->position.y + firstCollider->collider.circle.radius ) < 0.f )
			firstEntity->position.y = 720.f + firstCollider->collider.circle.radius;
	}
}

//-----------------------------------------------------------------------------------------------
void CollisionSystem2D::OnDestruction()
{
	for( unsigned int i = 0; i < m_collisionComponents.size(); ++i )
	{
		delete m_collisionComponents[ i ];
	}
	m_collisionComponents.clear();
}
#pragma endregion //Lifecycle

//-----------------------------------------------------------------------------------------------
void CollisionSystem2D::OnCollisionEvent( EventDataBundle& eventData )
{
	//Warn if strange event name received?

	WriteToDebuggerOutput( "Collision Event Received!" );
}
