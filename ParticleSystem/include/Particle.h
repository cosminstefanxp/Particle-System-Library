/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/
#pragma once
#include "Vector3D.h"
#include "Vector4D.h"
#include "ParticleCamera.h"

/***
 * PARTICLE CLASS
 * 
 * The class that describes a particle from a given particle system.
 */
class Particle
{
public:
	//Constructors
	__declspec(dllexport) Particle(void);
	__declspec(dllexport) virtual ~Particle(void);
	
	//AttributeS
	int 	energy;			//The energy of the particle
	float 	size;			//The size of the particle 

	//Methods

	//Update any relevant data of the particle when the position of the particle has been changed
	void updatePosition();
	//True, if the particle has no more energy left (is dead)
	bool hasDied();

};

