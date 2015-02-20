#pragma once
#ifndef INCLUDED_WARP_SYSTEM_HPP
#define INCLUDED_WARP_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <Code/Audio/AudioInterface.hpp>
#include <Code/Events/EventSubscriber.hpp>
#include <Code/Math/FloatVector2.hpp>
#include <Code/ComponentSystem.hpp>

#include "WarpComponent.hpp"


//-----------------------------------------------------------------------------------------------
/************************************************************************************************
************************************************************************************************/
class WarpSystem : public ComponentSystem< WarpComponent >, EventSubscriber
{
public:
	WarpSystem( size_t numComponentsInPool, const FloatVector2& worldDimensions );

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ );
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float /*deltaSeconds*/ ) { }
	void OnDestruction();

	//Events
	void OnWarpEngaged( EventDataBundle& eventData );

private:
	//Data Members
	FloatVector2 m_warpBounds;
	AudioInterface::SoundID m_warpSound;
};



inline WarpSystem::WarpSystem( size_t numComponentsInPool, const FloatVector2& worldDimensions )
	: ComponentSystem( numComponentsInPool )
	, m_warpBounds( worldDimensions )
	, m_warpSound( 0 )
{ }

#endif //INCLUDED_WARP_SYSTEM_HPP
