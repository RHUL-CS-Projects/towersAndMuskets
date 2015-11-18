#ifndef FACEDIRECTIONSYSTEM_H
#define FACEDIRECTIONSYSTEM_H

#include <ComponentSystem.h>

class FaceDirectionSystem : public ComponentSystem {
private:
	float interpolateAngles(float a, float b, float c);
public:
	FaceDirectionSystem() : ComponentSystem("FaceDirectionSystem") {}
	
	void update(float timestep);
	void draw(float timestep);
};


#endif