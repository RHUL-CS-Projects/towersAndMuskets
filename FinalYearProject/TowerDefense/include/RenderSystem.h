#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <string>

#include <ComponentSystem.h>
#include <RenderComponent.h>
#include <AnimatedMeshComponent.h>
#include <TransformComponent.h>

/**
 * Uses an object's mesh and transform components to render it to
 * the screen. Creates a scene node in Irrlicht's scene manager if
 * one has not already been created for the object
 */
class RenderSystem : public ComponentSystem {
private:
	/**
	 * Adds a scene node to the scene manager for an object
	 */
	void addSceneNode(RenderComponent* rendComp, AnimatedMeshComponent* animComp, TransformComponent* transComp);
public:
	RenderSystem() : ComponentSystem("RenderSystem") {}
	
	/**
	 * Updates the positions of game objects based on their transform components, and
	 * creates any scene nodes if necessary
	 */
	void update(float timestep);
	
	/**
	 * Tells Irrlicht's scene manager to render all objects
	 */
	void draw(float timestep);
};

#endif