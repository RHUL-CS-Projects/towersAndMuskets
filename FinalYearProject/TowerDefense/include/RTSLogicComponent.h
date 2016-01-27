#ifndef RTSMOVEMENTCOMPONENT_H
#define RTSMOVEMENTCOMPONENT_H

#include <GameComponent.h>
#include <sfml/SFML/Audio.hpp>
#include <random>

enum RTS_UNIT_STATE { IDLE, WALKING, MOVE_TO_ATTACK, ATTACKING, RELOADING, DEAD }; 

/**
 * Component to control the logic/AI of units by controlling their states
 */
class RTSLogicComponent : public GameComponent {
private:

public:
	RTSLogicComponent() : GameComponent("RTSLogicComponent") {}
	
	RTSLogicComponent(int team, int actionFrame, sf::Sound shoot, int shootDelay) : GameComponent("RTSLogicComponent") {
		teamID = team;
		attackActionFrame = actionFrame;
		shootSound = shoot;
		
		this->shootDelay = shootDelay;
		shootCounter = rand() % shootDelay;
	}
	
	RTS_UNIT_STATE currentState = IDLE;
	
	int attackTargetID = -1;
	int teamID = -1;
	int attackActionFrame;
	sf::Sound shootSound;
	int shootDelay;
	int shootCounter;
};

#endif