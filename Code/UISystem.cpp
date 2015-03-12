#include "UISystem.hpp"

#include <Code/Graphics/RendererInterface.hpp>

#include "UIElement.hpp"


STATIC const FloatVector2 UISystem::UI_LAYOUT_DIMENSIONS( 800.f, 800.f );

#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void UISystem::OnRender() const
{
	RendererInterface::DisableFeature( RendererInterface::DEPTH_TESTING );
	RendererInterface::DisableDepthBufferWriting();
	RendererInterface::SetViewMatrixToIdentity();
	RendererInterface::SetOrthographicProjection( 0.0, UI_LAYOUT_DIMENSIONS.x, 0.0, UI_LAYOUT_DIMENSIONS.y, 0.0, 1.0 );

	RendererInterface::PushMatrix();

	for( unsigned int i = 0; i < m_connectedUIElements.size(); ++i )
	{
		UIElement* const& uiElement = m_connectedUIElements[ i ];

		if( uiElement->isVisible )
			uiElement->Render();
	}
	RendererInterface::PopMatrix();

	RendererInterface::EnableDepthBufferWriting();
	RendererInterface::EnableFeature( RendererInterface::DEPTH_TESTING );
}

//-----------------------------------------------------------------------------------------------
void UISystem::OnUpdate( float deltaSeconds )
{
	for( unsigned int i = 0; i < m_connectedUIElements.size(); ++i )
	{
		m_connectedUIElements[ i ]->Update( deltaSeconds );
	}
}

//-----------------------------------------------------------------------------------------------
void UISystem::OnDestruction()
{
	for( unsigned int i = 0; i < m_connectedUIElements.size(); ++i )
	{
		m_connectedUIElements[ i ]->PreDestruction();
		delete m_connectedUIElements[ i ];
	}
	m_connectedUIElements.clear();
}
#pragma endregion //Lifecycle
