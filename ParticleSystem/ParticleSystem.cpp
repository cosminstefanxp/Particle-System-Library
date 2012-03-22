/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/
#pragma comment(lib,"glew32.lib")

#include <glew.h>
#include <glut.h>
#include "ParticleSystem.h"

enum
{
    VERTEX_DATA = 0,
    COLOR_DATA = 2,
	TEXTURE_DATA = 1,
	NORMAL_DATA = 3
};
#define VBO_COUNT 4

ParticleSystem::ParticleSystem(int _particleLimit, int _particlesPerGeneration, bool _oneTimeGeneration, ParticleEmitter* _emitter, ParticleCamera* _camera)
{
	this->particleLimit=_particleLimit;
	this->particlesPerGeneration=_particlesPerGeneration;
	this->particles=new Particle*[_particleLimit];
	this->particlesPositions=new Vector3D[_particleLimit];
	this->particlesVelocities=new Vector3D[_particleLimit];

	this->vertexData=new GLfloat[_particleLimit*4*3];	// 4 vertixes per particle, each with 3 coordinates
	this->colorData=new GLfloat[_particleLimit*4*3];	// 4 vertixes per particle, each with 3 coordinates

	this->firstParticle=0;
	this->lastParticle=-1;
	this->oneTimeGeneration=_oneTimeGeneration;
	this->particlesLeft=particleLimit;
	this->camera=_camera;
	this->textured=false;

	if(_emitter==NULL)
	{
		this->emitter=new ParticleEmitter(1,Vector3D(1,1,1),100);
		selfGeneratedEmitter=true;
	}
	else
	{
		this->emitter=_emitter;
		selfGeneratedEmitter=false;
	}

	//Init texture data
	textureData=new GLfloat[_particleLimit*4*2];	// 4 vertixes per particle, each with 2 coordinates
	for(int j=0;j<particleLimit;j++)
	{
		int i=j*8;
		//Tex coords are 00,01,11,10
		textureData[i]=textureData[i+1] = 0;
		textureData[i+2]=1.0f; textureData[i+3]=0.0f;
		textureData[i+4]=1.0f; textureData[i+5]=1.0f;
		textureData[i+6]=0.0f; textureData[i+7]=1.0f;
	}
	//Init normal data
	GLfloat* normalData=new GLfloat[_particleLimit*4*3];	// 4 vertixes per particle, each with 3 coordinates
	for(int j=0;j<particleLimit;j++)
	{
		int i=j*12;
		//The normals should face the camera (0,0,1)
		normalData[i]=normalData[i+1]=normalData[i+3]=normalData[i+4]=normalData[i+6]=normalData[i+7]=normalData[i+9]=normalData[i+10]=1;
		normalData[i+2]=normalData[i+5]=normalData[i+8]=normalData[i+11]=1;
	}

	//Generate and initialize VBO buffers
	glGenBuffers(VBO_COUNT, BufferName);

	//Upload Normals Data
	glBindBuffer(GL_ARRAY_BUFFER, BufferName[NORMAL_DATA]);
	glBufferData(GL_ARRAY_BUFFER, _particleLimit*4*3, normalData, GL_STATIC_DRAW);
	glNormalPointer(GL_FLOAT, 0, 0);



	//Free used memory
	//delete[] textureData;
	delete[] normalData;
}


ParticleSystem::~ParticleSystem(void)
{
	for(int i=firstParticle;i<=lastParticle;i++)
		delete particles[i];
	delete [] particles;
	delete [] particlesPositions;
	delete [] particlesVelocities;
	delete [] colorData;
	delete [] vertexData;
	if(selfGeneratedEmitter)
		delete emitter;

	// delete VBO when program terminated
	glDeleteBuffers(VBO_COUNT, BufferName);
}

