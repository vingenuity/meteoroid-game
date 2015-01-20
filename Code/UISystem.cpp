#include "UISystem.hpp"

#include <Code/Graphics/Renderer.hpp>

#include "UIElement.hpp"


#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void UISystem::OnRender() const
{
	Renderer* renderer = Renderer::GetRenderer();

	renderer->DisableFeature( Renderer::DEPTH_TESTING );
	renderer->DisableDepthBufferWriting();
	renderer->SetViewMatrixToIdentity();
	renderer->SetOrthographicProjection( 0.0, 1920.0, 0.0, 1080.0, 0.0, 1.0 );

	for( unsigned int i = 0; i < m_connectedUIElements.size(); ++i )
	{
		m_connectedUIElements[ i ]->Render();
	}

	renderer->EnableDepthBufferWriting();
	renderer->EnableFeature( Renderer::DEPTH_TESTING );
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
