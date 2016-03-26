#ifndef STATEWINLOSE_H
#define STATEWINLOSE_H

#include <GameState.h>
#include <GuiObserver.h>
#include <list>
#include <GuiElement.h>
#include <sfml/SFML/Audio.hpp>
#include <RenderManager.h>
#include <Game.h>

class StateWinLose : public GameState, public GuiObserver {
private:
	std::list<GuiElement*> guiElements;
	sf::Sound* sndRolloverSound;
	sf::Sound* sndClickSound;
	irr::video::ITexture* winImage, *loseImage;
	
	bool win;
public:
	StateWinLose (bool win) : GameState("StateWinLose") {
		this->win = win;
		
		std::string filepath = RenderManager::resPath + "/materials/textures/SerifFont.xml";
		int midY = Game::game.getRendMgr()->getDriver()->getScreenSize().Height / 2;
		int midX = Game::game.getRendMgr()->getDriver()->getScreenSize().Width / 2;
		guiElements.push_back(new GuiElement(midX - 100, midY + 100, 200, 25, "Main Menu", filepath, irr::video::SColor(50,255,255,255), 0));
			
		for (GuiElement* e : guiElements) {
			e->registerObserver(this);
		}

		filepath = RenderManager::resPath + "/materials/textures/victory.png";
		winImage = Game::game.getRendMgr()->getDriver()->getTexture(irr::io::path(filepath.c_str()));
		
		filepath = RenderManager::resPath + "/materials/textures/defeat.png";
		loseImage = Game::game.getRendMgr()->getDriver()->getTexture(irr::io::path(filepath.c_str()));
		
		sndRolloverSound = Game::game.resources.loadSound("click.ogg");
		sndClickSound = Game::game.resources.loadSound("click2.ogg");
		
		transparentUpdate = false;
		transparentDraw = true;
	}

	virtual void update();
	virtual void render(irr::video::IVideoDriver* driver);
	virtual void transitionIn();
	virtual void transitionOut();
	virtual void onNotify(int id, int eventID);
};

#endif