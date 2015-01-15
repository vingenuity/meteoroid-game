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


	CollisionComponent2D();


	//Helpers
	void SetColliderToAABB(const FloatVector2& lowerLeftCorner, const FloatVector2& upperRightCorner );
	void SetColliderToCircle( const FloatVector2& center, float radius );


	//Data Members
	ColliderData collider;
	ColliderGroup group;
	ColliderShape colliderShape;
};



//-----------------------------------------------------------------------------------------------
inline CollisionComponent2D::CollisionComponent2D()
	: Component()
	, group( GROUP_None )
	, colliderShape( COLLIDER_None )
{ }



#pragma region Helpers
//-----------------------------------------------------------------------------------------------
inline void CollisionComponent2D::SetColliderToAABB(const FloatVector2& lowerLeftCorner, const FloatVector2& upperRightCorner )
{
	colliderShape = COLLIDER_AABB;

	collider.axisAlignedBox.lowerLeftCornerX  = lowerLeftCorner.x;
	collider.axisAlignedBox.lowerLeftCornerY  = lowerLeftCorner.y;
	collider.axisAlignedBox.upperRightCornerX = upperRightCorner.x;
	collider.axisAlignedBox.upperRightCornerY = upperRightCorner.y;
}

//-----------------------------------------------------------------------------------------------
inline void CollisionComponent2D::SetColliderToCircle( const FloatVector2& center, float radius )
{
	colliderShape = COLLIDER_Circle;

	collider.circle.centerX = center.x;
	collider.circle.centerY = center.y;
	collider.circle.radius = radius;
}
#pragma endregion //Helpers

#endif //INCLUDED_COLLISION_COMPONENT_2D
