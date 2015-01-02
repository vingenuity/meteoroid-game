#pragma once
#ifndef INCLUDED_METEOROID_BLUEPRINT_HPP
#define INCLUDED_METEOROID_BLUEPRINT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Graphics/VertexData.hpp>
#include <Code/EntityBlueprint.hpp>

struct Material;
class MeteoroidGame;


//-----------------------------------------------------------------------------------------------
class MeteoroidBlueprint : public EntityBlueprint
{
public:
	static const unsigned int NUM_METEOROID_SIZES = 3;
	static const float MAX_METEOROID_SPEED;
	static const float MAX_METEOROID_ANGULAR_SPEED;
	static const unsigned int METEOROID_COLLISION_GROUP = 2;

	MeteoroidBlueprint( MeteoroidGame* const game );
	~MeteoroidBlueprint();

	void BuildEntity( Entity& out_entity );


	//"hints" that can be given in order to change spawn behavior
	FloatVector2 hint_spawnPosition;
	unsigned int hint_meteorSize;

private:
	VertexData* BuildMeteoroidVertexData( unsigned int meteorSize );

	//Data Members
	MeteoroidGame* m_game;
	Material* m_material;

	FloatVector2 m_colliderCenter;
};
#endif //INCLUDED_METEOROID_BLUEPRINT_HPP
