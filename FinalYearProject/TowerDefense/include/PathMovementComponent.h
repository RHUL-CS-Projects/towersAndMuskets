#ifndef PATHMOVEMENTCOMPONENT_H
#define PATHMOVEMENTCOMPONENT_H

#include <GameComponent.h>
#include <queue>

#include <irrlicht/irrlicht.h>

class PathMovementComponent : public GameComponent {
private:
public:
	PathMovementComponent(float moveSpeed) : GameComponent("PathMovementComponent") {
		this->moveSpeed = moveSpeed;
		waypointPosition = 0;
		distToWaypoint = -1;
	}

	std::queue<irr::core::vector3df> waypoints;
	irr::core::vector3df prevPos;
	float moveSpeed;
	float stepSize;
	float totalSteps;
	float waypointPosition;
	float distToWaypoint;
};

#endif