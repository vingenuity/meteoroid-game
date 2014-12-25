#include "CollisionSystem2D.hpp"

#include <Code/DebuggerInterface.hpp>
#include "CollisionComponent2D.hpp"


#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void CollisionSystem2D::OnAttachment( SystemManager* /*manager*/ )
{

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

			float xDistance = ( firstEntity->position.x + firstCollider->collider.circle.centerX ) - ( secondEntity->position.x + secondCollider->collider.circle.centerX );
			float yDistance = ( firstEntity->position.y + firstCollider->collider.circle.centerY ) - ( secondEntity->position.y + secondCollider->collider.circle.centerY );
			float squaredDistance = ( xDistance * xDistance ) + ( yDistance * yDistance );
			float combinedColliderRadius = firstCollider->collider.circle.radius + secondCollider->collider.circle.radius;

			if( squaredDistance < ( combinedColliderRadius * combinedColliderRadius ) && i == 0 )
			{
				WriteToDebuggerOutput( "COLLISION!" );
			}
		}
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
