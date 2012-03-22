/*
 * Sisteme de prelucrare Grafica - Tema 1
          o Editor Cascada
          o Stefan-Dobrin Cosmin
          o 342C4
*/

#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <glut.h>
#include "Vector3D.h"
#include "Vector4D.h"

// nivelul maxim de detaliu
#define MAXIMUM_LOD 50

enum ObjectType
{
	TypeCube,			// glut cube
	Sphere,			// glut sphere
	Dodecahedron,	// glut dodecahedron
	Teapot,			// glut teapot
	Cone,			// glut cone
	Tetrahedron,		// glut tetrahedron	
	Custom,			// obiect propriu
	Cylinder,		// glu cylinder
	Torus			// glut torus
};
/*
	OBJECT3D
	Clasa pentru desenare si modificare a unui obiect 3d.
*/
class Object3D
{
// VARIABILE STATICE ::
//-------------------------------------------------
public:
	static Vector3D SelectedColor;
	static Vector3D ColorIncrement;

// VARIABILE
//-------------------------------------------------
// publice
public :
	// incep cu litera mare
	ObjectType Type;	// tipul obiectuliu
	bool Wireframe;		// daca va fi desenat wireframe
	bool Visible;		// daca va fi sau nu desenat
	bool Lighted;		// daca este sau nu luminat
	bool FullyTransparent;	// daca este sau nu complet transparent (toate componentele) - pentru optimizare la draw

// private
protected:
	// incep cu litera mica
	Vector3D translation;	// pozitie
	Vector3D rotation;		// rotatie
	Vector3D scale;			// scalare
	Vector3D color;			// culoare
	Vector4D ambient;		// culoarea ambientala a materialului (nu se foloseste in mod normal, fiind utilizata componenta difuza)
	Vector4D diffuse;		// culoare difuza a materialului ( atentie , la testul alfa se foloseste componenta A din culoarea difuza !!!
	Vector4D specular;		// culoare speculara a materialului

	float levelOfDetail;	// nivelul de detaliu (pentru glutSphere)

	bool selectable;		// daca este sau nu selectabil
	int selectIndex;		// id-ul sau in stiva de nume
	bool selected;			// daca este sau nu selectat

	GLUquadric *cylinderQuadric;

// METODE ::
//-------------------------------------------------
public:
// constructor
	// fara parametri
	Object3D();		
	// doar cu tip
	Object3D(ObjectType);
	// doar cu pozitie
	Object3D(Vector3D);			
	// cu pozitie, rotatie, scalare
	Object3D(Vector3D,Vector3D,Vector3D);

// functie de desenare
	void virtual Draw();

// setters 
	// pentru toate variabilele care nu sunt publice
	void select();
	void deselect();
	virtual void setPosition(Vector3D);
	virtual void setRotation(Vector3D);
	virtual void setScale(Vector3D);
	virtual void setColor(Vector3D);
	virtual void setDiffuseColor(Vector4D); // Seteaza culoare difuza ( atentie , la testul alfa se foloseste componenta A din culoarea difuza !!!_
	virtual void setSpecularColor(Vector4D);
	virtual void setAmbientColor(Vector4D);
	void setSelectable(bool);
	void setSelectIndex(int);
	void setLevelOfDetail(float);


// getters
	// pentru toate variabilele care nu sunt publice, si pot fi modificate din exterior
	Vector3D getRotation();
	Vector3D getScale();
	Vector3D getPosition();
	Vector3D getColor();
	Vector4D getDiffuseColor();
	Vector4D getSpecularColor();
	Vector4D getAmbientColor();
	float getLevelOfDetail();
	bool isSelectable();
	int getSelectIndex();

protected:
	// functie proprie de desenare. se apeleaza doar daca tipul este Custom
	void virtual customDraw();
	// seteaza valorile default 
	void defaultSettings();
	
};

#endif
