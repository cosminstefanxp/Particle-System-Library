#include "include\ParticleManager.h"


ParticleManager::ParticleManager(void)
{
}


ParticleManager::~ParticleManager(void)
{
}

//Update all the systems
void ParticleManager::updateSystems()
{
	for(size_t i=0;i<systems.size();i++)
		systems[i]->update();
}
//Render/Draw all the systems
void ParticleManager::renderSystems()
{
	for(size_t i=0;i<systems.size();i++)
		systems[i]->draw();
}
//Notifies all the systems that the camera position/orientation has changed
void ParticleManager::notifyCameraChange()
{
	for(size_t i=0;i<systems.size();i++)
		systems[i]->notifyCameraChange();
}
//Adds an affector to all the systems
void ParticleManager::addAffectorAll(ParticleAffector* _affector)
{
	for(size_t i=0;i<systems.size();i++)
		systems[i]->addAffector(_affector);
}
//Adds an attractor to all the systems
void ParticleManager::addAttractorAll(ParticleAttractor* _attractor)
{
	for(size_t i=0;i<systems.size();i++)
		systems[i]->addAttractor(_attractor);
}
//Gets the total number of particles in this manager
int ParticleManager::getParticleCount()
{
	int count=0;
	for(size_t i=0;i<systems.size();i++)
		count+=systems[i]->getParticleCount();
	return count;
}
//Destroy all the particle systems inside
void ParticleManager::destroySystems()
{
	for(size_t i=0;i<systems.size();i++)
		delete systems[i];
	systems.clear();
}