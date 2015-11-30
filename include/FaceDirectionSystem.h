#ifndef FACEDIRECTIONSYSTEM_H
#define FACEDIRECTIONSYSTEM_H

#include <ComponentSystem.h>

/**
 * System to update the facing direction (Y-axis rotation) of a game
 * object
 */
class FaceDirectionSystem : public ComponentSystem {
private:
	
	/**
	 * Linearly interpolates between two angles, taking into account the shorter
	 * distance around the circle
	 */
	float interpolateAngles(float a, float b, float c);
public:
	FaceDirectionSystem() : ComponentSystem("FaceDirectionSystem") {}
	
	/**
	 * Interpolates current rotation of objects towards their target rotations
	 */
	void update(float timestep);
	void draw(float timestep);
};


#endif