#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>

/**
 * Component to specify that an object should be used as
 * the game's camera
 */
class CameraComponent : public GameComponent {	
private:

public:
	// Whether or not the camera is active
	bool active = true;
	
	/**
	 * Initialises the camera's state
	 */
	CameraComponent(bool active) : GameComponent("CameraComponent") {
		this->active = active;
	}
};

#endif
