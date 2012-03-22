/*
 * Sisteme de prelucrare Grafica - Tema 1
          o Editor Cascada
          o Stefan-Dobrin Cosmin
          o 342C4
*/

#include "Object3D.h"

/*
Declaratiile clasei Object3D
Clasa pentru desenare si modificare a unui obiect 3D.
*/

// VARIABILE STATICE
//-------------------------------------------------
//float *Vector3D::arr = new float[3];
Vector3D Object3D::SelectedColor = Vector3D(1,0,1);		// culoarea obiectului selectat
Vector3D Object3D::ColorIncrement = Vector3D(0.04,0.04,0.04);	// valoarea cu care creste/scade in timp culoarea de mai sus


// CONSTRUCTORI
//-------------------------------------------------

// constructor de baza
Object3D::Object3D()
{
	defaultSettings();
}

// seteaza si tipul obiectului
Object3D::Object3D(ObjectType _Type)
{
	defaultSettings();
	Type = _Type;
}

// seteaza si pozitia
Object3D::Object3D(Vector3D _translation)
{
	defaultSettings();
	this->setPosition(_translation);
}

// seteaza pozitie, rotatie, translatie
Object3D::Object3D(Vector3D _translation, Vector3D _rotation, Vector3D _scale)
{
	defaultSettings();
	this->setPosition(_translation);
	this->setRotation(_rotation);
	this->setScale(_scale);	
}

// seteaza toti parametrii la valorile lor default
void Object3D::defaultSettings()
{
	translation = Vector3D(0.0,0.0,0.0);
	rotation = Vector3D(0.0,0.0,0.0);
	scale = Vector3D(1.0,1.0,1.0);
	this->diffuse = Vector4D(1,1,1,1);
	this->ambient = Vector4D(1,1,1,1);
	this->color = Vector3D(1,1,1);
	this->specular = Vector4D(0,0,0,1);

	Lighted = true;
	Wireframe = false;
	Visible = true;
	Type = Custom;
	FullyTransparent = false;

	levelOfDetail = 1.0f/10;
	selected = false;

	cylinderQuadric=gluNewQuadric();
}

// DRAW
//-------------------------------------------------
void Object3D::Draw ()
{
	// daca nu este vizibil, nu-l desenam
	if(!Visible)
		return;

	glPushMatrix();

	// translatie
	glTranslatef( translation.x , translation.y , translation.z );

	// rotatie
	glRotatef( rotation.x , 1.0 , 0.0 , 0.0 );
	glRotatef( rotation.y , 0.0 , 1.0 , 0.0 );
	glRotatef( rotation.z , 0.0 , 0.0 , 1.0 );

	// scalare
	glScalef( scale.x , scale.y , scale.z);

	// setari de material :
	// daca nu este selectat
	if( !selected )
	{
		// culoare normala
		glColor3f(color.x,color.y,color.z);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuse.Array());
	}
	else
	{
		// culoarea atunci cand obiectul este selectat
		glColor3f(SelectedColor.x, SelectedColor.y, SelectedColor.z);
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,(Vector4D(SelectedColor.x,SelectedColor.y,SelectedColor.z,1)).Array());
	}
	// culoare speculara
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular.Array());

	// daca este wireframe
	if( Wireframe )
		switch( Type )
	{
		// cub wireframe
		case TypeCube :	glutWireCube(1.0); break;
		// sfera wireframe
		case Sphere : glutWireSphere(1.0, levelOfDetail, levelOfDetail); break;
		// orice alt obiect, specificat de programator
		case Custom : customDraw();
	}
	// daca nu este wireframe
	else
		switch( Type )
	{
		//tetraedru
		case Tetrahedron : glutSolidTetrahedron();
		//dodecaedru
		case Dodecahedron : glutSolidDodecahedron();
		// ibric
		case Teapot : glutSolidTeapot(1); break;
		// con solid
		case Cone : glutSolidCone(1.0,2,levelOfDetail,levelOfDetail); break;
		// cub solid
		case TypeCube :	glutSolidCube(1.0); break;
		// sfera solida
		case Sphere : glutSolidSphere(1.0, levelOfDetail, levelOfDetail); break;
		// cilindru
		case Cylinder: gluCylinder(cylinderQuadric,1,1,1,levelOfDetail,levelOfDetail); break;
		// tor
		case Torus: glutSolidTorus(0.7,1,levelOfDetail,levelOfDetail); break;
		// orice alt obiect, specificat de programator
		case Custom : customDraw(); break;
	}

	glPopMatrix();
}

// functie proprie
// se presupune ca Wireframe este tratat inauntru
void Object3D::customDraw()
{
	// TODO : aici va puteti desena un obiect personalizat/incarcat din fisier/pre-generat/etc
}

// SETTERS
//-------------------------------------------------

// selecteaza
void Object3D::select()
{
	selected = true;
}

// deselecteaza
void Object3D::deselect()
{
	selected = false;
}

// seteaza culoare
void Object3D::setColor(Vector3D _color)
{
	color = _color;
}

// seteaza pozitie
void Object3D::setPosition(Vector3D _translation)
{
	translation = _translation;
}

// seteaza rotatie
void Object3D::setRotation(Vector3D _rotation)
{
	rotation = _rotation;
}

// seteaza scalare
void Object3D::setScale(Vector3D _scale)
{
	scale = _scale;
}

// seteaza nivelul de detaliu
void Object3D::setLevelOfDetail(float _levelOfDetail)
{
	if( _levelOfDetail > 0 && _levelOfDetail < MAXIMUM_LOD)
		levelOfDetail = 1.0f/_levelOfDetail;
	else
		levelOfDetail = 1.0f/MAXIMUM_LOD;
}

// seteaza culoarea difuza
void Object3D::setDiffuseColor(Vector4D _diffuse)
{
	diffuse = _diffuse;
}

// seteaza culoarea speculara
void Object3D::setSpecularColor(Vector4D _specular)
{
	specular = _specular;
}

// seteaza culoarea ambientala
void Object3D::setAmbientColor(Vector4D _ambient)
{
	ambient = _ambient;
}

// daca obiectul este selectabil
bool Object3D::isSelectable()
{
	return selectable;
}

// intoarce indexul pentru stiva de nume al obiectului
int Object3D::getSelectIndex()
{
	return selectIndex;
}

// GETTERS
//-------------------------------------------------

// intoarce pozitia
Vector3D Object3D::getPosition()
{
	return translation;
}

// intoarce rotatia
Vector3D Object3D::getRotation()
{
	return rotation;
}

// intoarce scala
Vector3D Object3D::getScale()
{
	return scale;
}

// intoarce culoarea
Vector3D Object3D::getColor()
{
	return scale;
}

// intoarce culoarea speculara
Vector4D Object3D::getSpecularColor()
{
	return specular;
}

// intoarce culoarea diffusa
Vector4D Object3D::getDiffuseColor()
{
	return diffuse;
}

// intoarce culoarea ambientala
Vector4D Object3D::getAmbientColor()
{
	return ambient;
}

// intoarce nivelul de detaliu
float Object3D::getLevelOfDetail()
{
	return levelOfDetail;
}

// seteaza daca obiectul este selectabil
void Object3D::setSelectable(bool _selectable)
{
	selectable=_selectable;
}

// indexul folosit in stiva de nume pentru selectie
void Object3D::setSelectIndex(int _index)
{
	selectIndex=_index;
}