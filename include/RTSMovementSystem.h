#ifndef RTSMOVEMENTSYSTEM_H
#define RTSMOVEMENTSYSTEM_H

#include <ComponentSystem.h>

/**
 * System to update a selected object's path when the map is right-clicked
 */
class RTSMovementSystem : public ComponentSystem {
private:
	bool mouseClicked = false;
public:
	RTSMovementSystem() : ComponentSystem("RTSMovementSystem") {}	
	
	/**
	 * Adds waypoints to the object's path when the right-mouse button
	 * is pressed
	 */
	void update(float timestep);
	
	void draw(float timestep);
};


#endif