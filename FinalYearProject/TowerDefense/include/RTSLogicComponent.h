#ifndef RTSMOVEMENTCOMPONENT_H
#define RTSMOVEMENTCOMPONENT_H

#include <GameComponent.h>

enum RTS_UNIT_STATE { IDLE, WALKING, MOVE_TO_ATTACK, ATTACKING, RELOADING, DEAD }; 

/**
 * Component to control the logic/AI of units by controlling their states
 */
class RTSLogicComponent : public GameComponent {
private:

public:
	RTSLogicComponent() : GameComponent("RTSLogicComponent") {}
	
	RTSLogicComponent(int team) : GameComponent("RTSLogicComponent") {
		teamID = team;
	}
	
	RTS_UNIT_STATE currentState = IDLE;
	
	int attackTargetID = -1;
	int teamID = -1;
};

#endif