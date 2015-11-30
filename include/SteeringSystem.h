#ifndef STEERINGSYSTEM_H
#define STEERINGSYSTEM_H

#include <ComponentSystem.h>
#include <irrlicht/irrlicht.h>
#include <SteeringComponent.h>
#include <TransformComponent.h>
#include <Quadtree.h>
#include <list>

/**
 * System to update the positions of objects using steering behaviours
 * to move them along a path
 */
class SteeringSystem : public ComponentSystem {
private:
	// Quadtree used to optimise collision detection
	Quadtree* tempTree;
	
	/**
	 * Places all objects into the quadtree for collision detection
	 */
	void buildQuadtree(std::list<int> objects, Quadtree& root);
public:
	SteeringSystem() : ComponentSystem("SteeringSystem") {}
	
	/**
	 * Uses steering behaviours to update objects and applies collision
	 * avoidance
	 */
	void update(float timestep);
	
	/**
	 * Draws the quadtree for debugging
	 */
	void draw(float timestep);
	
	/**
	 * Calulcates and applies a force to steer towards a point
	 */
	void seek(irr::core::vector3df targetPos, SteeringComponent* steerComp, TransformComponent* transComp);
	
	/**
	 * Calulcates and applies a force to avoid a point 
	 */
	void avoid(irr::core::vector3df avoidPos, SteeringComponent* steerComp, TransformComponent* transComp);
};

#endif