#include <SelectionSystem.h>
#include <sfml/SFML/Window.hpp>
#include <RenderManager.h>
#include <iostream>

using namespace sf;
using namespace irr;
using namespace core;
using namespace video;

void SelectionSystem::update ( float timestep ) {
    
	if (Mouse::isButtonPressed(Mouse::Left)) {
		if (!dragging) {
			dragging = true;
			clickPos = vector2d<int>(Mouse::getPosition().x, Mouse::getPosition().y);
			mousePos = vector2d<int>(Mouse::getPosition().x, Mouse::getPosition().y);
		} else {
			mousePos = vector2d<int>(Mouse::getPosition().x, Mouse::getPosition().y);
			
			//recti* drawRect = new recti(clickPos.X, clickPos.Y, mousePos.X, mousePos.Y);
			recti* drawRect = new recti(20, 20, 200, 150);
				
// 			RenderManager::renderManager.getDriver()->draw2DRectangleOutline(*drawRect, SColor(255,255,255,255));	
		}
	} else {
		dragging = false;
	}
	
}

void SelectionSystem::draw ( float timestep ) {
	if (dragging) {
		recti drawRect(clickPos.X, clickPos.Y, mousePos.X, mousePos.Y);
		RenderManager::renderManager.getDriver()->draw2DRectangleOutline(drawRect, SColor(255,255,255,255));	
	}
}





