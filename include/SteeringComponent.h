#ifndef STEERINGCOMPONENT_H
#define STEERINGCOMPONENT_H

#include <GameComponent.h>
#include <NodePath.h>
#include <irrlicht/irrlicht.h>

class SteeringComponent : public GameComponent {
private:
	
public:
	SteeringComponent(float maxSpeed, float mass) : GameComponent("SteeringComponent") {
		this->maxSpeed = maxSpeed;
		this->mass = mass;
	}
	
	NodePath path;
	float maxSpeed;
	float radius = 2;
	float mass;
	irr::core::vector3df velocity = irr::core::vector3df(0,0,0);
	irr::core::vector3df prevTargetDir = irr::core::vector3df(0,0,0);
};

#endif