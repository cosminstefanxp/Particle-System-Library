/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/
#pragma once
#include "Vector3D.h"

enum AttractorType { ACT_ON_VELOCITY, ACT_ON_POSITION };
enum AttractionType { CONSTANT_ATTRACTION, LINEAR_ATTRACTION }; 

class ParticleAttractor
{
public:
	//Constructors
	__declspec(dllexport) ParticleAttractor(Vector3D _position, float _force, float maxDist, AttractorType _type=ACT_ON_VELOCITY, AttractionType _attractionType=CONSTANT_ATTRACTION);
	__declspec(dllexport) ~ParticleAttractor(void);

	//Attributes
	Vector3D position;		// indicate the position of the attractor. 
	float force;			// indicates the attracting force of the attractor. How the force acts on the particles depends on the following parameters. The force acts to draw the particle towards the attractor.
	float maxDist;			// indicates the maximal distance at which the attractor has effect. Only particles closer that this distance to the attractor will be attracted. 
							// A maximum distance of 0 implies that the attractor affects particles no matter the distance.
	AttractorType type;		// indicates whether the force is added to the speed and direction in each step (ACT_ON_VELOCITY) or only applied to the position of the particle (ACT_ON_POSITION). 
							// When ACT_ON_VELOCITY the particle will accelerate towards the attractor while with a non-additive force it will move there with constant speed. 
	AttractionType attractionType;	// indicates the kind of attractor. The following values exist 
									//	* CONSTANT_ATTRACTION: indicates that the force is constant independent of the distance.
									//	* LINEAR_ATTRACTION:   indicates a linearly growing force. At the maximal distance the force is 0 while at the position of the attractor it attains the given value. If the maximal distance is 0, the force is inversely proportional with the distance.

	//Methods

	/* Changes the position and velocity of a given particle, according to the parameters of the attractor. */
	__declspec(dllexport) virtual void attractParticle(Vector3D *position, Vector3D *velocity);
};

