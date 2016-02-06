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
	sf::SoundBuffer bufRolloverSound;
	sf::SoundBuffer bufClickSound;
	sf::Sound sndRolloverSound;
	sf::Sound sndClickSound;
	irr::video::ITexture* backImage;
	
public:
	StateMainMenu() : GameState() {
		std::string filepath = RenderManager::resPath + "/materials/textures/SerifFont.xml";
		guiElements.push_back(new GuiElement(40, 300, 200, 25, "Play", filepath, irr::video::SColor(50,255,255,255), btnPlay));
		guiElements.push_back(new GuiElement(40, 340, 200, 25, "Options", filepath, irr::video::SColor(50,255,255,255), btnOptions));
		guiElements.push_back(new GuiElement(40, 380, 200, 25, "Quit", filepath, irr::video::SColor(50,255,255,255), btnQuit));
			
		for (GuiElement* e : guiElements) {
			e->registerObserver(this);
		}
		
		filepath = RenderManager::resPath + "/materials/textures/menuImage.jpg";
		backImage = Game::game.getRendMgr()->getDriver()->getTexture(irr::io::path(filepath.c_str()));
		
		if (!bufRolloverSound.loadFromFile("res/sounds/click.ogg")) {
			std::cout << "Sound click.ogg not loaded" << std::endl;
		} else {
			sndRolloverSound.setBuffer(bufRolloverSound);
			sndRolloverSound.setVolume(50);
		}
		
		if (!bufClickSound.loadFromFile("res/sounds/click2.ogg")) {
			std::cout << "Sound click.ogg not loaded" << std::endl;
		} else {
			sndClickSound.setBuffer(bufClickSound);
			sndClickSound.setVolume(50);
		}
	}
	
	virtual void update();
	virtual void render(irr::video::IVideoDriver* driver);
	virtual void transitionIn();
	virtual void transitionOut();
	virtual void onNotify(int id, int eventID);
};

#endif