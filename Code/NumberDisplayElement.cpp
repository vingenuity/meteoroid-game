#include "NumberDisplayElement.hpp"

#include <Code/Font/BitmapFont.hpp>
#include <Code/Graphics/MeshGenerationText.hpp>
#include <Code/Graphics/RendererInterface.hpp>
#include <Code/StringConversion.hpp>


//-----------------------------------------------------------------------------------------------
NumberDisplayElement::NumberDisplayElement( const int* numberToDisplay, 
	unsigned int maxDigitsToDisplay, BitmapFont* font, Material* material, bool hidePrefixZeroes )
	: m_numberToDisplay( numberToDisplay )
	, m_lastValueOfNumber( *m_numberToDisplay )
	, m_numDigitsToDisplay( maxDigitsToDisplay )
	, m_hidePrefixZeroes( hidePrefixZeroes )
	, m_displayedVertices( new VertexData() )
	, m_font( font )
	, m_material( material )
{
	RendererInterface::GenerateBuffer( 1, &m_displayedVertices->bufferID );
	UpdateDisplayedVertices( *m_displayedVertices, *m_numberToDisplay );
}

#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void NumberDisplayElement::Render() const
{
	static const FloatVector3 X_AXIS( 1.f, 0.f, 0.f );
	static const FloatVector3 Y_AXIS( 0.f, 1.f, 0.f );
	static const FloatVector3 Z_AXIS( 0.f, 0.f, 1.f );

	RendererInterface::PushMatrix();
	RendererInterface::TranslateWorld( position );

	RendererInterface::BindVertexData( m_displayedVertices );
	RendererInterface::ApplyMaterial( m_material );

	RendererInterface::RenderVertexArray( m_displayedVertices->shape, 0, m_displayedVertices->numberOfVertices );

	RendererInterface::RemoveMaterial( m_material );
	RendererInterface::UnbindVertexData( m_displayedVertices );

	RendererInterface::PopMatrix();
}

//-----------------------------------------------------------------------------------------------
void NumberDisplayElement::Update( float /*deltaSeconds*/ )
{
	if( m_lastValueOfNumber == *m_numberToDisplay )
		return;

	UpdateDisplayedVertices( *m_displayedVertices, *m_numberToDisplay );
	m_lastValueOfNumber = *m_numberToDisplay;
}

//-----------------------------------------------------------------------------------------------
void NumberDisplayElement::PreDestruction()
{
	//delete m_font;
	delete m_displayedVertices;
}
#pragma endregion //Lifecycle



//-----------------------------------------------------------------------------------------------
void NumberDisplayElement::UpdateDisplayedVertices( VertexData& displayedVertices, 
	const unsigned int& numberToDisplay )
{
	std::string numberAsString = ConvertIntegerToString( numberToDisplay );

	//If we're too big, cut off the most significant digits
	if( numberAsString.length() > m_numDigitsToDisplay )
		numberAsString.erase( 0, numberAsString.length() - m_numDigitsToDisplay );

	if( !m_hidePrefixZeroes )
		numberAsString.insert( 0, m_numDigitsToDisplay - numberAsString.length(), '0' );

	GenerateTextMesh( displayedVertices, numberAsString, FloatVector2( 0.f, 0.f ), Color( 255, 255, 255 ), m_font, 50 );
	RendererInterface::BufferVertexData( m_displayedVertices );
}
