#include "LabelElement.hpp"

#include <Code/Font/BitmapFont.hpp>
#include <Code/Graphics/Material.hpp>
#include <Code/Graphics/MeshGenerationText.hpp>
#include <Code/Graphics/RendererInterface.hpp>


//-----------------------------------------------------------------------------------------------
LabelElement::LabelElement( const char* displayedString, BitmapFont* font, unsigned int fontHeightPixels, Material* material )
	: m_displayedString( displayedString )
	, m_displayedVertices( new VertexData() )
	, m_font( font )
	, m_fontHeightPixels( fontHeightPixels )
	, m_material( material )
{
	RendererInterface::GenerateBuffer( 1, &m_displayedVertices->bufferID );
	UpdateDisplayedVertices( *m_displayedVertices, m_displayedString, m_fontHeightPixels );
}

#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void LabelElement::Render() const
{
	static const FloatVector3 X_AXIS( 1.f, 0.f, 0.f );
	static const FloatVector3 Y_AXIS( 0.f, 1.f, 0.f );
	static const FloatVector3 Z_AXIS( 0.f, 0.f, 1.f );

	RendererInterface::PushMatrix();
	RendererInterface::TranslateWorld( position );

	RendererInterface::ApplyMaterial( m_material );
	RendererInterface::BindVertexDataToShader( m_displayedVertices, m_material->pipeline );

	RendererInterface::RenderVertexArray( m_displayedVertices->shape, 0, m_displayedVertices->numberOfVertices );

	RendererInterface::UnbindVertexDataFromShader( m_displayedVertices, m_material->pipeline );
	RendererInterface::RemoveMaterial( m_material );

	RendererInterface::PopMatrix();
}

//-----------------------------------------------------------------------------------------------
void LabelElement::Update( float /*deltaSeconds*/ )
{
}

//-----------------------------------------------------------------------------------------------
void LabelElement::PreDestruction()
{
	//delete m_font;
	delete m_displayedVertices;
}
#pragma endregion //Lifecycle



//-----------------------------------------------------------------------------------------------
void LabelElement::UpdateDisplayedVertices( VertexData& displayedVertices,
	const char* stringToDisplay, unsigned int fontHeightPixels )
{
	width = CalculateTextWidth( stringToDisplay, m_font, (float)fontHeightPixels );
	height = (float)fontHeightPixels;

	GenerateTextMesh( displayedVertices, stringToDisplay, FloatVector2( 0.f, 0.f ), Color( 255, 255, 255 ), m_font, (float)fontHeightPixels );
	RendererInterface::BufferVertexData( &displayedVertices );
}
