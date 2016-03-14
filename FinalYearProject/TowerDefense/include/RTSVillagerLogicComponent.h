#ifndef RTSVILLAGERMOVEMENTCOMPONENT_H
#define RTSVILLAGERMOVEMENTCOMPONENT_H

#include <GameComponent.h>
#include <sfml/SFML/Audio.hpp>
#include <random>
#include <stack>
#include <irrlicht/irrlicht.h> 
#include <Enums.h>

/**
 * Component to control the logic/AI of units by controlling their states
 */
class RTSVillagerLogicComponent : public GameComponent {
private:

public:
	RTSVillagerLogicComponent() : GameComponent("RTSVillagerLogicComponent") {}
	
	RTSVillagerLogicComponent(int chopFrame, int mineFrame) : GameComponent("RTSVillagerLogicComponent") {
		this->chopFrame = chopFrame;
		this->mineFrame = mineFrame;
	}
	
	int resourceTargetID = -1;
	int chopFrame;
	int mineFrame;
	bool soundPlayed = false;
	
	int gatherDelay = 60*4;
	int gatherTicker = gatherDelay;
	
	irr::core::vector3df terrainPoint;
	bool pathSet = false;
	bool resourceSet = false;
	
	std::stack<RTS_VILLAGER_STATE> stateStack;
	RESOURCE_TYPE resType;
};

#endif