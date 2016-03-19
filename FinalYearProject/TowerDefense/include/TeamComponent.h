#ifndef TEAMCOMPONENT_H
#define TEAMCOMPONENT_H

#include <GameComponent.h>
#include <vector>

class TeamComponent : public GameComponent {
private:
	static std::vector<int> objectsTeam1;
	static std::vector<int> objectsTeam2;
	
public:
	TeamComponent (int objectID, int teamID) : GameComponent("TeamComponent") {
		this->teamID = teamID;
		
		if (teamID == 0)
			objectsTeam1.push_back(objectID);
		else
			objectsTeam2.push_back(objectID);
	}
	
	int teamID;
	
	static std::vector<int> getObjectsOnTeam(int team);
};

#endif