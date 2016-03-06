#include <RTSVillagerLogicSystem.h>
#include <EventReceiver.h>
#include <ObjectManager.h>
#include <list>
#include <RenderManager.h>
#include <StatePlaying.h>

#include <RTSVillagerLogicComponent.h>
#include <SelectableComponent.h>
#include <PathMovementComponent.h>
#include <SteeringComponent.h>
#include <TransformComponent.h>
#include <AnimatorComponent.h>
#include <HealthComponent.h>
#include <RenderComponent.h>
#include <TowerComponent.h>

#include <PathFinder.h>
#include <FaceDirectionComponent.h>
#include <Quadtree.h>
#include <Game.h>
#include <irrlicht/irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace io;
using namespace scene;

void RTSVillagerLogicSystem::update ( float timestep ) {
	updateClickPoints();
	
    // Get the object object manager
	ObjectManager* mgr = Game::game.getObjMgr();
	
	// Get all objects with an RTSLogicComponent
	std::list<int> objects = mgr->getObjectsWithComponent("RTSVillagerLogicComponent");
	
	// Iterate over objects
	for (int i : objects) {
		
		currentRTSComp = mgr->getObjectComponent<RTSVillagerLogicComponent>(i, "RTSVillagerLogicComponent");
		currentSteerComp = mgr->getObjectComponent<SteeringComponent>(i, "SteeringComponent");
		currentHealthComp = mgr->getObjectComponent<HealthComponent>(i, "HealthComponent");
		currentTransComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
		currentRendComp = mgr->getObjectComponent<RenderComponent>(i, "RenderComponent");
		
		// Assert that the object has the necessary components to operate correctly
		if (currentRTSComp == nullptr || currentSteerComp == nullptr || currentHealthComp == nullptr 
			|| currentTransComp == nullptr || currentRendComp == nullptr)
			continue;
		
		currentSelectComp = mgr->getObjectComponent<SelectableComponent>(i, "SelectableComponent");
		currentAnimComp = mgr->getObjectComponent<AnimatorComponent>(i, "AnimatorComponent");
		currentPathComp = mgr->getObjectComponent<PathMovementComponent>(i, "PathMovementComponent");
		
		RTS_VILLAGER_STATE currentState;
		if (currentRTSComp->stateStack.size() == 0)
			currentRTSComp->stateStack.push(VILLAGER_IDLE);
		
		currentState = currentRTSComp->stateStack.top();
		bool selected = (currentSelectComp == nullptr) ? false : ((currentSelectComp->selected) ? true : false);
		
		if (currentHealthComp->health <= 0)
			currentRTSComp->stateStack.push(VILLAGER_DEAD);
		
		// Perform behaviours of current state
		switch (currentState) {
		case VILLAGER_IDLE:
			stateIdle(mgr, i);
			//if (selected) std::cout << "IDLE" << std::endl;
			break;
			
		case VILLAGER_WALKING:
			stateWalking(mgr, i);
			//if (selected) std::cout << "WALKING" << std::endl;
			break;

		case VILLAGER_DEAD:
			stateDead(mgr, i);
			//if (selected) std::cout << "DEAD" << std::endl;
			break;
			
		case VILLAGER_GATHER:
			stateGather(mgr, i);
			//if (selected) std::cout << "GATHER" << std::endl;
			break;
			
		case VILLAGER_MOVE_TO_RESOURCE:
			stateMoveToResource(mgr, i);
			//if (selected) std::cout << "MOVE_TO_RESOURCE" << std::endl;
			break;
		}
	}
	
	rightMousePressed = false;
}

void RTSVillagerLogicSystem::draw ( float timestep ) {
    
}

void RTSVillagerLogicSystem::updateClickPoints() {
	clickedObject = -1;

	// Check if the right mouse button was clicked on the terrain or a game object
	if (EventReceiver::getMouseState()->rightPressed && !rightMouseDown) {
		rightMouseDown = true;
		rightMousePressed = true;
		
		// Calculate click point on terrain
		ISceneCollisionManager* colmgr = Game::game.getRendMgr()->getSceneManager()->getSceneCollisionManager();
		line3df ray = colmgr->getRayFromScreenCoordinates(EventReceiver::getMouseState()->position);
		
		triangle3df triangle;
		ISceneNode* node;
		
		// Check if the user clicked on terrain or object
		colmgr->getCollisionPoint(ray, Game::game.getRendMgr()->getSceneManager()->getSceneNodeFromName("MainTerrain")->getTriangleSelector(), terrainPoint, triangle, node);
		
		ObjectManager* mgr = Game::game.getObjMgr();

		int hitID = -1;
		if ((hitID = EventReceiver::getHoverObject()) > -1) {	
			if (mgr->getObjectComponent<TransformComponent>(hitID, "TransformComponent") != nullptr) {	
				if (mgr->getObjectComponent<ResourceComponent>(hitID, "ResourceComponent") != nullptr) {
					clickedObject = hitID;
					objectPoint = mgr->getObjectComponent<TransformComponent>(hitID, "TransformComponent")->worldPosition;
					clickedObjType = mgr->getObjectComponent<ResourceComponent>(hitID, "ResourceComponent")->resType;
				}
			}
		}
	} else {
		if (!EventReceiver::getMouseState()->rightPressed)
			rightMouseDown = false;
	}
}

void RTSVillagerLogicSystem::setPath ( ObjectManager* mgr, int id, vector3df point ) {
	
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
	steerComp->path = pathFinder.findPath(transComp->worldPosition, point);
}

