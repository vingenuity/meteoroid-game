#pragma once
#ifndef SCORE_UI_SYSTEM_HPP
#define SCORE_UI_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include <Code/Math/FloatVector2.hpp>
#include <Code/Math/IntVector2.hpp>
#include <Code/System.hpp>

struct UIElement;


//-----------------------------------------------------------------------------------------------
class UISystem : public System
{
public:
	static const FloatVector2 UI_LAYOUT_DIMENSIONS;

	UISystem( IntVector2 windowDimensions )
	: m_windowDimensions( static_cast< float>( windowDimensions.x ), static_cast< float>( windowDimensions.y ) )
	{ }

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ ) { }
	void OnEndFrame() { }
	void OnRender() const;
	void OnUpdate( float deltaSeconds );
	void OnDestruction();

	void ConnectUIElement( UIElement* element ) { m_connectedUIElements.push_back( element ); }

private:
	FloatVector2 m_windowDimensions;
	std::vector< UIElement* > m_connectedUIElements;
};
#endif //SCORE_UI_SYSTEM_HPP
