#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <string>
#include <list>

#include <ComponentSystem.h>
#include <RenderComponent.h>
#include <AnimatedMeshComponent.h>
#include <TransformComponent.h>

/**
 * System for updating the position of the camera to the current
 * active camera component's position
 */
class CameraSystem : public ComponentSystem {	
public:
	CameraSystem() : ComponentSystem("CameraSystem") {}
	void update(float timestep);
	void draw(float timestep);
};


#endif