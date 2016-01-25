#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>
#include <RenderManager.h>

/**
 * Component that stores an Irrlicht scene node for rendering
 * a mesh to the screen
 */
class RenderComponent : public GameComponent {
private:
	
public:
	// Whether or not the object is visible
	bool visible;
	
	// The object's scene node
	irr::scene::IAnimatedMeshSceneNode* sceneNode = nullptr;

	RenderComponent() : GameComponent("RenderComponent") {}
	
	/**
	 * Alternate constructor to initialise the object's visibility
	 */
	RenderComponent(bool visible) : GameComponent("RenderComponent") {
		this->visible = visible;
	}
};

#endif

