/*
* Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
*/
//include librarii de opengl, glut si glew
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glew32.lib")

//include-uri, stdlib.h inainte de glut.h pentru a preveni warning-ul de redefinire a functiei
//exit (care e definita in ambele librarii)
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>

#include "glew.h"
#include "glut.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Object3D.h"
#include "Camera.h"
#include "Plane.h"
#include "ParticleSystem.h"
#include "PE_Parallelepiped.h"
#include "MyParticleCamera.h"
#include "Texture.h"
#include "ParticleManager.h"

/**
	Optimizari:
		- initial: 40, 12.3, ? ? ? ?
		- dupa quaduri directionate spre camera: 50, 15.5, 36.9, 77, 76.5, 60
		- dupa optimizarea cu load identity direct in particle system: 57, 17, 39.5, 78, 78, 65
		- dupa optimizarea cu pozitii si velocity in ParticleSystem: 58.8, 18.7, 41.5, 79, 79, 69
		- dupa trecerea pozitiilor si a culorii in ParticleSystem: 66, 22.1, 46.5, 79, 78, 71.5
		- dupa prima utilizare VBO: 61, 35, 49, 82, 77, 67
**/

// GLOBAL DEFINITIONS
//-------------------------------------------------

// definitii HUD
#define HUD_TOP					30
#define HUD_SPACING				20
#define HUD_COLUMN_1			20
#define HUD_COLUMN_2			200
#define HUD_FPS					(ecran_width-100)

// tasta escape
#define ESC	27

// definitii dimensiuni vectori
#define MAX_OBJECT_COUNT	100
#define MAX_LIGHT_COUNT		1

// numele obiectelor, vor fi folosite in procesul de picking
#define NONE			0
#define NAME_GROUND		1

// tasta escape
#define ESC	27


float *Vector3D::arr;
float *Vector4D::arr;
float *Camera::viewMatrix;

bool fullscreen=false;
bool animationRunning=true;

// VARIABLES
//-------------------------------------------------

// Optiuni de camera/vizualizare
Camera *camera;
ParticleCamera *pcamera;
int mainWindow;
int ecran_width;
int ecran_height;

// initial nici un obiect nu este selectat
int selectedObjectID = NONE;
Object3D *selectedObject=NULL;
char selectedObjectText[100]="nimic";

// numarul de obiecte
int objectCount;
// vector de obiecte 3D
Object3D **objects;

// Frame Rate
int frame, last_time;
float fps;

//ParticleSystem testing
string psDescriptions;
GLuint textura, textura1;
static int testCount=6;
static int currentTest=0;
ParticleSystem  *psystem;
ParticleManager *manager;
ParticleEmitter *emitter;
ParticleAttractor *attractors[3];
ParticleAffector *affector;

//Mouse
// Statusul fiecarui buton. 
int mouseButtons[5] = {GLUT_UP, GLUT_UP, GLUT_UP, GLUT_UP, GLUT_UP}; 
//Coordonatele ultimului click/actiune a mouseului
int mouseXClick, mouseYClick;

void generateTest(int i);

void init(void)
{
	// initialize vector arrays
	Vector3D::arr = new float[3];
	Vector4D::arr = new float[4];
	Camera::viewMatrix = new float[16];

	/*************CAMERA****************/
	// initializam camera principala
	camera = new Camera();
	camera->type=CameraTypeFPS;
	camera->SetPosition(Vector3D(0,5,15));
	camera->SetForwardVector(Vector3D(0,0,-1));
	camera->SetUpVector(Vector3D(0,1,0));
	camera->SetRightVector(Vector3D(1,0,0));
	// pozitionare camera
	camera->Render();

	/*************OBIECTE****************/
	objects = new Object3D*[MAX_OBJECT_COUNT];
	Object3D *newObject;
	//TODO: Creaza obiecte

	// creaza si seteaza obiectul
	newObject = new Plane(10);
	newObject->setColor(Vector3D(0,0.2,0));
	newObject->setLevelOfDetail(1);
	newObject->setSelectable(true);
	objects[objectCount++]=newObject;

	/*************SISTEME PARTICULE****************/
	textura=LoadTextureBMP("snow.bmp",TEXTURA_FILTRARE_TRILINEAR_ANISOTROPIC);
	textura1=LoadTextureBMP("spark.bmp",TEXTURA_FILTRARE_TRILINEAR_ANISOTROPIC);
	pcamera=new MyParticleCamera(camera);
	pcamera->notifyCameraChange();
	manager=new ParticleManager();
	generateTest(currentTest);

	/*************INITIALIZARE SCENA****************/
	glClearColor(0.0, 0.0, 0.0, 0.0);	// stergem tot
	glEnable(GL_DEPTH_TEST);			// activam verificarea distantei fata de camera (a adancimii)
	glShadeModel(GL_SMOOTH);			// shading model: mod de desenare SMOOTH
	//glEnable(GL_LIGHTING);				// activam iluminarea
	glEnable(GL_NORMALIZE);				// activam normalizarea normalelor
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,(Vector4D(0.1,0.1,0.1,1)).Array());	// lumina ambientala
	glEnable(GL_ALPHA_TEST);			// activam testarea parametrului alfa, pentru a permite transparenta
}

