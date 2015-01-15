#pragma once
#ifndef INCLUDED_RENDERING_SYSTEM_2D_HPP
#define INCLUDED_RENDERING_SYSTEM_2D_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Graphics/MeshComponent.hpp>
#include <Code/CameraComponent.hpp>
#include <Code/ComponentSystem.hpp>


//-----------------------------------------------------------------------------------------------
//We protect the component system inheritance because acquisition is gonna be ambiguous otherwise!
class RenderingSystem2D : protected ComponentSystem< CameraComponent >, ComponentSystem< MeshComponent >
{
public:
	RenderingSystem2D( size_t maxCameraComponents, size_t maxMeshComponents );

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ ) { }
	void OnEndFrame();
	void OnRender() const;
	void OnUpdate( float deltaSeconds );
	void OnDestruction();

	//Acquisition
	CameraComponent* AcquireCameraComponent() {  return ComponentSystem< CameraComponent >::AcquireComponent(); }
	void RelinquishCameraComponent( CameraComponent* camera ) { ComponentSystem< CameraComponent >::RelinquishComponent( camera ); }

	MeshComponent* AcquireMeshComponent() {  return ComponentSystem< MeshComponent >::AcquireComponent(); }
	void RelinquishMeshComponent( MeshComponent* mesh ) { ComponentSystem< MeshComponent >::RelinquishComponent( mesh ); }

	void SetActiveCamera( CameraComponent* camera ) { m_activeCamera = camera; }


private:
	//Private Helper
	void ViewWorldThroughCamera( const CameraComponent* camera ) const;

	CameraComponent* m_activeCamera;
};



//-----------------------------------------------------------------------------------------------
inline RenderingSystem2D::RenderingSystem2D( size_t maxCameraComponents, size_t maxMeshComponents )
	: ComponentSystem< CameraComponent >( maxCameraComponents )
	, ComponentSystem< MeshComponent >( maxMeshComponents )
{ }

#endif //INCLUDED_RENDERING_SYSTEM_2D_HPP