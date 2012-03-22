/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#include <vector>
#include <assert.h>

#include "Particle.h"
#include "ParticleAffector.h"
#include "ParticleAttractor.h"
#include "ParticleEmitter.h"
#include "ParticleCamera.h"

using namespace std;

/***
 * PARTICLE SYSTEM CLASS
 * 
 * The class that describes the whole particle system. Coordinates the generation, update and drawing of the particles.
 * 
 * The main information about particles are stored mainly in the Particle System: particle positions, velocities and colors.
 * For drawing VBO is used, using Vertex Data and Color Data (uploaded to the GPU (VBO) at every draw using STREAM_DRAW) & Texture Coords Data and Normals Data (uploaded 
 * to the GPU (VBO) only at the Particle System initialization).
 */
class ParticleSystem
{
public:
	//Constructors
	__declspec(dllexport) ParticleSystem(int _particleLimit, int _particlesPerGeneration, bool _oneTimeGeneration=false, ParticleEmitter* _emitter=NULL, ParticleCamera* _camera=NULL);
	__declspec(dllexport) ~ParticleSystem(void);
	
	//Methods
	//updates the whole particle system (generates new particles & updates existing particles)
	__declspec(dllexport) void update();			
	//draws all the particles in the system
	__declspec(dllexport) void draw();				
	//count of alive particles in the system
	__declspec(dllexport) int getParticleCount();	
	
	//adds a new attractor
	__declspec(dllexport) void addAttractor(ParticleAttractor* );
	//adds a new affector
	__declspec(dllexport) void addAffector(ParticleAffector* );
	//sets a new emitter
	__declspec(dllexport) void setEmitter(ParticleEmitter* emitter);
	//sets a new camera
	__declspec(dllexport) void setCamera(ParticleCamera* camera);
	//sets the texture (if texture is -1, the particles are not textured)
	inline __declspec(dllexport) void setTexture(GLuint _texture) { if(_texture>=0) {texture=_texture; textured=true;} else textured=false; }
	//notifies the particle system that it should update the vertexes of the particles, as the camera position has changed.
	__declspec(dllexport) void notifyCameraChange();

private:
	//adds a new particle to the particles array
	void addParticle(ParticleData* _particle);
	//removes the particle at given index in the particles array
	void removeParticle(int index);
	//updates all the 4 vertices of the particle
	void updateParticle(Vector3D *particlePos, GLfloat* vertexData, const GLfloat *viewMatrix, GLfloat particleSize);

private:
	//Attributes
	int		firstParticle, lastParticle;	//indexes of the first & last particles in the particles array
	int		particleLimit;					//number of maximum particles in the system
	bool oneTimeGeneration;					//if true, the system only generates particleLimit particles and then stop generating. If false, the system continues to generate particles until particleLimit cells are generated.
	int		particlesLeft;					//if one time generation is active, it counts the number of particles left to generate.
	ParticleEmitter*			emitter;	//the emitter that generates the particles
	ParticleCamera*				camera;		//the ParticleCamera that provides information about the camera (-> for use during particle drawing)
	vector<ParticleAttractor*>	attractors;	//the attractors, that affect the particles at a given range, such as a magnet
	vector<ParticleAffector*>	affectors;	//the affectors, that affect the particles at any range, such as gravity
	GLuint texture;							//the texture that is applied to the particles
	boolean textured;						//if the ParticleSysyem is textured
	boolean selfGeneratedEmitter;

	//Particle Data
	Particle**	particles;	//the particles of the system
	Vector3D*	particlesPositions;		//the positions of the particles in the system (in world-space coordinates)
	Vector3D*	particlesVelocities;	//the velocity vectors for the particles (position += velocity)
	GLfloat*	vertexData;				//the data for the vertexes positions
	GLfloat*	colorData;				//the data for the colors of the particleS (its vertex colors), which is combined with the texture (if any)
	//Texture and normal data are not changed (the normal always faces the user and the texcoords are always 00,01,11,10) so they are only used at initialization
	//and copied in the VBO for STATIC_DRAW
	GLfloat*	textureData;			//the data for the texture coordinates of the vertexes of the particles
	//GLfloat*	normalData;				//the data for the normals of the vertexes of the particles
	GLuint BufferName[4];				//the VBO buffers for the data

public:
	Vector3D position;						//the position of the particle system
	int	particlesPerGeneration;				//maximum number of new particles generated at each generation step, until the limit is reached


};

