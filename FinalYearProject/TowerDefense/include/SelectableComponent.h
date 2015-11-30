#ifndef SELECTABLECOMPONENT_H
#define SELECTABLECOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>
#include <iostream>

/**
 * Component to provide a selection circle for objects, and flag them
 * as selectable by the player
 */
class SelectableComponent : public GameComponent {	
private:

public:
	// Whether or not the object is selected
	bool selected = false;
	
	// X-scale of the selection circle
	float selectionXScale = 1;
	
	// Z-scale of the selection circle
	float selectionZScale = 1;
	
	// The plane mesh used for the selection circle
	irr::scene::IMesh* selectionMesh = nullptr;
	
	// The scene node for rendering the selection circle
	irr::scene::IMeshSceneNode* sceneNode = nullptr;
	
	SelectableComponent() : GameComponent("SelectableComponent") {}
};

#endif
