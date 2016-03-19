#include <TeamComponent.h>

std::vector<int> TeamComponent::objectsTeam1;
std::vector<int> TeamComponent::objectsTeam2;

std::vector< int > TeamComponent::getObjectsOnTeam ( int team ) {
	return team == 0 ? objectsTeam1 : objectsTeam2;
}
