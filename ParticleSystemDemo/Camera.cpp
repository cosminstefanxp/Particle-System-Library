/*
 * Sisteme de prelucrare Grafica - Tema 1
          o Editor Cascada
          o Stefan-Dobrin Cosmin
          o 342C4
*/

#include "camera.h"
#include "math.h"


// constructor
Camera::Camera(CameraType _type)
{
	Camera();
	type=_type;
}

// constructor
Camera::Camera()
{
	//initializare cu valorile standard OpenGL
	Position = Vector3D (0.0, 0.0,	12);
	ForwardVector = Vector3D( 0.0, 0.0, -1.0);
	RightVector = Vector3D (1.0, 0.0, 0.0);
	UpVector = Vector3D (0.0, 1.0, 0.0);
	type=CameraTypeFREECAM;
}

void Camera::RotateX (GLfloat angle)
{
	if(type!=CameraTypeFREECAM && type!=CameraTypeFPS)
	{
		fprintf(stderr, "Rotatie fata de x nepermisa. Tipul curent al camerei: %d\n",type);
		return ;
	}
	Vector3D UpNew = UpVector * cos(angle) + ForwardVector * sin(angle);
	Vector3D FwdNew = ForwardVector * cos(angle) - UpVector * sin(angle);
	//FwdNew =  UpNew.CrossProduct(RightVector);

	ForwardVector=FwdNew;
	UpVector=UpNew;

}

void Camera::RotateY (GLfloat angle)
{
	if(type==CameraTypeORBIT || type==CameraTypeFIXED)
	{
		fprintf(stderr, "Rotatie fata de y nepermisa. Tipul curent al camerei: %d\n",type);
		return ;
	}
	Vector3D FwdNew = ForwardVector * cos(angle) + RightVector * sin(angle);
	Vector3D RightNew = RightVector * cos(angle) - ForwardVector * sin(angle);
	//RightNew = FwdNew.CrossProduct(UpVector);

	ForwardVector=FwdNew;
	RightVector=RightNew;
}

void Camera::RotateZ (GLfloat angle)
{
	if(type!=CameraTypeFREECAM)
	{
		fprintf(stderr, "Rotatie fata de z nepermisa. Tipul curent al camerei: %d\n",type);
		return ;
	}

	Vector3D UpNew = UpVector * cos(angle) + RightVector * sin(angle);
	Vector3D RightNew = RightVector * cos(angle) - UpVector * sin(angle);
	//RightNew = -UpNew.CrossProduct(ForwardVector);

	RightVector=RightNew;
	UpVector=UpNew;
}

// rotire fata de centru, aflat in fata la o distanta data
void Camera::RotateXCenter (GLfloat angle, float distance)
{
	if(type!=CameraTypeORBIT)
	{
		fprintf(stderr, "Rotatie fata de un centru nepermisa. Tipul curent al camerei: %d\n",type);
		return ;
	}
	MoveForward(distance);
	RotateX(angle);
	MoveBackward(distance);
}

// rotire fata de centru, aflat in fata la o distanta data
void Camera::RotateYCenter (GLfloat angle, float distance)
{
	if(type!=CameraTypeORBIT)
	{
		fprintf(stderr, "Rotatie fata de un centru nepermisa. Tipul curent al camerei: %d\n",type);
		return ;
	}
	MoveForward(distance);
	RotateY(angle);
	MoveBackward(distance);
}

// rotire fata de centru, aflat in fata la o distanta data
void Camera::RotateZCenter (GLfloat angle, float distance)
{
	if(type!=CameraTypeORBIT)
	{
		fprintf(stderr, "Rotatie fata de un centru nepermisa. Tipul curent al camerei: %d\n",type);
		return ;
	}

	MoveForward(distance);
	RotateZ(angle);
	MoveBackward(distance);
}

// plasare observator/camera in scena
void Camera::Render( void )
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// punctul catre care se uita camera
	Vector3D ViewPoint = Position + ForwardVector;

	// stim vectorul UpVector, folosim LookAt
	if(type==CameraTypeFPS)
		gluLookAt(	Position.x,Position.y,Position.z,
				ViewPoint.x,ViewPoint.y,ViewPoint.z,
				0,1,0);
	else
		gluLookAt(	Position.x,Position.y,Position.z,
				ViewPoint.x,ViewPoint.y,ViewPoint.z,
				UpVector.x,UpVector.y,UpVector.z);
}

// miscari simple, pe toate axele
void Camera::MoveForward( GLfloat distance )
{
	Position = Position + (ForwardVector * distance);
}

void Camera::MoveBackward( GLfloat distance )
{
	Position = Position + (ForwardVector * -distance);
}

void Camera::MoveRight ( GLfloat distance )
{
	Position = Position + ( RightVector * distance);
}

void Camera::MoveLeft ( GLfloat distance )
{
	Position = Position - ( RightVector * distance);
}

void Camera::MoveUpward( GLfloat distance )
{
	Position = Position + ( UpVector * distance );
}

void Camera::MoveDownward( GLfloat distance )
{
	Position = Position + ( UpVector * -distance );
}

void Camera::Move( GLfloat xDistance, GLfloat yDistance, GLfloat zDistance)
{
	Position = Position + (RightVector * xDistance + UpVector * yDistance + ForwardVector * zDistance);
}

// seteaza pozitie
void Camera::SetPosition(Vector3D value)
{
	Position = value;
}

// seteaza forward vector
void Camera::SetForwardVector(Vector3D value)
{
	ForwardVector = value;
}

// seteaza right vector
void Camera::SetRightVector(Vector3D value)
{
	RightVector = value;
}

// seteaza up vector
void Camera::SetUpVector(Vector3D value)
{
	UpVector = value;
}

const GLfloat* Camera::getViewMatrix()
{
	//Salvez matricea curenta
	glPushMatrix();
	
	//Realizam un rendering al camerei -> matricea view este incarcata
	this->Render();
	
	//Salvam matricea de vizualizare
	glGetFloatv(GL_MODELVIEW_MATRIX,Camera::viewMatrix);

	//Recuperam matricea pre-existenta
	glPopMatrix();

	return this->viewMatrix;
}