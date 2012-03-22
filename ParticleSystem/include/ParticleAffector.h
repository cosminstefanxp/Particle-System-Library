/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/
#pragma once
#include "Vector3D.h"

enum AffectorType { ACT_ON_VELOCITY_AFF, ACT_ON_POSITION_AFF };

class ParticleAffector
{
public:
	__declspec(dllexport) ParticleAffector(Vector3D force, AffectorType type=ACT_ON_VELOCITY_AFF);
	__declspec(dllexport) virtual ~ParticleAffector(void);
	
	//Attributes
	Vector3D force;			// indicates the attracting force of the attractor.
	AffectorType type;		// indicates whether the force is added to the speed and direction in each step (ACT_ON_VELOCITY) or only applied to the position of the particle (ACT_ON_POSITION). 
							// When ACT_ON_VELOCITY the particle will accelerate (such as gravity), while with a non-additive force it will move with constant speed (such as wind)

	//Methods
	/* Changes the position and velocity of a given particle, according to the parameters of the affector. */
	__declspec(dllexport) virtual void affectParticle(Vector3D *position, Vector3D *velocity);
};

