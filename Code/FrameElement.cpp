#include "FrameElement.hpp"

#include <Code/Graphics/Material.hpp>
#include <Code/Graphics/Mesh2DGeneration.hpp>
#include <Code/Graphics/RendererInterface.hpp>

//-----------------------------------------------------------------------------------------------
STATIC const FloatVector2 FrameElement::FRAME_PADDING( 5.f, 5.f );

//-----------------------------------------------------------------------------------------------
FrameElement::FrameElement( Material* material, const Color& frameColor, const Color& paneColor )
	: m_frameColor( frameColor )
	, m_frameVertices( new VertexData() )
	, m_paneColor( paneColor )
	, m_paneVertices( new VertexData() )
	, m_material( material )
{
	RendererInterface::GenerateBuffer( 1, &m_frameVertices->bufferID );
	RendererInterface::GenerateBuffer( 1, &m_paneVertices->bufferID );
	UpdateDisplayedVertices( *m_frameVertices, *m_paneVertices, m_containedUIElements, m_frameColor, m_paneColor );
}

#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void FrameElement::Render() const
{
	static const FloatVector3 X_AXIS( 1.f, 0.f, 0.f );
	static const FloatVector3 Y_AXIS( 0.f, 1.f, 0.f );
	static const FloatVector3 Z_AXIS( 0.f, 0.f, 1.f );

	RendererInterface::PushMatrix();
	RendererInterface::TranslateWorld( position + FRAME_PADDING );

	for( unsigned int i = 0; i < m_containedUIElements.size(); ++i )
	{
		UIElement* const& uiElement = m_containedUIElements[ i ];

		if( uiElement->isVisible )
			uiElement->Render();
	}

	RendererInterface::TranslateWorld( -FloatVector2( FRAME_PADDING ) );

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
void FrameElement::Update( float deltaSeconds )
{
	for( unsigned int i = 0; i < m_containedUIElements.size(); ++i )
	{
		m_containedUIElements[ i ]->Update( deltaSeconds );
	}
}

//-----------------------------------------------------------------------------------------------
void FrameElement::PreDestruction()
{
	//delete m_font;
	for( unsigned int i = 0; i < m_containedUIElements.size(); ++i )
	{
		m_containedUIElements[ i ]->PreDestruction();
		delete m_containedUIElements[ i ];
	}

	delete m_frameVertices;
	delete m_paneVertices;
}
#pragma endregion //Lifecycle



//-----------------------------------------------------------------------------------------------
void FrameElement::InsertUIElement( UIElement* element )
{
	m_containedUIElements.push_back( element );
	UpdateDisplayedVertices( *m_frameVertices, *m_paneVertices, m_containedUIElements, m_frameColor, m_paneColor );
}

//-----------------------------------------------------------------------------------------------
void FrameElement::UpdateDisplayedVertices( VertexData& frameVertices, VertexData& paneVertices,
	std::vector< UIElement* >& containedUIElements,
	const Color& frameColor, const Color& paneColor )
{
	static const FloatVector2 PANE_LOWER_LEFT_CORNER( 0.f, 0.f );

	FloatVector2 containedElementsMinExtent( 0.f, 0.f );
	FloatVector2 containedElementsMaxExtent( 0.f, 0.f );
	for( unsigned int i = 0; i < containedUIElements.size(); ++i )
	{
		const UIElement& uiElement = *containedUIElements[ i ];

		containedElementsMinExtent.x = std::min( containedElementsMinExtent.x, uiElement.position.x - FRAME_PADDING.x );
		containedElementsMaxExtent.x = std::max( containedElementsMaxExtent.x, uiElement.position.x + uiElement.width + FRAME_PADDING.x );

		containedElementsMinExtent.y = std::min( containedElementsMinExtent.y, uiElement.position.y - FRAME_PADDING.y );
		containedElementsMaxExtent.y = std::max( containedElementsMaxExtent.y, uiElement.position.y + uiElement.height + FRAME_PADDING.y );
	}

	width = containedElementsMaxExtent.x - containedElementsMinExtent.x;
	height = containedElementsMaxExtent.y - containedElementsMinExtent.y;

	Generate2DRectangleOutline( frameVertices, PANE_LOWER_LEFT_CORNER, width, height,
		frameColor );
	RendererInterface::BufferVertexData( &frameVertices );

	Generate2DRectangleFilled( paneVertices, PANE_LOWER_LEFT_CORNER, width, height,
		paneColor );
	RendererInterface::BufferVertexData( &paneVertices );
}
