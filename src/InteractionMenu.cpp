#include <InteractionMenu.h>
#include <irrlicht/irrlicht.h>
#include <RenderManager.h>
#include <string>
#include <iostream>
#include <Game.h>
#include <StatePlaying.h>
#include <StatePauseMenu.h>
#include <GuiElementPurchase.h>

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
	//guiElements.push_back(new GuiElement(20, top - 200 + 30, 100, 20, "Tower", filepath, SColor(150,255,255,255), 0));
	//guiElements.push_back(new GuiElement(20, top - 200 + 60, 100, 20, "Tree", filepath, SColor(150,255,255,255), 1));
	//guiElements.push_back(new GuiElement(20, top - 200 + 90, 100, 20, "Rock", filepath, SColor(150,255,255,255), 2));
	//guiElements.push_back(new GuiElement(140, top - 200 + 30, 100, 20, "EnemyUnit", filepath, SColor(150,255,255,255), 3));
	//guiElements.push_back(new GuiElement(140, top - 200 + 60, 100, 20, "Musketeer", filepath, SColor(150,255,255,255), 4));
	//guiElements.push_back(new GuiElement(140, top - 200 + 90, 100, 20, "Cannon", filepath, SColor(150,255,255,255), 5));
	//guiElements.push_back(new GuiElement(260, top - 200 + 30, 100, 20, "Villager", filepath, SColor(150,255,255,255), 10));
	
	guiElements.push_back(new GuiElement(width - 130, top + 50, 100, 20, "Menu", filepath, SColor(50,255,255,255), 6));
	guiElements.push_back(new GuiElement(width - 520, top + height - 30, 100, 20, "Skip Wait", filepath, SColor(50,255,255,255), 11));
	
	PurchaseDetails towerPurchase;
	towerPurchase.gold = 50;
	towerPurchase.stone = 100;
	towerPurchase.wood = 200;
	towerPurchase.name = "Watchtower";
	purchases.insert(std::pair<int, PurchaseDetails>(0, towerPurchase));
	guiElements.push_back(new GuiElementPurchase(20 + 10, top + 20, "unitbuttonback.png", "towerbutton.png", "unitbuttonbacklight.png", towerPurchase, 0));
	
	PurchaseDetails musketPurchase;
	musketPurchase.gold = 100;
	musketPurchase.stone = 0;
	musketPurchase.wood = 50;
	musketPurchase.name = "Musketeer";
	purchases.insert(std::pair<int, PurchaseDetails>(4, musketPurchase));
	guiElements.push_back(new GuiElementPurchase(20 + 90, top + 20, "unitbuttonback.png", "musketbutton.png", "unitbuttonbacklight.png", musketPurchase, 4));
	
	PurchaseDetails cannonPurchase;
	cannonPurchase.gold = 100;
	cannonPurchase.stone = 20;
	cannonPurchase.wood = 100;
	cannonPurchase.name = "Cannon";
	purchases.insert(std::pair<int, PurchaseDetails>(5, cannonPurchase));
	guiElements.push_back(new GuiElementPurchase(20 + 170, top + 20, "unitbuttonback.png", "cannonbutton.png", "unitbuttonbacklight.png", cannonPurchase, 5));
	
	PurchaseDetails gatherPurchase;
	gatherPurchase.gold = 20;
	gatherPurchase.stone = 0;
	gatherPurchase.wood = 0;
	gatherPurchase.name = "Gatherer";
	purchases.insert(std::pair<int, PurchaseDetails>(10, gatherPurchase));
	guiElements.push_back(new GuiElementPurchase(20 + 250, top + 20, "unitbuttonback.png", "villagerbutton.png", "unitbuttonbacklight.png", gatherPurchase, 10));
	currentPurchase = gatherPurchase;
	
	//guiElements.push_back(new GuiElement(400, top + 40, 100, 20, "Map1", filepath, SColor(150,255,255,255), 7));
	//guiElements.push_back(new GuiElement(400, top + 70, 100, 20, "Map2", filepath, SColor(150,255,255,255), 8));
	//guiElements.push_back(new GuiElement(400, top + 100, 100, 20, "Map3", filepath, SColor(150,255,255,255), 9));
	
	font = Game::game.getRendMgr()->getGUIEnvironment()->getFont(filepath.c_str());
	
	for (GuiElement* e : guiElements) {
		e->registerObserver(this);
	}
	
	sndRolloverSound = Game::game.resources.loadSound("click.ogg");
	sndClickSound = Game::game.resources.loadSound("click2.ogg");
	
	texRes = Game::game.getRendMgr()->getDriver()->getTexture((RenderManager::resPath + "/materials/textures/hud resources.png").c_str());
	texPurchase = Game::game.getRendMgr()->getDriver()->getTexture((RenderManager::resPath + "/materials/textures/hud popup.png").c_str());
	
	texHUD = Game::game.getRendMgr()->getDriver()->getTexture((RenderManager::resPath + "/materials/textures/hudimage.png").c_str());
	texBarBack = Game::game.getRendMgr()->getDriver()->getTexture((RenderManager::resPath + "/materials/textures/progressbarback.png").c_str());
	texBarFront = Game::game.getRendMgr()->getDriver()->getTexture((RenderManager::resPath + "/materials/textures/progressbarfront.png").c_str());
}

