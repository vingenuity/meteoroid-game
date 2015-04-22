#pragma once
#ifndef INCLUDED_SHIP_BLUEPRINT_HPP
#define INCLUDED_SHIP_BLUEPRINT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Graphics/VertexData.hpp>
#include <Code/Entity.hpp>

struct Material;
class MeteoroidGame;


//-----------------------------------------------------------------------------------------------
class ShipBlueprint
{
	static const unsigned char SHIP_COLLISION_GROUP = 1;
	static const unsigned int NUM_SHIP_VERTICES = 10;

public:
	ShipBlueprint();
	~ShipBlueprint();

	void BuildEntityIntoGame( Entity& out_entity, const MeteoroidGame* game,
								const FloatVector2& atPosition );

private:
	void BuildShipVertexData();

	//Data Members
	Material* m_material;
	VertexData m_vertices;

	FloatVector2 m_colliderCenter;
	float m_colliderRadius;
};
#endif //INCLUDED_SHIP_BLUEPRINT_HPP
