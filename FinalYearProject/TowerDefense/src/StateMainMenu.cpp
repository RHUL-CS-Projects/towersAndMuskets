#include <StateMainMenu.h>
#include <irrlicht/irrlicht.h>
#include <string>
#include <StatePlaying.h>

using namespace std;
using namespace irr;
using namespace core;

void StateMainMenu::update() {
    GameState::update();
	
	for (GuiElement* e : guiElements) {
		e->update();
	}
}

void StateMainMenu::render ( irr::video::IVideoDriver* driver ) {
    GameState::render ( driver );
	
	driver->draw2DImage(backImage, 
						recti(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height), 
						recti(0, 0, backImage->getSize().Width, backImage->getSize().Height));
	
	for (GuiElement* e : guiElements) {
		e->render(driver);
	}
}

void StateMainMenu::transitionIn() {
    GameState::transitionIn();
}

void StateMainMenu::transitionOut() {
    GameState::transitionOut();
}

void StateMainMenu::onNotify ( int id, int eventID ) {
    if (eventID == mouseOver) {
		sndRolloverSound->play();
	} else {
		switch (id) {
		case btnPlay:
			//cout << "play clicked" << endl;
			sndClickSound->play();
			Game::game.pushState(new StatePlaying());
			break;
		case btnOptions:
			//cout << "options clicked" << endl;
			sndClickSound->play();
			break;
		case btnQuit:
			//cout << "quit clicked" << endl;
			Game::game.getRendMgr()->getDevice()->closeDevice();
			//sndClickSound.play();
			break;
		}
	}
}