void InteractionMenu::setProgress ( double percent ) {
	progressPercent = percent;
	if (progressPercent < 0) progressPercent = 0;
	if (progressPercent > 1) progressPercent = 1;
}

void InteractionMenu::onNotify ( int id, int eventID ) {
    GuiObserver::onNotify ( id, eventID );
	
	if (eventID == mouseOver) {
		sndRolloverSound->play();
		
		if (purchases.find(id) != purchases.end()) {
			showPurchaseTooltip = true;
			currentPurchase = purchases.at(id);
		}
	} else if (eventID == mouseLeft) {
		showPurchaseTooltip = false;
	} else {
		switch (id) {
		case 0:
			if (((StatePlaying*)parentState)->getResourceCache()->canAfford(currentPurchase.stone, currentPurchase.gold, currentPurchase.wood)) {
				((StatePlaying*)parentState)->message(SET_PLACE_OBJECT_TOWER);
				((StatePlaying*)parentState)->getResourceCache()->spend(currentPurchase.stone, currentPurchase.gold, currentPurchase.wood);
			} else {
				((StatePlaying*)parentState)->message(SHOW_MESSAGE_BAD, "Cannot afford this purchase!");
			}
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
			if (((StatePlaying*)parentState)->getResourceCache()->canAfford(currentPurchase.stone, currentPurchase.gold, currentPurchase.wood)) {
				((StatePlaying*)parentState)->message(SET_PLACE_OBJECT_PLAYER_UNIT);
				((StatePlaying*)parentState)->getResourceCache()->spend(currentPurchase.stone, currentPurchase.gold, currentPurchase.wood);
			} else {
				((StatePlaying*)parentState)->message(SHOW_MESSAGE_BAD, "Cannot afford this purchase!");
			}
			break;
		case 5:
			if (((StatePlaying*)parentState)->getResourceCache()->canAfford(currentPurchase.stone, currentPurchase.gold, currentPurchase.wood)) {
				((StatePlaying*)parentState)->message(SET_PLACE_OBJECT_PLAYER_CANNON);
				((StatePlaying*)parentState)->getResourceCache()->spend(currentPurchase.stone, currentPurchase.gold, currentPurchase.wood);
			} else {
				((StatePlaying*)parentState)->message(SHOW_MESSAGE_BAD, "Cannot afford this purchase!");
			}
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
			if (((StatePlaying*)parentState)->getResourceCache()->canAfford(currentPurchase.stone, currentPurchase.gold, currentPurchase.wood)) {
				((StatePlaying*)parentState)->message(SET_PLACE_OBJECT_PLAYER_VILLAGER);
				((StatePlaying*)parentState)->getResourceCache()->spend(currentPurchase.stone, currentPurchase.gold, currentPurchase.wood);
			} else {
				((StatePlaying*)parentState)->message(SHOW_MESSAGE_BAD, "Cannot afford this purchase!");
			}
			break;
		case 11:
			((StatePlaying*)parentState)->message(SKIP_WAVE_WAIT);
			break;
		}
		sndClickSound->play();
	}
}

