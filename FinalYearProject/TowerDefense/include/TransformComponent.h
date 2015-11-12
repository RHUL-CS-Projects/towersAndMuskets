#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>

class TransformComponent : public GameComponent {
private:
	static std::string componentName;

public:
	irr::core::vector3df worldPosition;
	irr::core::vector3df rotation;
	irr::core::vector3df scale;
	
	TransformComponent(irr::core::vector3df position) : GameComponent("TransformComponent") {
		worldPosition = position;
		rotation = irr::core::vector3df(0,0,0);
		scale = irr::core::vector3df(1,1,1);
	}
};

#endif

