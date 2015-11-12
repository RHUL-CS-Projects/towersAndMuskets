#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>

class RenderComponent : public GameComponent {
private:
	
public:
	bool visible;
	irr::scene::IAnimatedMeshSceneNode* sceneNode = nullptr;

	RenderComponent() : GameComponent("RenderComponent") {}
	RenderComponent(bool visible) : GameComponent("RenderComponent") {
		this->visible = visible;
	}
};

#endif

