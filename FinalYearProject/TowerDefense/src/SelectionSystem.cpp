#include <SelectionSystem.h>
#include <sfml/SFML/Window.hpp>
#include <RenderManager.h>
#include <iostream>
#include <EventReceiver.h>
#include <ObjectManager.h>
#include <list>
#include <TransformComponent.h>
#include <SelectableComponent.h>
#include <Game.h>

using namespace sf;
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

void SelectionSystem::update ( float timestep ) {
	EventReceiver::SMouseState mouseState = *EventReceiver::getMouseState();

	// Calculate click point on terrain
	ISceneCollisionManager* colmgr = Game::game.getRendMgr()->getSceneManager()->getSceneCollisionManager();
	line3df ray = colmgr->getRayFromScreenCoordinates(EventReceiver::getMouseState()->position);
	
	vector3df point;
	triangle3df triangle;
	ISceneNode* node;
	
	int hitID = -1;
	if ((hitID = colmgr->getSceneNodeFromRayBB(ray)->getID()) > -1) {	
		//std::cout << "mouse over" << std::endl;
	}
	
	if (mouseState.leftPressed) {
		if (!dragging) {
			dragging = true;
			clickPos = vector2d<int>(mouseState.position.X, mouseState.position.Y);
			mousePos = vector2d<int>(mouseState.position.X, mouseState.position.Y);
		} else {
			mousePos = vector2d<int>(mouseState.position.X, mouseState.position.Y);
		}
	} else {
		if (dragging) {
			selectObjects();
			dragging = false;

			int hitID = -1;
			if ((hitID = EventReceiver::getHoverObject()) > -1) {	
				if (Game::game.getObjMgr()->getObjectComponent<SelectableComponent>(hitID, "SelectableComponent") != nullptr) {	
					Game::game.getObjMgr()->getObjectComponent<SelectableComponent>(hitID, "SelectableComponent")->selected = true;
				}
			}
		}
	}
}

void SelectionSystem::draw ( float timestep ) {
	if (dragging) {
		recti drawRect(clickPos.X, clickPos.Y, mousePos.X, mousePos.Y);
		
		SMaterial m;
		m.Lighting = false;
		m.Thickness = 1.0f;
		Game::game.getRendMgr()->getDriver()->setMaterial(m);
		Game::game.getRendMgr()->getDriver()->draw2DRectangleOutline(drawRect, SColor(150,255,255,255));
	}
}

void SelectionSystem::selectObjects() {
	ObjectManager* mgr = Game::game.getObjMgr();
	
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
	
	matrix4 viewMatrix = Game::game.getRendMgr()->getSceneManager()->getActiveCamera()->getViewMatrix();
	matrix4 projMatrix = Game::game.getRendMgr()->getSceneManager()->getActiveCamera()->getProjectionMatrix();
	
	matrix4 worldMatrix = Game::game.getRendMgr()->getDriver()->getTransform(video::ETS_WORLD);
	
	u32 width = Game::game.getRendMgr()->getDriver()->getScreenSize().Width;
	u32 height = Game::game.getRendMgr()->getDriver()->getScreenSize().Height;
	
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









