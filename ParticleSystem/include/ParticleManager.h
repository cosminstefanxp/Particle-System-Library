#pragma once
#include "ParticleSystem.h"
#include <vector>

using namespace std;
/***
 * PARTICLE MANAGER CLASS
 *
 * Class that manages multiple particle systems, with functions such as updating, rendering, creating and destroying the particle systems it monitors.
 ***/
class ParticleManager
{
public:
	__declspec(dllexport) ParticleManager(void);
	__declspec(dllexport) ~ParticleManager(void);

	//Methods
	//Register a new system to the manager
	__declspec(dllexport) inline void registerSystem(ParticleSystem *_system) { this->systems.push_back(_system); }
	//Update all the systems
	__declspec(dllexport) void updateSystems();
	//Render/Draw all the systems
	__declspec(dllexport) void renderSystems();
	//Notifies all the systems that the camera position/orientation has changed
	__declspec(dllexport) void notifyCameraChange();
	//Adds an affector to all the systems
	__declspec(dllexport) void addAffectorAll(ParticleAffector* _affector);
	//Adds an attractor to all the systems
	__declspec(dllexport) void addAttractorAll(ParticleAttractor* _attractor);
	//Destroy all the particle systems inside
	__declspec(dllexport) void destroySystems();	
	//Gets the total number of particles in this manager
	__declspec(dllexport) int getParticleCount();

	//Attributes
private:
	std::vector<ParticleSystem*> systems;
};

