/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/
#include "ParticleAffector.h"


ParticleAffector::ParticleAffector(Vector3D _force, AffectorType _type)
{
	this->force=_force;
	this->type=_type;
}


ParticleAffector::~ParticleAffector(void)
{
}

/* Changes the position and velocity of a given particle, according to the parameters of the affector. */
void ParticleAffector::affectParticle(Vector3D *pPosition, Vector3D *pVelocity)
{
	if(type==ACT_ON_POSITION_AFF)
		*pPosition=*pPosition+force;
	else
		*pVelocity=*pVelocity+force;
}