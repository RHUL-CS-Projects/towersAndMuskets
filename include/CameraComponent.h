#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>

class CameraComponent : public GameComponent {	
private:

public:
	bool active = true;
	
	CameraComponent(bool active) : GameComponent("CameraComponent") {
		this->active = active;
	}
};

#endif
