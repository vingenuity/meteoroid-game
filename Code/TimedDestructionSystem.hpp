#pragma once
#ifndef INCLUDED_TIMED_DESTRUCTION_SYSTEM_HPP
#define INCLUDED_TIMED_DESTRUCTION_SYSTEM_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include <Code/System.hpp>

struct TimedDestructionComponent;


//-----------------------------------------------------------------------------------------------
class TimedDestructionSystem : public System
{
public:
	TimedDestructionSystem() { }

	void AddComponent( TimedDestructionComponent* component ) { m_components.push_back( component ); }

	//Lifecycle
	void OnAttachment( SystemManager* /*manager*/ ) { }
	void OnEndFrame();
	void OnRender() const { }
	void OnUpdate( float deltaSeconds );
	void OnDestruction();


private:
	//Data Members
	std::vector< TimedDestructionComponent* > m_components;
};

#endif //INCLUDED_TIMED_DESTRUCTION_SYSTEM_HPP
