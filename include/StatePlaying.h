#ifndef STATEPLAYING_H
#define STATEPLAYING_H

#include <GameState.h>

class StatePlaying : public GameState {
private:
	
public:
	StatePlaying () : GameState() {
	
	}
	
	virtual void update();
	virtual void render(irr::video::IVideoDriver* driver);
	virtual void transitionIn();
	virtual void transitionOut();
};

#endif