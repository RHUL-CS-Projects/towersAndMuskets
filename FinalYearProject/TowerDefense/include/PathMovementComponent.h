#ifndef PATHMOVEMENTCOMPONENT_H
#define PATHMOVEMENTCOMPONENT_H

#include <GameComponent.h>
#include <queue>

#include <irrlicht/irrlicht.h>

/**
 * Component to move an object along a specific path composed of a
 * set of 3D waypoints by interpolating the object's position over time.
 * This component has been largely deprecated by the SteeringComponent and
 * its use of NodePath
 */
class PathMovementComponent : public GameComponent {
private:

public:
	
	/**
	 * Initialises the movespeed and position on the path
	 */
	PathMovementComponent(float moveSpeed) : GameComponent("PathMovementComponent") {
		this->moveSpeed = moveSpeed;
		waypointPosition = 0;
		distToWaypoint = -1;
	}

	// List of 3D waypoints
	std::queue<irr::core::vector3df> waypoints;
	
	// The previous position on the path used for calculating a direction vector
	irr::core::vector3df prevPos;
	
	// Speed to move along the path
	float moveSpeed;
	
	// Size of the step that must be made towards the next node every update
	float stepSize;
	
	// Total number of steps that must be taken to reach the next node
	float totalSteps;
	
	// Current position between two waypoints as a percentage
	float waypointPosition;
	
	// Distance to the next waypoint
	float distToWaypoint;
};

#endif