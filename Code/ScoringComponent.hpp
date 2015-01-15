#pragma once
#ifndef INCLUDED_SCORING_COMPONENT_HPP
#define INCLUDED_SCORING_COMPONENT_HPP
//-----------------------------------------------------------------------------------------------
#include <Code/Component.hpp>

struct Entity;


//-----------------------------------------------------------------------------------------------
struct ScoringComponent : public Component
{
	ScoringComponent( unsigned int point_value = 0 );


	//Data Members
	int pointValue;
	int currentScore;
	Entity* lastCollidedEntity;
};



//-----------------------------------------------------------------------------------------------
inline ScoringComponent::ScoringComponent( unsigned int point_value )
	: pointValue( point_value )
	, currentScore( 0 )
	, lastCollidedEntity( nullptr )
{

}

#endif //INCLUDED_SCORING_COMPONENT_HPP
