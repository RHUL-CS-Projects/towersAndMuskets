#ifndef STEERINGCOMPONENT_H
#define STEERINGCOMPONENT_H

#include <GameComponent.h>
#include <NodePath.h>
#include <irrlicht/irrlicht.h>

/**
 * Component for applying steering behaviours to an object to move
 * along a path
 */
class SteeringComponent : public GameComponent {
private:
	
public:
	/**
	 * Initialises the object's speed and mass
	 */
	SteeringComponent(float maxSpeed, float mass, float radius = 4) : GameComponent("SteeringComponent") {
		this->maxSpeed = maxSpeed;
		this->mass = mass;
		this->radius = radius;
	}
	
	// The path to be followed
	NodePath path;
	
	// The max speed the object should move at
	float maxSpeed;
	
	// Radius threshold for marking a path node as visited
	float radius;
	
	// The object's mass used when calculating the turn rate
	float mass;
	
	// The current velocity of the object
	irr::core::vector3df velocity = irr::core::vector3df(0,0,0);
	
	// The previous direction to the target (used when drawing debug overlay)
	irr::core::vector3df prevTargetDir = irr::core::vector3df(0,0,0);
	
	// Whether the component is enabled. Allows other components to take control of unit movement/positioning
	bool enabled = true;
};

#endif