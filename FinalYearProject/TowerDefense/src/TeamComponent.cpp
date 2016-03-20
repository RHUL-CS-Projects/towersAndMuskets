#include <TeamComponent.h>
#include <algorithm>
#include <Game.h>

std::vector<int> TeamComponent::objectsTeam1;
std::vector<int> TeamComponent::objectsTeam2;

std::vector< int > TeamComponent::getObjectsOnTeam ( int team ) {
	return team == 0 ? objectsTeam1 : objectsTeam2;
}

void TeamComponent::updateTeamLists () {
	for (auto it = objectsTeam1.begin(); it < objectsTeam1.end();) {
		int i = (*it);
		
		if (Game::game.getObjMgr()->objectExists(i) && Game::game.getObjMgr()->objectHasComponent(i, "TeamComponent")) {
			it++;
		} else {
			objectsTeam1.erase(it);
		}
	}
	
	for (auto it = objectsTeam2.begin(); it < objectsTeam2.end();) {
		int i = (*it);
		
		if (Game::game.getObjMgr()->objectExists(i) && Game::game.getObjMgr()->objectHasComponent(i, "TeamComponent")) {
			it++;
		} else {
			objectsTeam2.erase(it);
		}
	}
}

void TeamComponent::reset() {
	objectsTeam1.clear();
	objectsTeam2.clear();
}
