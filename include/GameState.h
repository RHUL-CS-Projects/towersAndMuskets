#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <irrlicht/irrlicht.h>
#include <string>

class GameState {
public:
	GameState(std::string name) {
		this->name = name;
	}
	
	virtual void update() {}
	virtual void render(irr::video::IVideoDriver* driver) {}
	virtual void transitionIn() {}
	virtual void transitionOut() {}
	
	bool transparentDraw;
	bool transparentUpdate;
	bool deleted;
	std::string name;
};

#endif