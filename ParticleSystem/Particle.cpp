/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/
#include "Particle.h"
#include <stdio.h>


Particle::Particle(void)
{
}


Particle::~Particle(void)
{
}

void Particle::updatePosition()
{
	if(this->energy>0)
		this->energy--;
}

bool Particle::hasDied()
{
	return this->energy==0;
}