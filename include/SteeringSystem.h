#ifndef STEERINGSYSTEM_H
#define STEERINGSYSTEM_H

#include <ComponentSystem.h>
#include <irrlicht/irrlicht.h>
#include <SteeringComponent.h>
#include <TransformComponent.h>

class SteeringSystem : public ComponentSystem {
private:
	
public:
	SteeringSystem() : ComponentSystem("SteeringSystem") {}
	
	void update(float timestep);
	void draw(float timestep);
	
	void seek(irr::core::vector3df targetPos, SteeringComponent* steerComp, TransformComponent* transComp);
	void avoid(irr::core::vector3df avoidPos, SteeringComponent* steerComp, TransformComponent* transComp);
};

#endif