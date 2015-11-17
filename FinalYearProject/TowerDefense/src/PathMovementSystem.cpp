#include <PathMovementSystem.h>
#include <PathMovementComponent.h>
#include <TransformComponent.h>
#include <SelectableComponent.h>
#include <ObjectManager.h>
#include <list>
#include <irrlicht/irrlicht.h>
#include <RenderManager.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

/**
 * Update the PathMovementComponents to move game objects along their paths
 */
void PathMovementSystem::update ( float timestep ) {
	// Get the object manager
	ObjectManager* mgr = &ObjectManager::manager;
	
	// Get all objects with a PathMovementComponent
	std::list<int> objects = mgr->getObjectsWithComponent("PathMovementComponent");
	
	for (int i : objects) {
		PathMovementComponent* pathComp = mgr->getObjectComponent<PathMovementComponent>(i, "PathMovementComponent");
		
		// Check the object has a TransformComponent
		TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
		if (transComp == nullptr)
			continue;
	
		// Check there are waypoints
		if (pathComp->waypoints.size() <= 0)
			continue;
		
		// Get next waypoint and current object position
		vector3df nextPoint = pathComp->waypoints.front();
		vector3df* currentPos = &transComp->worldPosition;
		
		// Compute distance to next node in path
		if (pathComp->distToWaypoint < 0) {
			vector3df dif = nextPoint - *currentPos;
			float distance = dif.getLength();
			pathComp->distToWaypoint = distance;
			
			pathComp->totalSteps = distance / pathComp->moveSpeed;
			pathComp->stepSize = 1.0f / pathComp->totalSteps;
			pathComp->waypointPosition = 0;
			pathComp->prevPos = transComp->worldPosition;
		}
		
		// Step towards next waypoint
		pathComp->waypointPosition += pathComp->stepSize;
		pathComp->waypointPosition = (pathComp->waypointPosition >= 1) ? 1 : pathComp->waypointPosition;
		currentPos->interpolate(nextPoint, pathComp->prevPos, pathComp->waypointPosition);
		
		// Finally check if the waypoint was reached
		if (pathComp->waypointPosition >= 1) {
			pathComp->waypoints.pop();
			pathComp->distToWaypoint = -1;
		}
	}
}

/**
 * Draw debug information to the screen about each path being followed
 */
void PathMovementSystem::draw ( float timestep ) {
    if (RenderManager::DEBUG_GRAPHICS) {
		// Get the object manager
		ObjectManager* mgr = &ObjectManager::manager;
		
		// Get all objects with a PathMovementComponent
		std::list<int> objects = mgr->getObjectsWithComponent("PathMovementComponent");
		
		for (int i : objects) {
			PathMovementComponent* pathComp = mgr->getObjectComponent<PathMovementComponent>(i, "PathMovementComponent");
			
			// Check the object has a TransformComponent
			TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
			if (transComp == nullptr)
				continue;
		
			// Check if the object has a SelectableComponent (to only draw the path if it is selected)
			SelectableComponent* selectComp = mgr->getObjectComponent<SelectableComponent>(i, "SelectableComponent");
			if (selectComp == nullptr || !selectComp->selected)
				continue;
			
			// Check there are waypoints
			if (pathComp->waypoints.size() <= 0)
				continue;
			
			IVideoDriver* driver =  RenderManager::renderManager.getDriver();	
			
			std::queue<vector3df> copyQueue = pathComp->waypoints;
			
			SMaterial m;
			m.Lighting = false;
			m.Thickness = 1.0f;
			RenderManager::renderManager.getDriver()->setMaterial(m);
			RenderManager::renderManager.getDriver()->setTransform(video::ETS_WORLD, IdentityMatrix);
			
			vector3df prevNode = transComp->worldPosition;
			vector3df node;
			while (copyQueue.size() > 0) {
				node = copyQueue.front();
				driver->draw3DLine(prevNode+vector3df(0,1,0), node+vector3df(0,1,0), SColor(255,255,0,0));			
				
				copyQueue.pop();
				prevNode = node;
			}
		}
	}
}