//Genereaza testele 
void generateTest(int i)
{
	PE_Parallelepiped* emitter1;

	//Clean previous allocated memory
	if(emitter)
	{	delete emitter; emitter=NULL; }
	if(attractors[0])
	{	delete attractors[0]; attractors[0]=NULL; }
	if(attractors[1])
	{	delete attractors[1]; attractors[1]=NULL; }
	if(attractors[2])
	{	delete attractors[2]; attractors[2]=NULL; }	
	manager->destroySystems();


	switch(i)
	{
	case 0:
		//Sfera
		emitter=new ParticleEmitter(0.15,Vector3D(0.3,0.2,0.9),40,0.1);
		emitter->velocityVariance=0.1;
		psystem=new ParticleSystem(5000,125,false, emitter, pcamera);
		psystem->position=Vector3D(0,6,0);
		psystem->setTexture(textura);
		psDescriptions.assign("Sfera");
		break;
	case 1:
		//Ploaie
		emitter1=new PE_Parallelepiped(0.2,Vector3D(0.3,0.2,0.9),10,0.02,Vector3D(0,-1,0));
		emitter1->velocityVariance=0.8;
		emitter1->directionVariance=Vector3D(0.5,0,0);
		emitter1->length=4;
		emitter1->height=1;
		emitter1->width=0.5;
		emitter=emitter1;
		psystem=new ParticleSystem(3000,300,false,emitter,pcamera);
		psystem->position=Vector3D(0,6,0);
		psDescriptions.assign("Ploaie");
		break;
	case 2:
		//Particule cu attractori 1
		emitter=new ParticleEmitter(0.2,Vector3D(0.8,0.3,0.4),200,0.05,Vector3D(0,-1,0));
		attractors[0]=new ParticleAttractor(Vector3D(1,7,0),0.05,3);
		psystem=new ParticleSystem(150,1,false,emitter,pcamera);
		psystem->position=Vector3D(0,5,0);
		psystem->addAttractor(attractors[0]);
		psDescriptions.assign("Neuron cu atractor de viteza");
		manager->registerSystem(psystem);

		//Particule cu attractori 2
		emitter=new ParticleEmitter(0.2,Vector3D(0,0.9,0.4),100,0.05,Vector3D(1,0,0));
		psystem=new ParticleSystem(50,1,false,emitter,pcamera);
		psystem->position=Vector3D(-2,5,0);
		attractors[1]=new ParticleAttractor(Vector3D(4,7,0),0.01,3);
		psystem->addAttractor(attractors[1]);
		attractors[2]=new ParticleAttractor(Vector3D(7,5 ,0),0.02,4);
		psystem->addAttractor(attractors[2]);
		psDescriptions.assign("Atractori");
		break;
	case 3:
		//Gravitatie
		emitter1=new PE_Parallelepiped(0.1,Vector3D(0.2,0.1,0.7),30,0.01,Vector3D(0,0,1));
		emitter1->velocityVariance=0.2;
		emitter1->length=3;
		emitter1->height=0.1;
		emitter1->width=0.1;
		emitter1->colorVariance=Vector3D(0,0.3,0);
		emitter1->directionVariance=Vector3D(0,0,0.1);
		psystem=new ParticleSystem(6000,200,false,emitter1,pcamera);
		affector=new ParticleAffector(Vector3D(0,-0.01,0),ACT_ON_VELOCITY_AFF);
		psystem->addAffector(affector);
		psystem->position=Vector3D(0,5,0);
		psDescriptions.assign("Gravitatie");
		break;
	case 4:
		//Foc
		emitter1=new PE_Parallelepiped(0.05,Vector3D(0.75,0.4,0),10,0.01,Vector3D(0,1,0));
		emitter1->velocityVariance=0.3;
		emitter1->colorVariance=Vector3D(0.8,0.3,0);
		emitter1->directionVariance=Vector3D(0.2,0,0);
		emitter1->energyVariance=0.45;
		emitter1->length=4;
		emitter1->height=0;
		psystem=new ParticleSystem(25000,2400,false,emitter1,pcamera);
		psDescriptions.assign("Foc");
		break;
	case 5:
		//Artificii
		emitter=new ParticleEmitter(0.1,Vector3D(0.8,0.4,0.1),100,0.02);
		emitter->sizeVariance=0.5;
		emitter->velocityVariance=0.2;
		psystem=new ParticleSystem(5000,400,true, emitter, pcamera);
		psystem->position=Vector3D(0,6,0);
		psDescriptions.assign("Artificii");
		affector=new ParticleAffector(Vector3D(0,-0.001, 0), ACT_ON_VELOCITY_AFF);
		psystem->addAffector(affector);
		//psystem->setTexture(textura1);
		break;
	}
	manager->registerSystem(psystem);
}

