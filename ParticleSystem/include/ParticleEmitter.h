/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/
#pragma once
#include "Vector3D.h"
#include "Particle.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PARTICLE_GENERATOR Particle *(*particleGenerator)()

struct ParticleData {
	Particle *particle;
	Vector3D position;
	Vector3D velocity;
	Vector3D color;
};

/***
 * PARTICLE EMITTER CLASS
 * 
 * The class that describes the generator of new particles. Helps generate new particles, or regenerate old particles using already existing (but dead) particles.
 * The position of the emitter is set in the ParticleSystem, so the emitter should generate positions relative to it's center (0,0).
 */
class ParticleEmitter
{
public:
	//Constructors
	__declspec(dllexport) ParticleEmitter(float _initVelocity, Vector3D _initColor, float _initEnergy, float _initSize=1, Vector3D _initDirection=Vector3D(0,0,0),PARTICLE_GENERATOR=NULL);
	__declspec(dllexport) virtual ~ParticleEmitter(void);

	//Attributes
	Vector3D initialDirection;	//the initial value of the particle
	Vector3D directionVariance;	//the variance of the value e.q. particleValue=initialValue*(1+valueVariance)

	float initialVelocity;		//the initial value of the particle
	float velocityVariance;		//the variance of the value e.q. particleValue=initialValue*(1+valueVariance)
	
	Vector3D initialColor;		//the initial value of the particle
	Vector3D colorVariance;		//the variance of the value e.q. particleValue=initialValue*(1+valueVariance)
	
	int initialEnergy;			//the initial value of the particle
	float energyVariance;		//the variance of the value e.q. particleValue=initialValue*(1+valueVariance)
	
	float initialSize;			//the initial value of the particle
	float sizeVariance;			//the variance of the value e.q. particleValue=initialValue*(1+valueVariance)

	Particle *(*particleGenerator)();	//pointer to a function that generates a new empty particle. If null, generates normal Particle.

	//Methods
	//Generates a new particle, using the attributes set in the emitter. This method should not be overriden, as everything that it does can be set either by attributes,
	//either using the following methods (generateStartPosition, generateStartVelocity).
	__declspec(dllexport) ParticleData generateParticle();

	//Generate a starting point for one object, somewhere inside the particle generator surface. This method can be overriden
	__declspec(dllexport) virtual Vector3D generateStartPosition();

	//Generate the initial velocity (speed+direction) point for one object. This method can be overriden.
	__declspec(dllexport) virtual Vector3D generateStartVelocity();

protected:
	//generate a random number between 0 and 1
	float getFloatRand();
	//generate a random number between -1 and 1
	float getFloatRandNeg();
};

