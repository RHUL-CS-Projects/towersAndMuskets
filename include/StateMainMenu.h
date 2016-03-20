#ifndef STATEMAINMENU_H
#define STATEMAINMENU_H

#include <GameState.h>
#include <GuiElement.h>
#include <list>
#include <RenderManager.h>
#include <GuiObserver.h>
#include <sfml/SFML/Audio.hpp>
#include <iostream>
#include <Game.h>

enum MENU_BUTTONS {
	btnPlay,
	btnOptions,
	btnQuit
};

class StateMainMenu : public GameState, public GuiObserver {
private:
	std::list<GuiElement*> guiElements;
	sf::Sound* sndRolloverSound;
	sf::Sound* sndClickSound;
	irr::video::ITexture* backImage;
	
public:
	StateMainMenu() : GameState() {
		std::string filepath = RenderManager::resPath + "/materials/textures/SerifFont.xml";
		guiElements.push_back(new GuiElement(500, 680, 200, 25, "Play", filepath, irr::video::SColor(50,255,255,255), btnPlay));
		guiElements.push_back(new GuiElement(720, 680, 200, 25, "Options", filepath, irr::video::SColor(50,255,255,255), btnOptions));
		guiElements.push_back(new GuiElement(940, 680, 200, 25, "Quit", filepath, irr::video::SColor(50,255,255,255), btnQuit));
			
		for (GuiElement* e : guiElements) {
			e->registerObserver(this);
		}
		
		filepath = RenderManager::resPath + "/materials/textures/menuImage.jpg";
		backImage = Game::game.getRendMgr()->getDriver()->getTexture(irr::io::path(filepath.c_str()));

		sndRolloverSound = Game::game.resources.loadSound("click.ogg");
		sndClickSound = Game::game.resources.loadSound("click2.ogg");
		
		transparentDraw = false;
		transparentUpdate = false;
	}
	
	~StateMainMenu() {
		while (guiElements.size() > 0) {
			delete guiElements.front();
			guiElements.pop_front();
		}
	}
	
	virtual void update();
	virtual void render(irr::video::IVideoDriver* driver);
	virtual void transitionIn();
	virtual void transitionOut();
	virtual void onNotify(int id, int eventID);
};

#endif