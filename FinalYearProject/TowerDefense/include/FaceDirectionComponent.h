#ifndef FACEDIRECTIONCOMPONENT_H
#define FACEDIRECTIONCOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>

/**
 * Component for describing the direction an object
 * should be facing
 */
class FaceDirectionComponent : public GameComponent {
private:

public:
	// The target Y rotation of the object
	float targetYRot;
	
	// The current Y rotation
	float currentYRot;
	
	// Percent the current rot should interpolate towards the target each update
	float rotateSpeed;
	
	/**
	 * Initalises the target and current rotations, as well as the rotation
	 * interpolation percent
	 */
	FaceDirectionComponent(float startYRot, float rotSpeed) : GameComponent("FaceDirectionComponent") {
		targetYRot = startYRot;
		currentYRot = startYRot;
		rotateSpeed = rotSpeed;
	}
};


#endif