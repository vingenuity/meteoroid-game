#include "UISystem.hpp"

#include <Code/Graphics/RendererInterface.hpp>

#include "UIElement.hpp"


#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void UISystem::OnRender() const
{
	RendererInterface::DisableFeature( RendererInterface::DEPTH_TESTING );
	RendererInterface::DisableDepthBufferWriting();
	RendererInterface::SetViewMatrixToIdentity();
	RendererInterface::SetOrthographicProjection( 0.0, 1920.0, 0.0, 1080.0, 0.0, 1.0 );

	for( unsigned int i = 0; i < m_connectedUIElements.size(); ++i )
	{
		m_connectedUIElements[ i ]->Render();
	}

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
