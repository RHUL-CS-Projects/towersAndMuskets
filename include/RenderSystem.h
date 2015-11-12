#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <string>
#include <ComponentSystem.h>
#include <ObjectManager.h>
#include <RenderComponent.h>
#include <AnimatedMeshComponent.h>
#include <TransformComponent.h>

class RenderSystem : public ComponentSystem {
private:
	void addSceneNode(RenderComponent* rendComp, AnimatedMeshComponent* animComp, TransformComponent* transComp);
public:
	RenderSystem() : ComponentSystem("RenderSystem") {}
	void update(float timestep);
};

#endif