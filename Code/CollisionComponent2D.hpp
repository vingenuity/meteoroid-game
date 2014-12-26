#pragma once
#ifndef INCLUDED_COLLISION_COMPONENT_2D
#define INCLUDED_COLLISION_COMPONENT_2D

//-----------------------------------------------------------------------------------------------
#include <Code/Component.hpp>


#pragma region Internal Collider Structures
//-----------------------------------------------------------------------------------------------
struct AABBCollider
{
	float lowerLeftCornerX;
	float lowerLeftCornerY;

	float upperRightCornerX;
	float upperRightCornerY;
};

//-----------------------------------------------------------------------------------------------
struct CircleCollider
{
	float centerX;
	float centerY;

	float radius;
};
#pragma endregion //Internal Collider Structures



//-----------------------------------------------------------------------------------------------
struct CollisionComponent2D : public Component
{
	//Internal Data Types
	union ColliderData
	{
		AABBCollider axisAlignedBox;	//16 bytes
		CircleCollider circle;			//12 bytes
	};

	typedef unsigned char ColliderGroup;
	static const ColliderGroup GROUP_None = 0;

	typedef unsigned char ColliderShape;
	static const ColliderShape COLLIDER_None = 0;
	static const ColliderShape COLLIDER_Circle = 1;
	static const ColliderShape COLLIDER_AABB = 2;


	//Constructors
	CollisionComponent2D( Entity* owningEntity );
	CollisionComponent2D( Entity* owningEntity, const FloatVector2& center, float radius );
	CollisionComponent2D( Entity* owningEntity, const FloatVector2& lowerLeftCorner, 
												const FloatVector2& upperRightCorner );


	//Data Members
	ColliderData collider;
	ColliderGroup group;
	ColliderShape colliderShape;
};



#pragma region Constructors
//-----------------------------------------------------------------------------------------------
inline CollisionComponent2D::CollisionComponent2D( Entity* owningEntity )
	: Component( owningEntity )
	, group( GROUP_None )
	, colliderShape( COLLIDER_None )
{ }

//-----------------------------------------------------------------------------------------------
inline CollisionComponent2D::CollisionComponent2D( Entity* owningEntity,
													const FloatVector2& center, float radius )
	: Component( owningEntity )
	, group( GROUP_None )
	, colliderShape( COLLIDER_Circle )
{
	collider.circle.centerX = center.x;
	collider.circle.centerY = center.y;
	collider.circle.radius = radius;
}

//-----------------------------------------------------------------------------------------------
inline CollisionComponent2D::CollisionComponent2D( Entity* owningEntity,
												   const FloatVector2& lowerLeftCorner,
												   const FloatVector2& upperRightCorner )
	: Component( owningEntity )
	, group( GROUP_None )
	, colliderShape( COLLIDER_AABB )
{
	collider.axisAlignedBox.lowerLeftCornerX  = lowerLeftCorner.x;
	collider.axisAlignedBox.lowerLeftCornerY  = lowerLeftCorner.y;
	collider.axisAlignedBox.upperRightCornerX = upperRightCorner.x;
	collider.axisAlignedBox.upperRightCornerY = upperRightCorner.y;
}
#pragma endregion //Constructors

#endif //INCLUDED_COLLISION_COMPONENT_2D
