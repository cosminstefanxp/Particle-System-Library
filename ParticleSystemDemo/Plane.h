/*
 * Sisteme de prelucrare Grafica - Tema 1
          o Editor Cascada
          o Stefan-Dobrin Cosmin
          o 342C4
*/

#ifndef PLANE_H
#define PLANE_H

#include <glut.h>
#include "Object3D.h"

/*
	Clasa Plane
	Deseneaza un plan cu numar variabil de quaduri in XOZ. Planul va avea latimea si lungimea de 2*size si va fi centrat
	in coordonatele position.
*/

	// intensitarea culorii ambientale
const float ambientColorIntensity=0.2;

// derivata din Object3D
class Plane : public Object3D
{

// VARIABILE
//-------------------------------------------------
private:
	// dimensiunea unei laturi
	float size;


// FUNCTII
//-------------------------------------------------
public:
	// constructor fara parametri
	Plane();
	// constructor care ia dimensiunea unei laturi
	Plane(float);

	// obtine latimea planului
	float getSize();



protected:
	// functia custmDraw care va fi apelata de Object3D pentru a desena planul
	void customDraw();
};

#endif