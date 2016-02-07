#ifndef STATEPLAYING_H
#define STATEPLAYING_H

#include <GameState.h>
#include <TowerDefenseEngine.h>
#include <string>

enum MESSAGE_TYPES {
	SET_PLACE_OBJECT_TOWER,
	SET_PLACE_OBJECT_TREE,
	SET_PLACE_OBJECT_ROCK,
	SET_PLACE_OBJECT_ENEMY_UNIT,
	SET_PLACE_OBJECT_PLAYER_UNIT
};

class StatePlaying : public GameState {
private:
	
public:
	StatePlaying ();

	virtual void update();
	virtual void render(irr::video::IVideoDriver* driver);
	virtual void transitionIn();
	virtual void transitionOut();
	void message(int messageNum);
};

#endif