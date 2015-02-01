#include "RenderingSystem2D.hpp"

#include "GameEvents.hpp"


#pragma region Lifecycle
//-----------------------------------------------------------------------------------------------
void RenderingSystem2D::OnEndFrame()
{
	for( unsigned int i = 0; i < ComponentSystem< CameraComponent >::m_numComponentsInPool; ++i )
	{
		CameraComponent& camera = ComponentSystem< CameraComponent >::m_componentPool[i];
		if( !camera.readyForDeletion )
			continue;

		ComponentSystem< CameraComponent >::RelinquishComponent( &camera );
	}

	for( unsigned int i = 0; i < ComponentSystem< MeshComponent >::m_numComponentsInPool; ++i )
	{
		MeshComponent& mesh = ComponentSystem< MeshComponent >::m_componentPool[i];
		if( !mesh.readyForDeletion )
			continue;

		ComponentSystem< MeshComponent >::RelinquishComponent( &mesh );
	}
}

//-----------------------------------------------------------------------------------------------
void RenderingSystem2D::OnRender() const
{
	if( m_activeCamera == nullptr || !m_activeCamera->IsActive() )
		return;
	ViewWorldThroughCamera( m_activeCamera );

	static const FloatVector3 X_AXIS( 1.f, 0.f, 0.f );
	static const FloatVector3 Y_AXIS( 0.f, 1.f, 0.f );
	static const FloatVector3 Z_AXIS( 0.f, 0.f, 1.f );

	for( unsigned int i = 0; i < ComponentSystem< MeshComponent >::m_numComponentsInPool; ++i )
	{
		MeshComponent& mesh = ComponentSystem< MeshComponent >::m_componentPool[i];
		if( !mesh.IsActive() )
			continue;

		RendererInterface::PushMatrix();
		RendererInterface::TranslateWorld( mesh.owner->position );
		RendererInterface::RotateWorldAboutAxisDegrees( Z_AXIS, mesh.owner->orientation.yawDegreesAboutZ );
		RendererInterface::RotateWorldAboutAxisDegrees( Y_AXIS, mesh.owner->orientation.pitchDegreesAboutY );
		RendererInterface::RotateWorldAboutAxisDegrees( X_AXIS, mesh.owner->orientation.rollDegreesAboutX );

		RendererInterface::BindVertexData( mesh.vertexData );
		RendererInterface::ApplyMaterial( mesh.material );

		RendererInterface::RenderVertexArray( mesh.vertexData->shape, 0, mesh.vertexData->numberOfVertices );

		RendererInterface::RemoveMaterial( mesh.material );
		RendererInterface::UnbindVertexData( mesh.vertexData );

		RendererInterface::PopMatrix();
	}
}

//-----------------------------------------------------------------------------------------------
void RenderingSystem2D::OnUpdate( float deltaSeconds )
{
	ComponentSystem< CameraComponent >::OnUpdate( deltaSeconds );
	ComponentSystem< MeshComponent >::OnUpdate( deltaSeconds );
}

//-----------------------------------------------------------------------------------------------
void RenderingSystem2D::OnDestruction()
{
	ComponentSystem< CameraComponent >::OnDestruction();
	ComponentSystem< MeshComponent >::OnDestruction();
}
#pragma endregion //Lifecycle



//-----------------------------------------------------------------------------------------------
void RenderingSystem2D::ViewWorldThroughCamera( const CameraComponent* camera ) const
{
	switch( camera->projectionType )
	{
	case CameraComponent::PROJECTION_PERSPECTIVE:
		{
			const PerspectiveProjection& perspective = camera->projection.perspective;
			RendererInterface::SetPerpectiveProjection( perspective.horizontalFOVDegrees, perspective.aspectRatio,
				perspective.nearClippingPlane, perspective.farClippingPlane );
			break;
		}
	case CameraComponent::PROJECTION_ORTHOGRAPHIC:
		{
			const OrthographicProjection& projection = camera->projection.orthographic;
			RendererInterface::SetOrthographicProjection( projection.leftEdgeX, projection.rightEdgeX,
				projection.bottomEdgeY, projection.topEdgeY,
				projection.nearClippingPlane, projection.farClippingPlane );
			break;
		}
	case CameraComponent::PROJECTION_NONE:
	default:
		break;
	}
	Float4x4Matrix rotationMatrix = F4X4_IDENTITY_MATRIX;
	//const EulerAngles& cameraOrientation = camera->owner->orientation;
	//Float4x4Matrix xRotation, yRotation, zRotation, x2, z2; 
	//GetRotationMatrixForAxisAndAngleDegrees( xRotation, FloatVector3( 1.f, 0.f, 0.f ), -cameraOrientation.rollDegreesAboutX );
	//GetRotationMatrixForAxisAndAngleDegrees( yRotation, FloatVector3( 0.f, 1.f, 0.f ), -cameraOrientation.pitchDegreesAboutY );
	//GetRotationMatrixForAxisAndAngleDegrees( zRotation, FloatVector3( 0.f, 0.f, 1.f ), -cameraOrientation.yawDegreesAboutZ );
	//GetRotationMatrixForAxisAndAngleDegrees( x2, FloatVector3( 1.f, 0.f, 0.f ), -90.f );
	//GetRotationMatrixForAxisAndAngleDegrees( z2, FloatVector3( 0.f, 0.f, 1.f ),  90.f );
	//rotationMatrix = zRotation * yRotation * xRotation * z2 * x2;

	Float4x4Matrix translationMatrix = F4X4_IDENTITY_MATRIX;
	const FloatVector3& cameraPosition = camera->owner->position;
	translationMatrix[ 12 ] = -cameraPosition.x;
	translationMatrix[ 13 ] = -cameraPosition.y;
	translationMatrix[ 14 ] = -cameraPosition.z;
	translationMatrix[ 15 ] = 1.f;

	RendererInterface::SetViewMatrix( translationMatrix * rotationMatrix );
}
