#include <SelectionSystem.h>
#include <sfml/SFML/Window.hpp>
#include <RenderManager.h>
#include <iostream>
#include <EventReceiver.h>
#include <ObjectManager.h>
#include <list>
#include <TransformComponent.h>
#include <SelectableComponent.h>

using namespace sf;
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

void SelectionSystem::update ( float timestep ) {
	EventReceiver::SMouseState mouseState = EventReceiver::getMouseState();
	
	if (mouseState.leftPressed) {
		if (!dragging) {
			dragging = true;
			clickPos = vector2d<int>(mouseState.position.X, mouseState.position.Y);
			mousePos = vector2d<int>(mouseState.position.X, mouseState.position.Y);
		} else {
			mousePos = vector2d<int>(mouseState.position.X, mouseState.position.Y);
		}
		
		selectObjects();
	} else {
		dragging = false;
	}
	
}

void SelectionSystem::draw ( float timestep ) {
	if (dragging) {
		recti drawRect(clickPos.X, clickPos.Y, mousePos.X, mousePos.Y);
		RenderManager::renderManager.getDriver()->draw2DRectangleOutline(drawRect, SColor(150,255,255,255));
	}
}

void SelectionSystem::selectObjects() {
	ObjectManager* mgr = &ObjectManager::manager;
	
	// Get all selectable objects
	std::list<int> objects = mgr->getObjectsWithComponent("SelectableComponent");
	
	for (int i : objects) {
		
		SelectableComponent* selectComp = mgr->getObjectComponent<SelectableComponent>(i, "SelectableComponent");
		
		// Check if the object has a transform
		TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
		
		if (transComp == nullptr)
			continue;
		
		vector2di screenPos = worldToScreen(transComp->worldPosition);
		
		int topLeftX = (clickPos.X < mousePos.X) ? clickPos.X : mousePos.X;
		int topLeftY = (clickPos.Y < mousePos.Y) ? clickPos.Y : mousePos.Y;
		int bottomRightX = (clickPos.X > mousePos.X) ? clickPos.X : mousePos.X;
		int bottomRightY = (clickPos.Y > mousePos.Y) ? clickPos.Y : mousePos.Y;
		
		if (screenPos.X >= topLeftX && screenPos.X <= bottomRightX &&
			screenPos.Y >= topLeftY && screenPos.Y <= bottomRightY)
			selectComp->selected = true;
		else
			selectComp->selected = false;
	}
}

vector2di SelectionSystem::worldToScreen ( vector3df point ) {
	
	matrix4 viewMatrix = RenderManager::renderManager.getSceneManager()->getActiveCamera()->getViewMatrix();
	matrix4 projMatrix = RenderManager::renderManager.getSceneManager()->getActiveCamera()->getProjectionMatrix();
	
	matrix4 worldMatrix = RenderManager::renderManager.getDriver()->getTransform(video::ETS_WORLD);
	
	u32 width = RenderManager::renderManager.getDriver()->getScreenSize().Width;
	u32 height = RenderManager::renderManager.getDriver()->getScreenSize().Height;
	
	matrix4 viewProjMatrix = projMatrix * viewMatrix;
	
	f32 worldPosMat[4] = {point.X, point.Y, point.Z, 1};
	f32* ptrWorldPos = worldPosMat;	
	viewProjMatrix.multiplyWith1x4Matrix(ptrWorldPos);
	
	worldPosMat[0] /=  worldPosMat[3];
	worldPosMat[1] /=  worldPosMat[3];
	worldPosMat[2] /=  worldPosMat[3];

	int xpos = (int)round(width * (worldPosMat[0] + 1.0) / 2.0);
	int ypos = (int)round(height * (1.0 - ((worldPosMat[1] + 1.0) / 2.0)));
	
	return vector2di(xpos, ypos);
}









