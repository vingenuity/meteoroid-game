#include "FrameElement.hpp"

#include <Code/Graphics/Material.hpp>
#include <Code/Graphics/Mesh2DGeneration.hpp>
#include <Code/Graphics/RendererInterface.hpp>


//-----------------------------------------------------------------------------------------------
FrameElement::FrameElement( Material* material, bool drawFrame )
	: m_frameVertices( new VertexData() )
	, m_paneVertices( new VertexData() )
	, m_material( material )
{
	RendererInterface::GenerateBuffer( 1, &m_frameVertices->bufferID );
	RendererInterface::GenerateBuffer( 1, &m_paneVertices->bufferID );
	UpdateDisplayedVertices( *m_frameVertices, *m_paneVertices, drawFrame, 100.f, 100.f );
}

#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void FrameElement::Render() const
{
	static const FloatVector3 X_AXIS( 1.f, 0.f, 0.f );
	static const FloatVector3 Y_AXIS( 0.f, 1.f, 0.f );
	static const FloatVector3 Z_AXIS( 0.f, 0.f, 1.f );

	RendererInterface::PushMatrix();
	RendererInterface::TranslateWorld( position );

	for( unsigned int i = 0; i < m_containedUIElements.size(); ++i )
	{
		UIElement* const& uiElement = m_containedUIElements[ i ];

		if( uiElement->isVisible )
			uiElement->Render();
	}

	RendererInterface::ApplyMaterial( m_material );

	RendererInterface::BindVertexDataToShader( m_paneVertices, m_material->pipeline );
	RendererInterface::RenderVertexArray( m_paneVertices->shape, 0, m_paneVertices->numberOfVertices );
	RendererInterface::UnbindVertexDataFromShader( m_paneVertices, m_material->pipeline );

	RendererInterface::BindVertexDataToShader( m_frameVertices, m_material->pipeline );
	RendererInterface::RenderVertexArray( m_frameVertices->shape, 0, m_frameVertices->numberOfVertices );
	RendererInterface::UnbindVertexDataFromShader( m_frameVertices, m_material->pipeline );

	RendererInterface::RemoveMaterial( m_material );

	RendererInterface::PopMatrix();
}

//-----------------------------------------------------------------------------------------------
void FrameElement::PreDestruction()
{
	//delete m_font;
	delete m_frameVertices;
	delete m_paneVertices;
}
#pragma endregion //Lifecycle



//-----------------------------------------------------------------------------------------------
void FrameElement::InsertUIElement( UIElement* element )
{
	m_containedUIElements.push_back( element );
	UpdateDisplayedVertices( *m_frameVertices, *m_paneVertices, true, 100.f, 100.f );
}

//-----------------------------------------------------------------------------------------------
void FrameElement::UpdateDisplayedVertices( VertexData& frameVertices, VertexData& paneVertices,
	bool drawFrame, float frameWidth, float frameHeight )
{
	static const FloatVector2 PANE_LOWER_LEFT_CORNER( 0.f, 0.f );

	Generate2DRectangleOutline( frameVertices, PANE_LOWER_LEFT_CORNER, frameWidth, frameHeight,
		Color( 255, 255, 255, (unsigned char)drawFrame * 255 ) );
	RendererInterface::BufferVertexData( &frameVertices );

	Generate2DRectangleFilled( paneVertices, PANE_LOWER_LEFT_CORNER, frameWidth, frameHeight,
		Color( 128, 128, 128, 128 ) );
	RendererInterface::BufferVertexData( &paneVertices );
}