// functia pentru desenarea unui text in spatiu3D
void outputText(GLfloat x, GLfloat y, GLfloat z,char *format,...)
{
	va_list args;
	char buffer[1024],*p;

	//obtinere parametrii si generare text
	va_start(args,format);
	vsprintf_s(buffer, format, args);
	va_end(args);

	// pozitionare corecta text prin specificarea pozitie raster pentru operatii text
    glRasterPos3f (x, y, z);

	//scrie char by char tot stringul trimis
	for (p = buffer; *p; p++) 
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *p);
}

// functia pentru desenarea HUD-ului
void drawHUD()
{
	// obtinem limitele viewportului pentru calcularea aspectului
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);	

	// pregatim matricea de proiectie
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(viewport[0], viewport[2], viewport[3], viewport[1], -1, 20);

	
	// salveaza matricea MODELView
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	// pregateste noua matrice MODELView
	glLoadIdentity();

	/****** afiseaza elementele HUD-ului *****/
	glColor3f(1.0,1.0,1.0);

	// actualizez frame-rate
	frame++;
	int time = glutGet(GLUT_ELAPSED_TIME);
	if (time - last_time > 1000) {
		fps = frame * 1000.0 / (time - last_time);
		last_time = time;
		frame = 0;
	}


	// afiseaza texte
	outputText(HUD_COLUMN_1,HUD_TOP,0,"Sistem curent: %d/%d) %s",currentTest, testCount, psDescriptions.c_str());
	outputText(HUD_COLUMN_1,HUD_TOP+HUD_SPACING,0,"Numar Particule: %d",manager->getParticleCount());
	outputText(HUD_FPS, HUD_TOP, 0,"FPS: %2.3f", fps);


	// revenim la matricea MODELView anterioara
	glPopMatrix();

	// revenim la matricea de proiectie normala
	glMatrixMode(GL_PROJECTION);						
	glPopMatrix();								
	glMatrixMode(GL_MODELVIEW);

}


// functia pentru desenarea scenei 3D
void drawScene()
{
	//desenare sistem de particule
	manager->renderSystems();

	// desenare obiecte
	for ( int i = 0 ; i < objectCount ; i ++ )
			objects[i]->Draw();
}

// functia de display
void display(void)
{

	// de fiecare data cand desenam, la inceput resetam toate bufferurile a.i
	// ele sa aiba valorile initiale definite de functiile:
	// glClearColor ... va seta culoarea backgroundului, vezi init
	// glClearDepth ... va seta valoarea cea mai indepartata a adancimii (adica 1), pentru
	// ca suntem in coordonate fereastra
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	//Deseneaza scena
	drawScene();
	drawHUD();

	//Schimba bufferele
	glutSwapBuffers();
}


// functia de animatie ( functia de idle a GLUT-ului -- similara cu un thread separat)
void update()
{
	Sleep(10);

	manager->updateSystems();

	//de fiecare data cand schimbam ceva in scena tb sa redesenam ca sa observam schimbarea
	glutPostRedisplay();
}

