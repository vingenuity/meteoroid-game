#pragma once
#ifndef INCLUDED_FRAME_ELEMENT_HPP
#define INCLUDED_FRAME_ELEMENT_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include <Code/Graphics/VertexData.hpp>

#include "UIElement.hpp"

struct Material;


//-----------------------------------------------------------------------------------------------
struct FrameElement : public UIElement
{
	FrameElement( Material* material, bool drawFrame = false );

	void Render() const;
	void Update( float /*deltaSeconds*/ ) { }
	void PreDestruction();

	void InsertUIElement( UIElement* element );

private:
	void UpdateDisplayedVertices( VertexData& frameVertices, VertexData& paneVertices, bool drawFrame, float frameWidth, float frameHeight );

	//Data Members
	std::vector< UIElement* >	m_containedUIElements;
	VertexData*					m_frameVertices;
	VertexData*					m_paneVertices;
	Material*					m_material;
};

#endif //INCLUDED_FRAME_ELEMENT_HPP
