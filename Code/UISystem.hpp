#pragma once
#ifndef SCORE_UI_SYSTEM_HPP
#define SCORE_UI_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include <Code/System.hpp>

struct UIElement;


//-----------------------------------------------------------------------------------------------
class UISystem : public System
{
public:
	UISystem() { }

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ ) { }
	void OnEndFrame() { }
	void OnRender() const;
	void OnUpdate( float deltaSeconds );
	void OnDestruction();

	void ConnectUIElement( UIElement* element ) { m_connectedUIElements.push_back( element ); }

private:
	std::vector< UIElement* > m_connectedUIElements;
};
#endif //SCORE_UI_SYSTEM_HPP