// functia pentru procesarea evenimentelor de la mouse
void mouse(int buton, int stare, int x, int y)
{
	mouseButtons[buton] = stare;
	mouseXClick = x;
	mouseYClick = y; 
  	// redeseneaza scena
	glutPostRedisplay();

	switch(buton)
	{
	case GLUT_LEFT_BUTTON:
		break;

	}
}

// GLUT callback
//  called on mouse movement
void motion (int x, int y)
{
	float factor=300;
	//if(buttons[GLUT_LEFT_BUTTON] == GLUT_DOWN)
	//	camera->MoveLeft((x-mouseXClick)/factor);

	if(mouseButtons[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
	{
		camera->RotateX((y-mouseYClick)/factor);
		camera->RotateY((x-mouseXClick)/factor);		
		// pozitionare camera
		camera->Render();
		pcamera->notifyCameraChange();
		manager->notifyCameraChange(); 
	}

	mouseXClick=x;
	mouseYClick=y;
	// redeseneaza scena
	glutPostRedisplay();
}   

// functia pentru procesarea evenimentelor de la tastatura
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		// cu W A S D 8 2 observatorul se misca prin scena
		case 'w' : case 'W' : case '8':
			camera->MoveForward(1); camera->Render(); pcamera->notifyCameraChange(); manager->notifyCameraChange(); break;
		case 's' : case 'S' : case '2':
			camera->MoveBackward(1); camera->Render(); pcamera->notifyCameraChange(); manager->notifyCameraChange();  break;
		case 'a' : case 'A' : case '4':
			camera->MoveLeft(1); camera->Render(); pcamera->notifyCameraChange();  manager->notifyCameraChange(); break;
		case 'd' : case 'D' : case '6':
			camera->MoveRight(1); camera->Render(); pcamera->notifyCameraChange(); manager->notifyCameraChange();  break;
		case 'f' : case 'F' :
			if(animationRunning==true)
			{
				glutIdleFunc(NULL);
				animationRunning=false;
			}
			else
			{
				glutIdleFunc(update);
				animationRunning=true;
			}
			break;
		case '.': case '>': 
			currentTest=(currentTest+1)%testCount; generateTest(currentTest); break;
		case ',': case '<': 
			currentTest=(currentTest-1);
			if(currentTest<0)
				currentTest=testCount-1;
			generateTest(currentTest);
			break;
		case '7':
			camera->MoveUpward(2); break;
		case '1':
			camera->MoveDownward(2); break;
		//cu ESC se iese din aplicatie
		case ESC:
			exit(0);
			break;
	}
	glutPostRedisplay();
}

// handler taste speciale
void keyboard(int key , int x, int y)
{
	Vector3D pozitie;
	
	switch (key)
	{
	// cu stanga/dreapta se misca obstacolul
	case GLUT_KEY_RIGHT :
		break;
	case GLUT_KEY_LEFT : 
		break;

	// cu F12 se intra/iese din modul fullscreen
	case GLUT_KEY_F10: 
		if(fullscreen==false)
		{
			glutFullScreen();
			fullscreen=true;
		} else
		{
			glutReshapeWindow(800,600);
			glutPositionWindow(200,100);
			fullscreen=false;
		}
		break;
	default: printf("Unrecognized special key pressed!\n");
	}
	
	glutPostRedisplay();
}


void reshape(int w, int h)
{
	// viewport de dimensiuni date
	glViewport(0,0, (GLsizei) w, (GLsizei) h);
	// calculare aspect ratio ( Width/ Height )
	GLfloat aspect = (GLfloat) w / (GLfloat) h;

	// intram in modul proiectie
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// incarcam matrice de perspectiva 
	gluPerspective(45, aspect, 0.2, 70);
	
	ecran_height = h;
	ecran_width = w;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(200,100);
	
	mainWindow = glutCreateWindow("Shadows");

	//de indata ce avem contextul ogl initallizam glew
	GLenum eroare = glewInit();
	glewIsSupported("GL_EXT_texture_filter_anisotropic");
	glewIsSupported("GL_ARB_multitexture");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);			//mouse click
	glutMotionFunc(motion);			//mouse movement
	glutKeyboardFunc(keyboard);		//normal key press
	glutSpecialFunc(keyboard);		//special key press
	glutIdleFunc(update);			//function to run in the idle time of the processor - can be null

	glutMainLoop();

	return 0;
}