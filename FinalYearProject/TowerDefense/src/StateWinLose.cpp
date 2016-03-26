#include <StateWinLose.h>
#include <irrlicht/irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;

void StateWinLose::render ( irr::video::IVideoDriver* driver ) {
    GameState::render ( driver );
	
	int bottom = Game::game.getRendMgr()->getDriver()->getScreenSize().Height;
	int right = Game::game.getRendMgr()->getDriver()->getScreenSize().Width;
	
	driver->draw2DRectangle(SColor(100, 0, 0, 0), recti(vector2di(0,0), vector2di(right,bottom)));
	
	recti imageRect(0, 0, winImage->getSize().Width, winImage->getSize().Height);
	if (win)
		driver->draw2DImage(winImage, vector2di(right/2-imageRect.getWidth()/2,bottom/2-imageRect.getHeight()/1.5), imageRect, 0, SColor(255,255,255,255), true);
	else
		driver->draw2DImage(loseImage, vector2di(right/2-imageRect.getWidth()/2,bottom/2-imageRect.getHeight()/1.5), imageRect, 0, SColor(255,255,255,255), true);
	
	for (GuiElement* e : guiElements) {
		e->render(driver);
	}
}

void StateWinLose::transitionIn() {
    GameState::transitionIn();
}

void StateWinLose::transitionOut() {
    GameState::transitionOut();
}

void StateWinLose::update() {
    GameState::update();
	
	for (GuiElement* e : guiElements) {
		e->update();
	}
}

void StateWinLose::onNotify ( int id, int eventID ) {
    GuiObserver::onNotify ( id, eventID );
	
	if (eventID == mouseOver) {
		sndRolloverSound->play();
	} else if (eventID == mousePressed) {
		switch (id) {
		case 0:
			Game::game.getRendMgr()->getSceneManager()->clear();
			Game::game.getObjMgr()->clearObjects();
			Game::game.popStates(2);
			break;
		}
		sndClickSound->play();
	}
}
