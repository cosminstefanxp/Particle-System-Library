/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/
#include "ParticleEmitter.h"

#define MAX_RAND_VAL 32000

ParticleEmitter::ParticleEmitter(float _initVelocity, Vector3D _initColor, float _initEnergy, float _initSize, Vector3D _initDirection,Particle *(*_particleGenerator)())
{
	this->initialVelocity=_initVelocity;
	this->initialColor=_initColor;
	this->initialEnergy=_initEnergy;
	this->initialSize=_initSize;
	this->initialDirection=_initDirection;

	this->colorVariance=Vector3D(0,0,0);
	this->directionVariance=Vector3D(0,0,0);
	this->energyVariance=0;
	this->sizeVariance=0;
	this->velocityVariance=0;

	this->particleGenerator=_particleGenerator;

	/* initialize random seed: */
	srand ( time(NULL) );
}


ParticleEmitter::~ParticleEmitter(void)
{
}

//generate a random number between 0 and 1
float ParticleEmitter::getFloatRand()
{
	float val=(((float)(rand()%MAX_RAND_VAL))/(MAX_RAND_VAL-1));
	return val;
}

//generate a random number between -1 and 1
float ParticleEmitter::getFloatRandNeg()
{
	float val=(((float)(rand()%MAX_RAND_VAL))/(MAX_RAND_VAL-1));
	return (val-0.5)*2;
}

//Generates a new particle, using the attributes set in the emitter and the position provided by generateStartPosition
ParticleData ParticleEmitter::generateParticle()
{
	Particle *newParticle;
	ParticleData pData;
	GLfloat val;

	//generate using the generation function, if declared
	if(particleGenerator!=NULL)
		newParticle=particleGenerator();
	else
		newParticle=new Particle();

	//set parameters, considering variance
	//color
	if(!(colorVariance==Vector3D(0,0,0)))
	{
		val=getFloatRand()*colorVariance.x;
		pData.color.x=this->initialColor.x+this->initialColor.x*val;
		
		val=getFloatRand()*colorVariance.y;
		pData.color.y=this->initialColor.y+this->initialColor.y*val;

		val=getFloatRand()*colorVariance.z;
		pData.color.z=this->initialColor.z+this->initialColor.z*val;
	}
	else
		pData.color=this->initialColor;

	//size
	if(sizeVariance>0)
	{
		val=getFloatRandNeg()*sizeVariance;
		newParticle->size=this->initialSize+this->initialSize*val;
	}
	else
		newParticle->size=this->initialSize;

	//energy
	if(energyVariance>0)
	{
		val=getFloatRand()*energyVariance;
		newParticle->energy=this->initialEnergy+this->initialEnergy*val;
	}
	else
		newParticle->energy=this->initialEnergy;

	//velocity
	pData.velocity=generateStartVelocity();

	//position
	pData.position=generateStartPosition();

	//particle
	pData.particle=newParticle;
	
	return pData;
}

//Generate a starting point for one object, somewhere inside the particle generator surface. This method should be overriden
Vector3D ParticleEmitter::generateStartPosition()
{
	return Vector3D(0,0,0);
}

//Generate the initial velocity (speed+direction) point for one object. This method can be overriden.
Vector3D ParticleEmitter::generateStartVelocity()
{
	float velocityTemp;
	float val;
	if(velocityVariance>0)
	{
		val=getFloatRandNeg()*velocityVariance;
		velocityTemp=this->initialVelocity+this->initialVelocity*val;
	}
	else
		velocityTemp=this->initialVelocity;

	//direction -> final velocity
	Vector3D directionTemp=this->initialDirection;
	//if no initial direction was provided, we generate a random direction
	if(directionTemp==Vector3D(0,0,0))
		directionTemp=Vector3D(getFloatRandNeg(), getFloatRandNeg(), getFloatRandNeg());
	else
		//if a initial direction was provided, add the variance
		if(!(directionVariance==Vector3D(0,0,0)))
		{
			val = getFloatRandNeg()*directionVariance.x;
			directionTemp.x += val;

			val = getFloatRandNeg()*directionVariance.y;
			directionTemp.y += val;

			val = getFloatRandNeg()*directionVariance.z;
			directionTemp.z += val;
		}
	directionTemp=directionTemp.Normalize();
	
	//Combine the normalized direction with the velocity to generate the 3D velocity
	return directionTemp*velocityTemp;
}