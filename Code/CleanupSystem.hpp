#pragma once
#ifndef INCLUDED_CLEANUP_SYSTEM_HPP
#define INCLUDED_CLEANUP_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>
#include <Code/Component.hpp>
#include <Code/System.hpp>


//-----------------------------------------------------------------------------------------------
/************************************************************************************************
The cleanup system is just a demonstration of how to properly cleanup components.

It can come in handy for components that are purely data or for testing new components.
************************************************************************************************/
class CleanupSystem : public System
{
public:
	CleanupSystem() { }

	void AddComponent( Component* component ) { m_components.push_back( component ); }

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ ) { }
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float /*deltaSeconds*/ ) { }
	void OnDestruction();


private:
	//Data Members
	std::vector< Component* > m_components;
};

#endif //INCLUDED_CLEANUP_SYSTEM_HPP
