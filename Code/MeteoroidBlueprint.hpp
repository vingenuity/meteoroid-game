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
	MeteoroidBlueprint();
	~MeteoroidBlueprint();

	void BuildEntityIntoGame( Entity& out_entity, const MeteoroidGame* game,
		const FloatVector2& atPosition );

private:
	void BuildMeteoroidVertexData();

	//Data Members
	Material* m_material;
	VertexData m_vertices;
};
#endif //INCLUDED_METEOROID_BLUEPRINT_HPP
