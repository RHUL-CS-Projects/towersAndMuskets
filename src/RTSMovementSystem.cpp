#include <RTSMovementComponent.h>
#include <RTSMovementSystem.h>
#include <EventReceiver.h>
#include <ObjectManager.h>
#include <list>
#include <RenderManager.h>
#include <SelectableComponent.h>
#include <PathMovementComponent.h>
#include <SteeringComponent.h>
#include <TransformComponent.h>

#include <irrlicht/irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace io;
using namespace scene;

void RTSMovementSystem::update ( float timestep ) {
    if (EventReceiver::getMouseState()->rightPressed && !mouseClicked) {
		mouseClicked = true;
			
		// Calculate click point on terrain
		ISceneCollisionManager* colmgr = RenderManager::renderManager.getSceneManager()->getSceneCollisionManager();
		line3df ray = colmgr->getRayFromScreenCoordinates(EventReceiver::getMouseState()->position);
		
		vector3df point;
		triangle3df triangle;
		ISceneNode* node;
		
		// Check if the user clicked on terrain
		if (!colmgr->getCollisionPoint(ray, RenderManager::renderManager.getSceneManager()->getSceneNodeFromName("MainTerrain")->getTriangleSelector(), point, triangle, node))
			return;
		
		// Get the object object manager
		ObjectManager* mgr = &ObjectManager::manager;
		
		// Get all objects with an RTSMovementComponent
		std::list<int> objects = mgr->getObjectsWithComponent("RTSMovementComponent");
		
		for (int i : objects) {
			// Get only selected selectable objects
			SelectableComponent* selectComp = mgr->getObjectComponent<SelectableComponent>(i, "SelectableComponent");
			
			if (selectComp == nullptr || selectComp->selected == false)
				continue;
			
			// Get only selected selectable objects
			TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
			
			if (transComp == nullptr)
				continue;
			
			/*
			// Get object's path mover
			PathMovementComponent* pathComp = mgr->getObjectComponent<PathMovementComponent>(i, "PathMovementComponent");
			
			if (pathComp == nullptr)
				continue;
			
			pathComp->waypoints.push(point);
			*/
			
			// Get object's steering system
			SteeringComponent* steerComp = mgr->getObjectComponent<SteeringComponent>(i, "SteeringComponent");
			
			if (steerComp == nullptr)
				continue;
			
			if (steerComp->path.ended())
				steerComp->path.resetPath();
			
			if (steerComp->path.getWaypoints().size() == 0)
				steerComp->path.addNode(transComp->worldPosition);
			
			steerComp->path.addNode(point);
		}
	} else {
		if (!EventReceiver::getMouseState()->rightPressed)
			mouseClicked = false;
	}
}


void RTSMovementSystem::draw ( float timestep ) {
    
}