//updates the whole particle system (generates new particles & updates existing particles)
void ParticleSystem::update()
{
	ParticleData pData;
	Particle *particle;
	int count;
	Vector3D *posTemp, *velTemp;

	//UPDATE
	assert(camera!=NULL);
	const GLfloat *mat=camera->getViewMatrix();
	for(register int i=lastParticle;i>=firstParticle;i--)	//reverse order to simply the removal -> no more skipping
	{
		//save the adresses now, in order to optimize memory access
		particle=particles[i];
		posTemp=particlesPositions+i;
		velTemp=particlesVelocities+i;

		//update the particles velocity according to the ATTRACTORS
		count=attractors.size();
		for(int j=0; j<count;j++)
			attractors[j]->attractParticle(posTemp,velTemp);

		//update the particles velocity according to the AFFECTORS
		count=affectors.size();
		for(int j=0;j<count;j++)
			affectors[j]->affectParticle(posTemp,velTemp);

		//update the new position and vertexes of the particle
		*posTemp+=*velTemp;
		this->updateParticle(posTemp,&vertexData[i*12],mat,particle->size);
		particle->updatePosition();
		//check if it should be removed
		if(particle->hasDied())
			removeParticle(i);
	}

	//GENERATION
	assert(emitter!=NULL);
	//Generate and add 'particlesPerGeneration' new particles.
	if(!oneTimeGeneration)	//if we generate continously
		for(int i=0;i<particlesPerGeneration;i++)
		{
			pData=emitter->generateParticle();
			pData.position+=this->position;
			this->addParticle(&pData);
		}
	else	//if one time generation is active, we stop after we reach particleLimit generated items
		for(int i=0;particlesLeft>0 && i<particlesPerGeneration;i++,particlesLeft--)
		{
			pData=emitter->generateParticle();
			pData.position+=this->position;
			this->addParticle(&pData);
		}
}			
//draws all the particles in the system
void ParticleSystem::draw()
{
	assert(camera!=NULL);

	//Translate the drawing to the required position
	glPushMatrix();
	glTranslatef(position.x,position.y,position.z);
	glColor3f(1,0,0);
	glutSolidCube(0.05);
	//Pop the matrix back
	glPopMatrix();

	//Draw the particles
	glPushMatrix();
	glLoadIdentity();	//the particles are drawn directly in camera space coordinates
	int particleCount=lastParticle-firstParticle+1;
	int dataSize=particleCount*12*sizeof(float);	//Same data size for colors and vertexes (3 colors and 3 coordinates for each of the 4 vertexes of the quads)

	//Apply texture, if any
	if(textured)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,texture);
	}


	/*** SIMPLE VBO VERSION ***/

	//Upload Color Data
    glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_DATA]);
	glBufferData(GL_ARRAY_BUFFER, dataSize, colorData, GL_STREAM_DRAW);
	glColorPointer(3, GL_FLOAT, 0, 0);

	//Upload Vertex Data
    glBindBuffer(GL_ARRAY_BUFFER, BufferName[VERTEX_DATA]);
	glBufferData(GL_ARRAY_BUFFER, dataSize, vertexData, GL_STREAM_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, 0);

	//Upload Texture Data
	//Initially, the optimised version did the uploading only at initialization, using STATIC_DRAW, but due to my video card, the functioning was not 100% functional
	glBindBuffer(GL_ARRAY_BUFFER, BufferName[TEXTURE_DATA]);
	glBufferData(GL_ARRAY_BUFFER, particleCount*8*sizeof(float), textureData, GL_DYNAMIC_DRAW);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	/*** MAPPING VERSION ***/

	////Map the color buffer
	//glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_DATA]);
	//glBufferData(GL_ARRAY_BUFFER, dataSize, NULL, GL_STREAM_DRAW);
	//GLvoid* ColorBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	////Map the vertex buffer
	//glBindBuffer(GL_ARRAY_BUFFER, BufferName[VERTEX_DATA]);
	//glBufferData(GL_ARRAY_BUFFER, dataSize, NULL, GL_STREAM_DRAW);
	//GLvoid* VertexBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	////Copy the data
	//memcpy(ColorBuffer, colorData, dataSize);
	//memcpy(VertexBuffer, vertexData, dataSize);

	////Unmap data and set color pointer
	//glBindBuffer(GL_ARRAY_BUFFER, BufferName[COLOR_DATA]);
	//glUnmapBuffer(GL_ARRAY_BUFFER);
	//glColorPointer(3, GL_FLOAT, 0, 0);

	////Unmap data and set vertex pointer
	//glBindBuffer(GL_ARRAY_BUFFER, BufferName[VERTEX_DATA]);
	//glUnmapBuffer(GL_ARRAY_BUFFER);
	//glVertexPointer(3, GL_FLOAT, 0, 0);

	/*** DRAWING ***/

	//Draw the particles
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	

	glDrawArrays(GL_QUADS, 0, particleCount*4);
    
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
	

	//Disable Texture, if any
	if(textured)
		glDisable(GL_TEXTURE_2D);

	///Restore the ModelView Matrix
	glPopMatrix();
}

