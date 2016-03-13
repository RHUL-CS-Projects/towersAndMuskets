#ifndef TEAMCOMPONENT_H
#define TEAMCOMPONENT_H

#include <GameComponent.h>

class TeamComponent : public GameComponent {
private:
	
public:
	TeamComponent (int teamID) : GameComponent("TeamComponent") {
		this->teamID = teamID;
	}
	
	int teamID;
};

#endif