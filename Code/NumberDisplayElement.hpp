#pragma once
#ifndef INCLUDED_NUMBER_DISPLAY_ELEMENT_HPP
#define INCLUDED_NUMBER_DISPLAY_ELEMENT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Graphics/VertexData.hpp>

#include "UIElement.hpp"

struct BitmapFont;
struct Material;


//-----------------------------------------------------------------------------------------------
struct NumberDisplayElement : public UIElement
{
	NumberDisplayElement( const int* numberToDisplay, unsigned int maxDigitsToDisplay, 
		BitmapFont* font, unsigned int fontHeightPixels, Material* material, bool hidePrefixZeroes = true );

	NumberDisplayElement( const NumberDisplayElement& other );

	void Render() const;
	void Update( float deltaSeconds );
	void PreDestruction();

	
private:
	//There's no reason we couldn't have an assignment for this class...
	//...I am just not sure how to write it with the const reference.
	NumberDisplayElement& operator=( const NumberDisplayElement& other );

	void UpdateDisplayedVertices( VertexData& displayedVertices, const int& numberToDisplay, unsigned int fontHeightPixels );

	//Data Members
	const int*		m_numberToDisplay;
	int				m_lastValueOfNumber;
	unsigned int	m_numDigitsToDisplay;
	bool			m_hidePrefixZeroes;
	VertexData*		m_displayedVertices;
	BitmapFont*		m_font;
	unsigned int	m_fontHeightPixels;
	Material*		m_material;
};



//-----------------------------------------------------------------------------------------------
inline NumberDisplayElement::NumberDisplayElement( const NumberDisplayElement& other )
	: m_numberToDisplay( other.m_numberToDisplay )
	, m_lastValueOfNumber( *m_numberToDisplay )
	, m_numDigitsToDisplay( other.m_numDigitsToDisplay )
	, m_hidePrefixZeroes( other.m_hidePrefixZeroes )
	, m_font( other.m_font )
	, m_fontHeightPixels( other.m_fontHeightPixels )
	, m_material( other.m_material )
{
	UpdateDisplayedVertices( *m_displayedVertices, *m_numberToDisplay, m_fontHeightPixels );
}
#endif //INCLUDED_NUMBER_DISPLAY_ELEMENT_HPP