bool RTSVillagerLogicSystem::animationComplete() {
	if (currentAnimComp != nullptr) {
		if (currentRendComp->sceneNode->getFrameNr() >= currentRendComp->sceneNode->getEndFrame())
			return true;
		else
			return false;
	}
	
	return true;
}

bool RTSVillagerLogicSystem::selected() {
	if (currentSelectComp == nullptr)
		return false;
	
	return currentSelectComp->selected;
}

void RTSVillagerLogicSystem::setAnimation ( std::string animation, bool loop ) {
	if (currentAnimComp != nullptr) {
		if (currentAnimComp->currentAnimation != animation) {
			currentAnimComp->setAnimation(animation, currentRendComp->sceneNode);
			currentRendComp->sceneNode->setLoopMode(loop);
		}
	}
}

void RTSVillagerLogicSystem::faceTarget ( ObjectManager* mgr, int id ) {
	FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(id, "FaceDirectionComponent");
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(currentRTSComp->resourceTargetID, "TransformComponent");
	
	if (faceComp != nullptr) {
		if (currentRTSComp->resourceTargetID > -1) {
			vector3df dif = currentTransComp->worldPosition - otherTransComp->worldPosition;
			faceComp->targetYRot = radToDeg(atan2(dif.X, dif.Z));
		}
	}
}

vector3df RTSVillagerLogicSystem::resourceTargetPosition ( ObjectManager* mgr ) {
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(currentRTSComp->resourceTargetID, "TransformComponent");
	ResourceComponent* resComp = mgr->getObjectComponent<ResourceComponent>(currentRTSComp->resourceTargetID, "ResourceComponent");
	
	if (otherTransComp != nullptr)
		return vector3df(otherTransComp->worldPosition.X, 
						 0, 
						 otherTransComp->worldPosition.Z + 1 * mgr->worldManager->gridSize);

	return currentTransComp->worldPosition;
}

void RTSVillagerLogicSystem::stateDead ( ObjectManager* mgr, int id ) {
	mgr->detachComponent(id, "SteeringComponent");
	mgr->detachComponent(id, "RTSVillagerLogicComponent");
	mgr->detachComponent(id, "SelectableComponent");
	
	AnimatorComponent* animComp = mgr->getObjectComponent<AnimatorComponent>(id, "AnimatorComponent");
	RenderComponent* rendComp = mgr->getObjectComponent<RenderComponent>(id, "RenderComponent");
	
	if (animComp != nullptr && rendComp != nullptr) {
		animComp->setAnimation("DEATH1", rendComp->sceneNode);
		rendComp->sceneNode->setLoopMode(false);
	}
}

void RTSVillagerLogicSystem::stateIdle ( ObjectManager* mgr, int id ) {
	setAnimation("IDLE", true);

	// Start walking to resource target
	if (selected() && clickedObject >= 0) {
		currentRTSComp->resourceTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		currentRTSComp->resType = clickedObjType;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_MOVE_TO_RESOURCE);
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		currentRTSComp->resourceTargetID = -1;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->terrainPoint = terrainPoint;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_WALKING);
		return;
	}
}

void RTSVillagerLogicSystem::stateGather ( ObjectManager* mgr, int id ) {
	switch (currentRTSComp->resType) {
		case GOLD:
			setAnimation("DIG", true);
			break;
		case STONE:
			setAnimation("DIG", true);
			break;
		case WOOD:
			setAnimation("CHOP", true);
			break;
	}
	
	faceTarget(mgr, id);
	
	// Start walking to resource
	if (selected() && clickedObject >= 0 && clickedObject != currentRTSComp->resourceTargetID) {
		currentRTSComp->resourceTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		currentRTSComp->resType = clickedObjType;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_MOVE_TO_RESOURCE);
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		currentRTSComp->resourceTargetID = -1;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->terrainPoint = terrainPoint;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_WALKING);
		return;
	}
	
	// Gather again
	if (animationComplete()) {
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_GATHER);
		return;
	}
}

void RTSVillagerLogicSystem::stateWalking ( ObjectManager* mgr, int id ) {
	setAnimation("WALK", true);
	
	if (!currentRTSComp->pathSet) {
		setPath(mgr, id, currentRTSComp->terrainPoint);
		currentRTSComp->pathSet = true;
	}

	// Start walking to resource target
	if (selected() && clickedObject >= 0) {
		currentRTSComp->resourceTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		currentRTSComp->resType = clickedObjType;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_MOVE_TO_RESOURCE);
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		currentRTSComp->resourceTargetID = -1;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->terrainPoint = terrainPoint;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_WALKING);
		return;
	}
	
	// Position reached
	if (currentSteerComp->path.ended()) {
		currentRTSComp->pathSet = false;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_IDLE);
		return;
	}
}

void RTSVillagerLogicSystem::stateMoveToResource ( ObjectManager* mgr, int id ) {
	setAnimation("WALK", true);
	
	if (!currentRTSComp->pathSet) {
		setPath(mgr, id, resourceTargetPosition(mgr));
		currentRTSComp->pathSet = true;
	}

	// Start walking to resource
	if (selected() && clickedObject >= 0 && clickedObject != currentRTSComp->resourceTargetID) {
		currentRTSComp->resourceTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		currentRTSComp->resType = clickedObjType;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_MOVE_TO_RESOURCE);
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		currentRTSComp->resourceTargetID = -1;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->terrainPoint = terrainPoint;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_WALKING);
		return;
	}
	
	// Start gathering resource
	if (currentSteerComp->path.ended()) {
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_GATHER);
	}
}





