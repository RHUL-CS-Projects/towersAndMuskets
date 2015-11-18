#ifndef FACEDIRECTIONCOMPONENT_H
#define FACEDIRECTIONCOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>

class FaceDirectionComponent : public GameComponent {
private:

public:
	FaceDirectionComponent(float startYRot, float rotSpeed) : GameComponent("FaceDirectionComponent") {
		targetYRot = startYRot;
		currentYRot = startYRot;
		rotateSpeed = rotSpeed;
	}
	
	float targetYRot;
	float currentYRot;
	float rotateSpeed;
};


#endif