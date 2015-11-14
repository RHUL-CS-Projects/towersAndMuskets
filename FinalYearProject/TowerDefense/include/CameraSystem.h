#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <string>
#include <list>

#include <ComponentSystem.h>
#include <RenderComponent.h>
#include <AnimatedMeshComponent.h>
#include <TransformComponent.h>

class CameraSystem : public ComponentSystem {	
public:
	CameraSystem() : ComponentSystem("CameraSystem") {}
	void update(float timestep);
	void draw(float timestep);
};


#endif