#pragma once
#ifndef INCLUDED_MISSILE_BLUEPRINT_HPP
#define INCLUDED_MISSILE_BLUEPRINT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Graphics/VertexData.hpp>
#include <Code/EntityBlueprint.hpp>

class MeteoroidGame;


//-----------------------------------------------------------------------------------------------
class MissileBlueprint : public EntityBlueprint
{
	static const unsigned int NUM_MISSILE_VERTICES = 2;

public:
	MissileBlueprint( MeteoroidGame* const game );
	virtual ~MissileBlueprint();

	void BuildEntity( Entity& out_entity );


private:
	void BuildMissileVertexData();

	//Data Members
	MeteoroidGame* m_game;

	Material* m_material;
	VertexData m_vertices;

	FloatVector2 m_colliderCenter;
	float m_colliderRadius;
};

#endif //INCLUDED_MISSILE_BLUEPRINT_HPP