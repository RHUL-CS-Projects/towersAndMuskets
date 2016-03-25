#ifndef RTSMOVEMENTCOMPONENT_H
#define RTSMOVEMENTCOMPONENT_H

#include <GameComponent.h>
#include <sfml/SFML/Audio.hpp>
#include <random>
#include <stack>
#include <irrlicht/irrlicht.h>
#include <Enums.h>
#include <iostream>
#include <ParticleManager.h>

/**
 * Component to control the logic/AI of units by controlling their states
 */
class RTSLogicComponent : public GameComponent {
private:

public:
	RTSLogicComponent() : GameComponent("RTSLogicComponent") {}
	
	RTSLogicComponent(int actionFrame, sf::Sound* shoot, sf::Sound* walkSound, int shootDelay, bool canGarrisson, int attackDamage = 4, int rangeInSquares = 10,
					  EFFECT_TYPE effectType = ET_MUSKET) : GameComponent("RTSLogicComponent") {
		attackActionFrame = actionFrame;
		shootSound = shoot;
		this->canGarrisson = canGarrisson;
		
		this->shootDelay = shootDelay;
		shootCounter = rand() % shootDelay;
		this->attackDamage = attackDamage;
		this->rangeInSquares = rangeInSquares;
		this->walkSound = walkSound;
		this->effectType = effectType;
		
		nearestEnemyDelay = rand()%120;
	}
	
	int attackTargetID = -1;
	int towerID = -1;
	bool garrissoned = false;
	bool canGarrisson = true;
	
	int attackDamage = 4;
	int attackBonus = 1;
	int rangeInSquares = 10;
	
	irr::core::vector3df terrainPoint;
	bool pathSet = false;
	
	int attackActionFrame;
	sf::Sound* shootSound;
	sf::Sound* walkSound;
	int shootDelay;
	int shootCounter;
	
	int nearestEnemyDelay;
	
	EFFECT_TYPE effectType;
	
	std::stack<RTS_UNIT_STATE> stateStack;
};

#endif