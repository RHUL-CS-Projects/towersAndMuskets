#ifndef PATHMOVEMENTSYSTEM_H
#define PATHMOVEMENTSYSTEM_H

#include <ComponentSystem.h>

class PathMovementSystem : public ComponentSystem {
private:
	
public:
	PathMovementSystem() : ComponentSystem("PathMovementSystem") {}
	
	void update(float timestep);
	void draw(float timestep);
};

#endif