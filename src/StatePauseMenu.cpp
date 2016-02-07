#include <StatePauseMenu.h>
#include <irrlicht/irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;

void StatePauseMenu::render ( irr::video::IVideoDriver* driver ) {
    GameState::render ( driver );
	
	int bottom = Game::game.getRendMgr()->getDriver()->getScreenSize().Height;
	int right = Game::game.getRendMgr()->getDriver()->getScreenSize().Width;
	
	driver->draw2DRectangle(SColor(100, 0, 0, 0), recti(vector2di(0,0), vector2di(right,bottom)));
	
	for (GuiElement* e : guiElements) {
		e->render(driver);
	}
}

void StatePauseMenu::transitionIn() {
    GameState::transitionIn();
}

void StatePauseMenu::transitionOut() {
    GameState::transitionOut();
}

void StatePauseMenu::update() {
    GameState::update();
	
	for (GuiElement* e : guiElements) {
		e->update();
	}
}

void StatePauseMenu::onNotify ( int id, int eventID ) {
    GuiObserver::onNotify ( id, eventID );
	
	if (eventID == mouseOver) {
		sndRolloverSound->play();
	} else if (eventID == mousePressed) {
		switch (id) {
		case 0:
			Game::game.popState();
			break;
		case 1:
			Game::game.getRendMgr()->getSceneManager()->clear();
			Game::game.getObjMgr()->clearObjects();
			Game::game.popStates(2);
			break;
		}
		sndClickSound->play();
	}
}
