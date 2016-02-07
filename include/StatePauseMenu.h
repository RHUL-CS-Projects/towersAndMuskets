#ifndef STATEPAUSEMENU_H
#define STATEPAUSEMENU_H

#include <GameState.h>
#include <GuiObserver.h>
#include <list>
#include <GuiElement.h>
#include <sfml/SFML/Audio.hpp>
#include <RenderManager.h>
#include <Game.h>

class StatePauseMenu : public GameState, public GuiObserver {
private:
	std::list<GuiElement*> guiElements;
	sf::Sound* sndRolloverSound;
	sf::Sound* sndClickSound;
	
public:
	StatePauseMenu () : GameState() {
		std::string filepath = RenderManager::resPath + "/materials/textures/SerifFont.xml";
		int midY = Game::game.getRendMgr()->getDriver()->getScreenSize().Height / 2;
		int midX = Game::game.getRendMgr()->getDriver()->getScreenSize().Width / 2;
		guiElements.push_back(new GuiElement(midX - 100, midY - 25, 200, 25, "Resume", filepath, irr::video::SColor(50,255,255,255), 0));
		guiElements.push_back(new GuiElement(midX - 100, midY + 20, 200, 25, "Quit", filepath, irr::video::SColor(50,255,255,255), 1));
			
		for (GuiElement* e : guiElements) {
			e->registerObserver(this);
		}

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