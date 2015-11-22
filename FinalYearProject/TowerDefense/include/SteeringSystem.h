#ifndef STEERINGSYSTEM_H
#define STEERINGSYSTEM_H

#include <ComponentSystem.h>
#include <irrlicht/irrlicht.h>
#include <SteeringComponent.h>
#include <TransformComponent.h>
#include <Quadtree.h>
#include <list>

class SteeringSystem : public ComponentSystem {
private:
	Quadtree* tempTree;
	void buildQuadtree(std::list<int> objects, Quadtree& root);
public:
	SteeringSystem() : ComponentSystem("SteeringSystem") {}
	
	void update(float timestep);
	void draw(float timestep);
	
	void seek(irr::core::vector3df targetPos, SteeringComponent* steerComp, TransformComponent* transComp);
	void avoid(irr::core::vector3df avoidPos, SteeringComponent* steerComp, TransformComponent* transComp);
};

#endif