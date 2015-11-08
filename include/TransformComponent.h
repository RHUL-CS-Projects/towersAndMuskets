#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>

class TransformComponent : public GameComponent {
public:
	irr::core::vector3df worldPosition;
	
	TransformComponent(irr::core::vector3df position) : GameComponent("TransformComponent") {
		worldPosition = position;
	}
};

#endif