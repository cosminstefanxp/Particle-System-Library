/*
 * Sisteme de prelucrare Grafica - Tema 1
          o Editor Cascada
          o Stefan-Dobrin Cosmin
          o 342C4
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <glut.h>
#include <stdio.h>
#include "Vector3D.h"


// tipul de CAMERA
enum CameraType
{
	CameraTypeFIXED,		// camera fixa, ce nu poate fi modificata
	CameraTypeFIXED_HEIGHT, // camera aflata la aceeasi inaltim. Permite rotatii Oy
	CameraTypeSATTELITE,	// camera aflata la inaltime mare, priveste in directia jos. Permite rotatii al vectorului Up.
	CameraTypeFREECAM,		// utilizatorul va putea sa deplaseze cum doreste el camera folsind orice tip de rotatii
	CameraTypeFPS,			// utilizatorul va putea sa deplaseze cum doreste el camera, insa nu sunt permite rotatii pe axa Oz. La afisare, vectorul UP este in permanenta pe axa Oy (0,1,0)
	CameraTypeORBIT			// camera care permite rotatii fata de un centru (orbitare in jurul acestuia)
};

/*
	Camera
	Clasa pentru initializarea si miscarea unui observator prin scena
*/

class Camera
{
	
// VARIABILE
//-------------------------------------------------
private:
	Vector3D ForwardVector;
	Vector3D RightVector;	
	Vector3D UpVector;
	Vector3D Position;
public:
	CameraType type;
	static GLfloat *viewMatrix;

// FUNCTII
//-------------------------------------------------
public:
	// constructor fara parametri
	Camera();			

	// constructor ce primeste tipul camerei
	Camera(CameraType type);

	// plaseaza observatorul in scena
	void Render ( void );	

	// rotatie fata de axele de coordonate, in aceeasi pozitie
	void RotateX ( GLfloat Angle );
	// rotatie fata de axele de coordonate, in aceeasi pozitie
	void RotateY ( GLfloat Angle );
	// rotatie fata de axele de coordonate, in aceeasi pozitie
	void RotateZ ( GLfloat Angle );

	// rotatie fata de un centru de rotatie aflat la o distanta data in fata
	void RotateXCenter ( GLfloat Angle, float Distance );
	// rotatie fata de un centru de rotatie aflat la o distanta data in fata
	void RotateYCenter ( GLfloat Angle, float Distance );
	// rotatie fata de un centru de rotatie aflat la o distanta data in fata
	void RotateZCenter ( GLfloat Angle, float Distance );

	// miscare camera in toate directiile
	void Move( GLfloat xDistance, GLfloat yDistance, GLfloat zDistance);

	// miscare fata/spate
	void MoveForward ( GLfloat Distance );
	// miscare fata/spate
	void MoveBackward ( GLfloat Distance );

	// miscare sus/jos
	void MoveUpward ( GLfloat Distance );
	// miscare sus/jos
	void MoveDownward ( GLfloat Distance );

	// miscare stanga/dreapta
	void MoveRight ( GLfloat Distance );
	// miscare stanga/dreapta
	void MoveLeft ( GLfloat Distance );

	//Setteri
	// setare Pozitie
	void SetPosition(Vector3D value);

	// setare ForwardVector
	void SetForwardVector(Vector3D value);

	// setare RightVector
	void SetRightVector(Vector3D value);

	// setare UpVector
	void SetUpVector(Vector3D value);

	//Getteri
	inline Vector3D GetForwardVector() {return this->ForwardVector;}
	inline Vector3D GetUpVector() {return this->UpVector;}
	inline Vector3D GetRightVector() {return this->RightVector;}

	// obtine matricea de vizualizare; Nu ar trebui modificare valorile
	const GLfloat* getViewMatrix();
};

#endif
