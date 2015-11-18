#ifndef RTSMOVEMENTSYSTEM_H
#define RTSMOVEMENTSYSTEM_H

#include <ComponentSystem.h>

class RTSMovementSystem : public ComponentSystem {
private:
	bool mouseClicked = false;
public:
	RTSMovementSystem() : ComponentSystem("RTSMovementSystem") {}	
	
	void update(float timestep);
	void draw(float timestep);
};


#endif