#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>

/**
 * Component to hold information about an object's position, rotation
 * and scale
 */
class TransformComponent : public GameComponent {
private:
	// The component's name
	static std::string componentName;

public:
	
	// The object's position
	irr::core::vector3df worldPosition;
	
	// The rotation of the object as Euler angles
	irr::core::vector3df rotation;
	
	// The object's scale
	irr::core::vector3df scale;
	
	/**
	 * Initialises the position, rotation and scale of the object
	 */
	TransformComponent(irr::core::vector3df position) : GameComponent("TransformComponent") {
		worldPosition = position;
		rotation = irr::core::vector3df(0,0,0);
		scale = irr::core::vector3df(1,1,1);
	}
};

#endif

