/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/
#include "ParticleAttractor.h"


ParticleAttractor::ParticleAttractor(Vector3D _position, float _force, float _maxDist, AttractorType _type, AttractionType _attractionType)
{
	this->position=_position;
	this->maxDist=_maxDist;
	this->force=_force;
	this->type=_type;
	this->attractionType=_attractionType;
}


ParticleAttractor::~ParticleAttractor(void)
{
}

/* Changes the position and velocity of a given particle, according to the parameters of the attractor. */
void ParticleAttractor::attractParticle(Vector3D *pPosition, Vector3D *pVelocity)
{
	//Get the vector from particle towards attractor
	Vector3D vector= position- (*pPosition);
	float distance=vector.Length();

	//check distance
	if(maxDist && distance>maxDist)
		return;

	//Compute attraction force (with value + direction)
	Vector3D totalForce;
	vector=vector.Normalize();
	if(attractionType==CONSTANT_ATTRACTION)
		totalForce=vector*force;
	else	//LINEAR_ATTRACTION
	{
		if(maxDist)
			totalForce=vector*force*distance/maxDist;
		else
			totalForce=vector*force/distance;
	}

	if(type==ACT_ON_POSITION)
		*pPosition=*pPosition+totalForce;
	else
		*pVelocity=*pVelocity+totalForce;
}


