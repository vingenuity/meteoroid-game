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
	static const FloatVector2 FRAME_PADDING;

	FrameElement( Material* material, const Color& frameColor = Color( 0, 0, 0, 0 ), const Color& paneColor = Color( 0, 0, 0, 0 ) );

	void Render() const;
	void Update( float /*deltaSeconds*/ );
	void PreDestruction();

	void InsertUIElement( UIElement* element );

private:
	void UpdateDisplayedVertices( VertexData& frameVertices, VertexData& paneVertices,
		std::vector< UIElement* >& containedUIElements,
		const Color& frameColor = Color( 0, 0, 0, 0 ), const Color& paneColor = Color( 0, 0, 0, 0 ) );

	//Data Members
	std::vector< UIElement* >	m_containedUIElements;
	Color						m_frameColor;
	VertexData*					m_frameVertices;
	Color						m_paneColor;
	VertexData*					m_paneVertices;
	Material*					m_material;
};

#endif //INCLUDED_FRAME_ELEMENT_HPP
