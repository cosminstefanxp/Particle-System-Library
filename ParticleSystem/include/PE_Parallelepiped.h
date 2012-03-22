#pragma once
#include "ParticleEmitter.h"

/***
 * Parallelepiped Particle EMITOR CLASS
 * 
 * Class that extends ParticleEmittor, defining e genration volume given by a parallelepiped
 */
class PE_Parallelepiped : 	public ParticleEmitter
{
public:
	__declspec(dllexport) PE_Parallelepiped(float _initVelocity, Vector3D _initColor, float _initEnergy, float _initSize=1, Vector3D _initDirection=Vector3D(0,0,0),PARTICLE_GENERATOR=NULL);
	__declspec(dllexport) virtual ~PE_Parallelepiped(void);
	
	//Dimensions of generation volume
	float length;
	float width;
	float height;

	//Generate a starting point for one object, somewhere inside the particle generator surface.
	//@Override
	__declspec(dllexport) Vector3D generateStartPosition();
};

