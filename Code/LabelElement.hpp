#pragma once
#ifndef INCLUDED_LABEL_ELEMENT_HPP
#define INCLUDED_LABEL_ELEMENT_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Graphics/VertexData.hpp>

#include "UIElement.hpp"

struct BitmapFont;
struct Material;


//-----------------------------------------------------------------------------------------------
struct LabelElement : public UIElement
{
	LabelElement( const char* displayedString, BitmapFont* font, unsigned int fontHeightPixels, Material* material );

	void Render() const;
	void Update( float deltaSeconds );
	void PreDestruction();

private:
	void UpdateDisplayedVertices( VertexData& displayedVertices, const char* stringToDisplay, unsigned int fontHeightPixels );

	//Data Members
	const char*			m_displayedString;
	VertexData*			m_displayedVertices;
	BitmapFont*			m_font;
	unsigned int		m_fontHeightPixels;
	Material*			m_material;
};

#endif //INCLUDED_LABEL_ELEMENT_HPP
