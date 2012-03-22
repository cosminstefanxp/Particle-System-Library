#include "PE_Parallelepiped.h"

PE_Parallelepiped::PE_Parallelepiped(float _initVelocity, Vector3D _initColor, float _initEnergy, float _initSize, Vector3D _initDirection,PARTICLE_GENERATOR) :
	ParticleEmitter(_initVelocity,_initColor,_initEnergy,_initSize,_initDirection,particleGenerator)
{
	this->length=0;
	this->height=0;
	this->width=0;
}

PE_Parallelepiped::~PE_Parallelepiped(void)
{

}

//Generate a starting point for one object, somewhere inside the particle generator surface.
Vector3D PE_Parallelepiped::generateStartPosition()
{
	GLfloat x,y,z;
	int maxLimit=100;

	/* generate numbers between 0 and 1*/
	x = getFloatRand();
	y = getFloatRand();
	z = getFloatRand();
	
	/* generate positions */
	x = -length/2.0f + length*x;
	y = -height/2.0f + height*y;
	z = -width/2.0f + width*z;
	
	return Vector3D(x,y,z);
}