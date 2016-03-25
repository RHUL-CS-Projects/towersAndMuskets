#ifndef RTSAICOMPONENT_H
#define RTSAICOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>
#include <sfml/SFML/Audio.hpp>
#include <Enums.h>
#include <ParticleManager.h>

class RTSAIComponent : public GameComponent {
public:
	RTSAIComponent() : GameComponent("RTSAIComponent") {}
	
	RTSAIComponent(int actionFrame, sf::Sound* shoot, sf::Sound* walkSound, int shootDelay, int attackDamage = 4, int rangeInSquares = 10, 
				   EFFECT_TYPE effectType = ET_MUSKET) : GameComponent("RTSAIComponent") {
		attackActionFrame = actionFrame;
		shootSound = shoot;
		
		this->shootDelay = shootDelay;
		shootCounter = rand() % shootDelay;
		this->attackDamage = attackDamage;
		this->rangeInSquares = rangeInSquares;
		this->walkSound = walkSound;
		this->effectType = effectType;
	}
	
	int attackTargetID = -1;

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
	
	EFFECT_TYPE effectType;
	
	std::stack<RTS_UNIT_STATE> stateStack;
};

#endif