void InteractionMenu::update() {
	for (GuiElement* e : guiElements) {
		e->update();
	}
	
	if (showPurchaseTooltip)
		alpha = alpha + (1.0 - alpha) * 0.2;
	else
		alpha = alpha - alpha * 0.2;
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
	
	vector2di hudPos(0, bottom-height-30);
	driver->draw2DImage(texHUD, recti(hudPos.X, hudPos.Y, width, bottom), recti(0, 0, texHUD->getSize().Width, texHUD->getSize().Height), 0, 0, true);
	
	vector2di barPos(hudPos.X+394, hudPos.Y+55);
	recti barTexRect(barPos.X, barPos.Y, barPos.X+texBarFront->getSize().Width, barPos.Y+texBarFront->getSize().Height);
	driver->draw2DImage(texBarBack, barTexRect, recti(0, 0, texBarBack->getSize().Width, texBarBack->getSize().Height), 0, 0, true);
	
	recti barRect(barPos.X+20, barPos.Y+14, (int)(barPos.X+(barTexRect.getWidth()-20)*progressPercent), barPos.Y+barTexRect.getHeight()-14);

	driver->draw2DRectangle(SColor(150,0,255,0), barRect);
	driver->draw2DImage(texBarFront, barTexRect, recti(0, 0, texBarFront->getSize().Width, texBarFront->getSize().Height), 0, 0, true);
	
	recti captionShadowRect(barPos.X + 3, barPos.Y + barTexRect.getHeight() + 3, barTexRect.LowerRightCorner.X + 3, barTexRect.LowerRightCorner.Y + barTexRect.getHeight() + 13);
	font->draw("Time to Next Wave", captionShadowRect, SColor(200,0,0,0), true);
	
	recti captionRect(barPos.X, barPos.Y + barTexRect.getHeight(), barTexRect.LowerRightCorner.X, barTexRect.LowerRightCorner.Y + barTexRect.getHeight() + 10);
	font->draw("Time to Next Wave", captionRect, SColor(200,255,255,255), true);
	
	for (GuiElement* e : guiElements) {
		e->render(driver);
	}
	
	driver->draw2DImage(texRes, recti(920, bottom-height+10, 920+texRes->getSize().Width, bottom-height+10+texRes->getSize().Height), 
						recti(0, 0, texRes->getSize().Width, texRes->getSize().Height), 0, 0, true);

	
	std::string stone = std::to_string(((StatePlaying*)Game::game.currentState())->getResourceCache()->getStone());
	std::string gold = std::to_string(((StatePlaying*)Game::game.currentState())->getResourceCache()->getGold());
	std::string wood = std::to_string(((StatePlaying*)Game::game.currentState())->getResourceCache()->getWood());
	font->draw(gold.c_str(), recti(980, bottom-height+20, 1040, bottom-height+40),SColor(200,255,255,255),false, true);
	font->draw(stone.c_str(), recti(980, bottom-height+55, 1040, bottom-height+75),SColor(200,255,255,255),false, true);
	font->draw(wood.c_str(), recti(980, bottom-height+90, 1040, bottom-height+110),SColor(200,255,255,255),false, true);
	
	PlayerResourceCache* resCache = ((StatePlaying*)parentState)->getResourceCache();
	
	vector2di topLeft(15, bottom-height-20-texPurchase->getSize().Height);
	driver->draw2DImage(
		texPurchase, topLeft, 
		recti(0,0,texPurchase->getSize().Width, texPurchase->getSize().Height),
		0, 
		SColor(255*alpha,255,255,255),
		true
	);
	
	SColor textCol = resCache->canAfford(currentPurchase.stone, currentPurchase.gold, currentPurchase.wood)? SColor(255*alpha,255,255,255) : SColor(100*alpha,255,255,255);
	font->draw(currentPurchase.name.c_str(), recti(topLeft + vector2di(10,5), dimension2di(100,20)), textCol);
	
	SColor goldCol = resCache->getGold() >= currentPurchase.gold ? SColor(255*alpha,214,210,184) : SColor(200*alpha,255,0,0);
	font->draw(std::to_string(currentPurchase.gold).c_str(), recti(topLeft + vector2di(50,95), dimension2di(50,20)), goldCol);
	
	SColor stoneCol = resCache->getStone() >= currentPurchase.stone ? SColor(255*alpha,214,214,214) : SColor(200*alpha,255,0,0);
	font->draw(std::to_string(currentPurchase.stone).c_str(), recti(topLeft + vector2di(130,95), dimension2di(50,20)), stoneCol);
	
	SColor woodCol = resCache->getWood() >= currentPurchase.wood ? SColor(255*alpha,214,199,182) : SColor(200*alpha,255,0,0);
	font->draw(std::to_string(currentPurchase.wood).c_str(), recti(topLeft + vector2di(215,95), dimension2di(50,20)), woodCol);
}



