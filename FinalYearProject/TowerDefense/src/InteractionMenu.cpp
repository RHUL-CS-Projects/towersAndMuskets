#include <InteractionMenu.h>
#include <irrlicht/irrlicht.h>
#include <RenderManager.h>
#include <string>
#include <iostream>
#include <Game.h>
#include <StatePlaying.h>

using namespace irr;
using namespace core;
using namespace video;

InteractionMenu::InteractionMenu() {}

void InteractionMenu::init ( int height, GameState* state ) {
	this->height = height;
	this->parentState = state;
	
	int top = Game::game.getRendMgr()->getDriver()->getScreenSize().Height - height;
	int width = Game::game.getRendMgr()->getDriver()->getScreenSize().Width;
	
	std::string filepath = RenderManager::resPath + "/materials/textures/SerifFont.xml";
	guiElements.push_back(new GuiElement(40, top + 20, 100, 20, "Tower", filepath, SColor(50,255,255,255), 0));
	guiElements.push_back(new GuiElement(40, top + 50, 100, 20, "Tree", filepath, SColor(50,255,255,255), 1));
	guiElements.push_back(new GuiElement(40, top + 80, 100, 20, "Rock", filepath, SColor(50,255,255,255), 2));
	guiElements.push_back(new GuiElement(160, top + 20, 100, 20, "EnemyUnit", filepath, SColor(50,255,255,255), 3));
	guiElements.push_back(new GuiElement(160, top + 50, 100, 20, "PlayerUnit", filepath, SColor(50,255,255,255), 4));
	
	for (GuiElement* e : guiElements) {
		e->registerObserver(this);
	}
	
	sndRolloverSound = Game::game.resources.loadSound("click.ogg");
	sndClickSound = Game::game.resources.loadSound("click2.ogg");
}

void InteractionMenu::onNotify ( int id, int eventID ) {
    GuiObserver::onNotify ( id, eventID );
	
	if (eventID == mouseOver) {
		sndRolloverSound->play();
	} else {
		switch (id) {
		case 0:
			((StatePlaying*)parentState)->message(SET_PLACE_OBJECT_TOWER);
			break;
		case 1:
			((StatePlaying*)parentState)->message(SET_PLACE_OBJECT_TREE);
			break;
		case 2:
			((StatePlaying*)parentState)->message(SET_PLACE_OBJECT_ROCK);
			break;
		case 3:
			((StatePlaying*)parentState)->message(SET_PLACE_OBJECT_ENEMY_UNIT);
			break;
		case 4:
			((StatePlaying*)parentState)->message(SET_PLACE_OBJECT_PLAYER_UNIT);
			break;
		}
		sndClickSound->play();
	}
}

void InteractionMenu::update() {
	for (GuiElement* e : guiElements) {
		e->update();
	}
}

void InteractionMenu::render ( irr::video::IVideoDriver* driver ) {
	int bottom = driver->getScreenSize().Height;
	int width = driver->getScreenSize().Width;
	
	int panelSize = 320;
	int panelPadding = 5;
	
	recti drawRectBack(0, bottom-height, width, bottom);
	recti drawRectLeft(panelPadding, bottom-height+panelPadding, panelSize, bottom-panelPadding);
	recti drawRectRight(width - panelSize, bottom-height+panelPadding, width-panelPadding, bottom-panelPadding);
	
	driver->draw2DRectangle(SColor(200,60,38,28), drawRectBack);
	driver->draw2DRectangle(SColor(255,80,58,48), drawRectLeft);
	driver->draw2DRectangle(SColor(255,80,58,48), drawRectRight);
	
	for (GuiElement* e : guiElements) {
		e->render(driver);
	}
}
