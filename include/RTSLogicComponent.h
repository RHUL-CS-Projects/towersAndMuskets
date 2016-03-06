#ifndef RTSMOVEMENTCOMPONENT_H
#define RTSMOVEMENTCOMPONENT_H

#include <GameComponent.h>
#include <sfml/SFML/Audio.hpp>
#include <random>
#include <stack>
#include <irrlicht/irrlicht.h>
#include <Enums.h>

/**
 * Component to control the logic/AI of units by controlling their states
 */
class RTSLogicComponent : public GameComponent {
private:

public:
	RTSLogicComponent() : GameComponent("RTSLogicComponent") {}
	
	RTSLogicComponent(int team, int actionFrame, sf::Sound* shoot, int shootDelay, bool canGarrissonm, int attackDamage = 4, int rangeInSquares = 10) : GameComponent("RTSLogicComponent") {
		teamID = team;
		attackActionFrame = actionFrame;
		shootSound = shoot;
		this->canGarrisson = canGarrisson;
		
		this->shootDelay = shootDelay;
		shootCounter = rand() % shootDelay;
		this->attackDamage = attackDamage;
		this->rangeInSquares = rangeInSquares;
	}
	
	int attackTargetID = -1;
	int towerID = -1;
	int teamID = -1;
	bool garrissoned = false;
	bool canGarrisson = true;
	
	int attackDamage = 4;
	int attackBonus = 1;
	int rangeInSquares = 10;
	
	irr::core::vector3df terrainPoint;
	bool pathSet = false;
	
	int attackActionFrame;
	sf::Sound* shootSound;
	int shootDelay;
	int shootCounter;
	
	std::stack<RTS_UNIT_STATE> stateStack;
};

#endif