#ifndef PATHMOVEMENTSYSTEM_H
#define PATHMOVEMENTSYSTEM_H

#include <ComponentSystem.h>

/**
 * Interpolates an object's position over time along a path
 */
class PathMovementSystem : public ComponentSystem {
private:
	
public:
	PathMovementSystem() : ComponentSystem("PathMovementSystem") {}
	
	/**
	 * Updates the position of the object based on the path movement
	 * speed
	 */
	void update(float timestep);
	void draw(float timestep);
};

#endif