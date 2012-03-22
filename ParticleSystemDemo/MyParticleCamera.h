#pragma once
#include "ParticleCamera.h"
#include "Camera.h"

class MyParticleCamera : public ParticleCamera
{
public:
	MyParticleCamera(Camera *camera)
	{ this->camera=camera; }

	// Returns sixteen values: the view matrix
	inline const GLfloat* getViewMatrix() { return viewMatrix; }
	
	// Notifies the ParticleCamera that the camera position has changed
	virtual void notifyCameraChange() { viewMatrix = camera->getViewMatrix(); }

private:
	Camera *camera;
	const GLfloat *viewMatrix;
};

