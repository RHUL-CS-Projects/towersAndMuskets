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
	double spacing = gridSize * 0.75;
	
	double spacedWidth = boxWidth * spacing;
	double spacedHeight = boxHeight * spacing;
	vector3df mid = dest;
	
	int xcurrent = 0;
	int ycurrent = 0;
	
	vector<vector3df> points;
	int unassigned = numObjects;
	
	double px, py;
	
	while (unassigned > 0) {
		// Check current point
		px = mid.X - spacedWidth/2 + (xcurrent * spacing);
		py = mid.Z - spacedHeight/2 + (ycurrent * spacing);
		
		vector3df tempPoint(px, 0, py);
		
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
		
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->stateStack.pop();
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->pathSet = true;
	}
}

void GroupPathMover::requestPath ( int id, irr::core::vector3df pos ) {
	destination = pos;
	objects.push_back(id);
}

