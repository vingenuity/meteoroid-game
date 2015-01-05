#pragma once
#ifndef INCLUDED_WARP_SYSTEM_HPP
#define INCLUDED_WARP_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include <Code/Events/EventSubscriber.hpp>
#include <Code/Math/FloatVector2.hpp>
#include <Code/System.hpp>

struct WarpComponent;


//-----------------------------------------------------------------------------------------------
/************************************************************************************************
************************************************************************************************/
class WarpSystem : public System, EventSubscriber
{
public:
	WarpSystem( const FloatVector2& worldDimensions );

	void AddWarpComponent( WarpComponent* warpComponent ) { m_warpComponents.push_back( warpComponent ); }

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
	std::vector< WarpComponent* > m_warpComponents;
	FloatVector2 m_warpBounds;
};



inline WarpSystem::WarpSystem( const FloatVector2& worldDimensions )
	: m_warpBounds( worldDimensions )
{ }

#endif //INCLUDED_WARP_SYSTEM_HPP
