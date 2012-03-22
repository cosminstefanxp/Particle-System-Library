/****
 * Sisteme de prelucrare Grafica
          o Stefan-Dobrin Cosmin
          o 342C4
 ****/
#pragma once

/***
 * PARTICLE CAMERA CLASS
 * 
 * The class that gets information from the system camera, in order to be used for particle vertexes generation. This is an abstract class and should be extended.
 */
class ParticleCamera
{
public:
	// Returns sixteen values: the view matrix
	virtual const GLfloat* getViewMatrix() = 0;
	// Notifies the ParticleCamera that the camera position has changed
	virtual void notifyCameraChange() = 0;
};

