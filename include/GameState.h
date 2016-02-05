#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <irrlicht/irrlicht.h>

class GameState {
public:
	GameState() {}
	
	virtual void update() {}
	virtual void render(irr::video::IVideoDriver* driver) {}
	virtual void transitionIn() {}
	virtual void transitionOut() {}
	
	bool transparentDraw;
	bool transparentUpdate;
	bool deleted;
};

#endif