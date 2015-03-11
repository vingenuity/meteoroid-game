#pragma once
#ifndef INCLUDED_UI_ELEMENT_HPP
#define INCLUDED_UI_ELEMENT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Math/FloatVector2.hpp>


//-----------------------------------------------------------------------------------------------
struct UIElement
{
	UIElement( bool visible = true );
	virtual ~UIElement() { }

	virtual void Render() const = 0;
	virtual void Update( float deltaSeconds ) = 0;
	virtual void PreDestruction() { }


	//Data Members
	bool isVisible;
	FloatVector2 position;
};

//-----------------------------------------------------------------------------------------------
inline UIElement::UIElement( bool visible )
	: isVisible( visible )
{ }
#endif //INCLUDED_UI_ELEMENT_HPP