//count of alive particles in the system
int ParticleSystem::getParticleCount()
{ return lastParticle-firstParticle+1; }	
	
//adds a new attractor
void ParticleSystem::addAttractor(ParticleAttractor* attractor)
{ attractors.push_back(attractor); }

//adds a new affector
void ParticleSystem::addAffector(ParticleAffector* affector)
{ affectors.push_back(affector); }

//sets a new emitter
void ParticleSystem::setEmitter(ParticleEmitter* _emitter)
{
	if(selfGeneratedEmitter)
	{
		delete emitter;
		selfGeneratedEmitter=false;
	}
	this->emitter=_emitter; 
}

//sets a new camera
void ParticleSystem::setCamera(ParticleCamera* _camera)
{ this->camera=_camera; }

//adds a new particle to the particles array
void ParticleSystem::addParticle(ParticleData* _particle) 
{
	if(lastParticle < this->particleLimit-1)
	{
		particles[++lastParticle]=_particle->particle;
		particlesPositions[lastParticle]=_particle->position;
		particlesVelocities[lastParticle]=_particle->velocity;
		//Add vertex data
		updateParticle(&particlesPositions[lastParticle],&vertexData[lastParticle*12],camera->getViewMatrix(),_particle->particle->size);
		//Add color data
		for(int i=0;i<4;i++)
		{
			colorData[lastParticle*12+i*3]=_particle->color.x;
			colorData[lastParticle*12+i*3+1]=_particle->color.y;
			colorData[lastParticle*12+i*3+2]=_particle->color.z;
		}
	}
	else
	{
		//delete the particle so not to lose memory
		delete _particle->particle;
		//fprintf(stderr,"[ERROR] No more new particles can be added. The system can only handle %d particles.\n",particleLimit);
	}
}

//removes the particle at given index in the particles array
void ParticleSystem::removeParticle(int index) 
{
	//free use memory
	delete particles[index];
	//delete the particle
	particles[index]=particles[lastParticle];
	particlesPositions[index]=particlesPositions[lastParticle];
	particlesVelocities[index]=particlesVelocities[lastParticle];
	
	//Copy color and vertex data
	for(int i=0;i<12;i++)
	{
		vertexData[index*12+i]=vertexData[lastParticle*12+i];
		colorData[index*12+i] =colorData[lastParticle*12+i];
	}

	lastParticle--;
}

//updates all the 4 vertices of the quad shape of the particle
void ParticleSystem::updateParticle(Vector3D *position, GLfloat* vertexData, const GLfloat *viewMatrix, GLfloat particleSize)
{
	//Compute the position of the particle in CameraSpace coordinates
	Vector4D pos(position->x,position->y,position->z,1);
	Vector4D res=pos.multiply(viewMatrix);
	GLfloat t;

	//The z coordinate is the same
	vertexData[2]=vertexData[5]=vertexData[8]=vertexData[11]=res.z/res.a;	

	//The x coordinate
	t=res.x/res.a;
	vertexData[0]=vertexData[9]=t-particleSize;
	vertexData[3]=vertexData[6]=t+particleSize;

	//The y coordinate
	t=res.y/res.a;
	vertexData[1]=vertexData[4]=t+particleSize;
	vertexData[7]=vertexData[10]=t-particleSize;
}

//notifies the particle system that it should update the vertexes of the particles, as the camera position has changed.
void ParticleSystem::notifyCameraChange()
{
	const GLfloat *mat=camera->getViewMatrix();
	for(int i=firstParticle;i<=lastParticle;i++)
		this->updateParticle(particlesPositions+i,&vertexData[i*12],mat,particles[i]->size);
}