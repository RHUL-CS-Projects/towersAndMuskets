#ifndef SELECTABLECOMPONENT_H
#define SELECTABLECOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>
#include <iostream>

class SelectableComponent : public GameComponent {	
private:

public:
	bool selected = false;
	float selectionXScale = 1;
	float selectionZScale = 1;
	irr::scene::IMesh* selectionMesh = nullptr;
	irr::scene::IMeshSceneNode* sceneNode = nullptr;
	
	SelectableComponent() : GameComponent("SelectableComponent") {}
};

#endif
