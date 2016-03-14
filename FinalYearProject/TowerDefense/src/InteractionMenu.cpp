#include <InteractionMenu.h>
#include <irrlicht/irrlicht.h>
#include <RenderManager.h>
#include <string>
#include <iostream>
#include <Game.h>
#include <StatePlaying.h>
#include <StatePauseMenu.h>

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
	guiElements.push_back(new GuiElement(20, top + 30, 100, 20, "Tower", filepath, SColor(150,255,255,255), 0));
	guiElements.push_back(new GuiElement(20, top + 60, 100, 20, "Tree", filepath, SColor(150,255,255,255), 1));
	guiElements.push_back(new GuiElement(20, top + 90, 100, 20, "Rock", filepath, SColor(150,255,255,255), 2));
	guiElements.push_back(new GuiElement(140, top + 30, 100, 20, "EnemyUnit", filepath, SColor(150,255,255,255), 3));
	guiElements.push_back(new GuiElement(140, top + 60, 100, 20, "Musketeer", filepath, SColor(150,255,255,255), 4));
	guiElements.push_back(new GuiElement(140, top + 90, 100, 20, "Cannon", filepath, SColor(150,255,255,255), 5));
	guiElements.push_back(new GuiElement(260, top + 30, 100, 20, "Villager", filepath, SColor(150,255,255,255), 10));
	
	//guiElements.push_back(new GuiElement(400, top + 40, 100, 20, "Map1", filepath, SColor(150,255,255,255), 7));
	//guiElements.push_back(new GuiElement(400, top + 70, 100, 20, "Map2", filepath, SColor(150,255,255,255), 8));
	//guiElements.push_back(new GuiElement(400, top + 100, 100, 20, "Map3", filepath, SColor(150,255,255,255), 9));
	
	guiElements.push_back(new GuiElement(width - 130, top + 50, 100, 20, "Menu", filepath, SColor(50,255,255,255), 6));
	
	font = Game::game.getRendMgr()->getGUIEnvironment()->getFont(filepath.c_str());
	
	for (GuiElement* e : guiElements) {
		e->registerObserver(this);
	}
	
	sndRolloverSound = Game::game.resources.loadSound("click.ogg");
	sndClickSound = Game::game.resources.loadSound("click2.ogg");
	
	texGold = Game::game.getRendMgr()->getDriver()->getTexture((RenderManager::resPath + "/materials/textures/GoldIcon.png").c_str());
	texStone = Game::game.getRendMgr()->getDriver()->getTexture((RenderManager::resPath + "/materials/textures/StoneIcon.png").c_str());
	texWood = Game::game.getRendMgr()->getDriver()->getTexture((RenderManager::resPath + "/materials/textures/WoodIcon.png").c_str());
	
	texHUD = Game::game.getRendMgr()->getDriver()->getTexture((RenderManager::resPath + "/materials/textures/hudimage.png").c_str());
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
		case 5:
			((StatePlaying*)parentState)->message(SET_PLACE_OBJECT_PLAYER_CANNON);
			break;
		case 6:
			Game::game.pushState(new StatePauseMenu());
			break;
		case 7:
			((StatePlaying*)Game::game.currentState())->reloadMap("map1");
			break;
		case 8:
			((StatePlaying*)Game::game.currentState())->reloadMap("map2");
			break;
		case 9:
			((StatePlaying*)Game::game.currentState())->reloadMap("map3");
			break;
		case 10:
			((StatePlaying*)parentState)->message(SET_PLACE_OBJECT_PLAYER_VILLAGER);
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
	
	driver->draw2DImage(texHUD, recti(0, bottom-height-30, width, bottom), recti(0, 0, texHUD->getSize().Width, texHUD->getSize().Height), 0, 0, true);
	
	for (GuiElement* e : guiElements) {
		e->render(driver);
	}
	
	driver->draw2DImage(texGold, recti(920, bottom-height+25, 940, bottom-height+45), recti(0, 0, texGold->getSize().Width, texGold->getSize().Height), 0, 0, true);
	driver->draw2DImage(texStone, recti(920, bottom-height+55, 940, bottom-height+75), recti(0, 0, texStone->getSize().Width, texStone->getSize().Height), 0, 0, true);
	driver->draw2DImage(texWood, recti(920, bottom-height+85, 940, bottom-height+105), recti(0, 0, texWood->getSize().Width, texWood->getSize().Height), 0, 0, true);
	
	std::string stone = std::to_string(((StatePlaying*)Game::game.currentState())->getResourceCache()->getStone());
	std::string gold = std::to_string(((StatePlaying*)Game::game.currentState())->getResourceCache()->getGold());
	std::string wood = std::to_string(((StatePlaying*)Game::game.currentState())->getResourceCache()->getWood());
	font->draw(gold.c_str(), recti(950, bottom-height+25, 1040, bottom-height+45),SColor(200,255,255,255),false, true);
	font->draw(stone.c_str(), recti(950, bottom-height+55, 1040, bottom-height+75),SColor(200,255,255,255),false, true);
	font->draw(wood.c_str(), recti(950, bottom-height+85, 1040, bottom-height+105),SColor(200,255,255,255),false, true);
}



