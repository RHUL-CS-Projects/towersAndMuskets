#ifndef STATEPLAYING_H
#define STATEPLAYING_H

#include <GameState.h>
#include <TowerDefenseEngine.h>
#include <string>
#include <sfml/SFML/Audio.hpp>

enum MESSAGE_TYPES {
	SET_PLACE_OBJECT_TOWER,
	SET_PLACE_OBJECT_TREE,
	SET_PLACE_OBJECT_ROCK,
	SET_PLACE_OBJECT_ENEMY_UNIT,
	SET_PLACE_OBJECT_PLAYER_UNIT,
	SET_PLACE_OBJECT_PLAYER_CANNON,
	SHOW_MESSAGE_GOOD,
	SHOW_MESSAGE_BAD
};

class StatePlaying : public GameState {
private:
	sf::SoundBuffer bufGunshot1;
	sf::Sound sndGunshot1;

	RTSCamera camera;
	InteractionMenu interactionMenu;
	MapGenerator mapGenerator;
	ObjectPlacer objectPlacer;
	NotificationMessage messageDisplay;
	
public:
	StatePlaying (std::string mapname);
	
	virtual void update();
	virtual void render(irr::video::IVideoDriver* driver);
	virtual void transitionIn();
	virtual void transitionOut();
	void loadMap(std::string mapname);
	void message(int messageNum, std::string message = "");
};

#endif