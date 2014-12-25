#pragma once
#ifndef INCLUDED_METEOROID_BLUEPRINT_HPP
#define INCLUDED_METEOROID_BLUEPRINT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Graphics/VertexData.hpp>
#include <Code/Entity.hpp>

struct Material;
class MeteoroidGame;


//-----------------------------------------------------------------------------------------------
class MeteoroidBlueprint
{
public:
	static const unsigned int NUM_METEOROID_SIZES = 3;
	static const float MAX_METEOROID_SPEED;
	static const float MAX_METEOROID_ANGULAR_SPEED;

	MeteoroidBlueprint();
	~MeteoroidBlueprint();

	void BuildEntityIntoGame( Entity& out_entity, const MeteoroidGame* game,
		const FloatVector2& atPosition );


private:
	void BuildMeteoroidVertexData( VertexData& out_vertData, unsigned int meteorSize );

	//Data Members
	Material* m_material;

	FloatVector2 m_colliderCenter;
};
#endif //INCLUDED_METEOROID_BLUEPRINT_HPP
