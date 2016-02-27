#ifndef RTSMOVEMENTCOMPONENT_H
#define RTSMOVEMENTCOMPONENT_H

#include <GameComponent.h>
#include <sfml/SFML/Audio.hpp>
#include <random>
#include <stack>
#include <irrlicht/irrlicht.h>

enum RTS_UNIT_STATE { IDLE=0, WALKING, MOVE_TO_ATTACK, ATTACKING, AIMING, TAKE_AIM, RELEASE_AIM, RELOADING, DEAD, MOVE_TO_TOWER, CLIMB_UP, CLIMB_DOWN, GARRISSONED }; 

/**
 * Component to control the logic/AI of units by controlling their states
 */
class RTSLogicComponent : public GameComponent {
private:

public:
	RTSLogicComponent() : GameComponent("RTSLogicComponent") {}
	
	RTSLogicComponent(int team, int actionFrame, sf::Sound* shoot, int shootDelay, bool canGarrisson) : GameComponent("RTSLogicComponent") {
		teamID = team;
		attackActionFrame = actionFrame;
		shootSound = shoot;
		this->canGarrisson = canGarrisson;
		
		this->shootDelay = shootDelay;
		shootCounter = rand() % shootDelay;
	}
	
	int attackTargetID = -1;
	int towerID = -1;
	int teamID = -1;
	bool garrissoned = false;
	bool canGarrisson = true;
	
	irr::core::vector3df terrainPoint;
	bool pathSet = false;
	
	int attackActionFrame;
	sf::Sound* shootSound;
	int shootDelay;
	int shootCounter;
	
	std::stack<RTS_UNIT_STATE> stateStack;
};

#endif