#include <GroupPathMover.h>
#include <thread>
#include <Game.h>
#include <PathFinder.h>
#include <TransformComponent.h>
#include <SteeringComponent.h>
#include <RTSLogicComponent.h>

using namespace irr;
using namespace core;
using namespace std;

void GroupPathMover::startCalc() {
	std::thread t(&GroupPathMover::calculatePaths, objects, destination);
	t.detach();
}

void GroupPathMover::calculatePaths( std::vector<int> objs, irr::core::vector3df dest ) {
	int numObjects = objs.size();
	int boxWidth = (int)ceil(sqrt(numObjects));
	int boxHeight = (int)ceil((double)numObjects/boxWidth);
	
	ObjectManager* mgr = Game::game.getObjMgr();
	WorldManager* wmgr = mgr->worldManager;
	
	double gridSize = wmgr->gridSize;
	double spacing = gridSize * 1;
	
	double spacedWidth = boxWidth * spacing;
	double spacedHeight = boxHeight * spacing;
	vector3df mid = dest;
	
	int xcurrent = 0;
	int ycurrent = 0;
	
	vector<vector3df> points;
	int unassigned = numObjects;
	
	double px, py;
	
	vector3df averagePoint(0,0,0);
	for (int i : objs) {
		TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
		
		if (transComp == nullptr)
			return;
		
		averagePoint += transComp->worldPosition;
	}
	averagePoint /= objs.size();
	
	vector3df forward = (dest - averagePoint).normalize();
	vector3df left(-forward.Z, 0, forward.X);
	
	// Find a position for all units in group
	while (unassigned > 0) {
		
		vector3df tempPoint(mid.X, 0, mid.Z);
		tempPoint += (spacedWidth/2)*left;
		tempPoint += (spacedHeight/2)*forward;
		tempPoint -= (xcurrent*spacing+spacing/2)*left;
		tempPoint -= (ycurrent*spacing+spacing/2)*forward;
		
		if (wmgr->checkPassable(tempPoint)) {
			unassigned--;
			points.push_back(tempPoint);
		}
		
		xcurrent++;
		if (xcurrent >= boxWidth) {
			xcurrent = 0;
			ycurrent++;
		}
	}
	
	for (int id : objs) {
		// Get only selected selectable objects
		TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(id, "TransformComponent");
		
		if (transComp == nullptr)
			return;
		
		// Get object's steering system
		SteeringComponent* steerComp = mgr->getObjectComponent<SteeringComponent>(id, "SteeringComponent");
		
		if (steerComp == nullptr)
			return;
		
		steerComp->path.resetPath();
		
		PathFinder pathFinder(mgr->worldManager);
		steerComp->path = pathFinder.findPath(transComp->worldPosition, points.back());
		points.pop_back();
		
		RTSLogicComponent* rtsComp = mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent");
		if (rtsComp != nullptr) {
			rtsComp->stateStack.pop();
			rtsComp->pathSet = true;
		}
	}
}

void GroupPathMover::requestPath ( int id, irr::core::vector3df pos ) {
	destination = pos;
	objects.push_back(id);
}

