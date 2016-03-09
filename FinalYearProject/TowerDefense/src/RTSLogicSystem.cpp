#include <RTSLogicSystem.h>
#include <EventReceiver.h>
#include <ObjectManager.h>
#include <list>
#include <RenderManager.h>
#include <StatePlaying.h>

#include <RTSLogicComponent.h>
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
#include <Enums.h>

#include <thread>

using namespace irr;
using namespace core;
using namespace video;
using namespace io;
using namespace scene;

void RTSLogicSystem::update ( float timestep ) {
	updateClickPoints();
	
    // Get the object object manager
	ObjectManager* mgr = Game::game.getObjMgr();
	
	// Get all objects with an RTSLogicComponent
	std::list<int> objects = mgr->getObjectsWithComponent("RTSLogicComponent");
	
	// Iterate over objects
	for (int i : objects) {
		
		currentRTSComp = mgr->getObjectComponent<RTSLogicComponent>(i, "RTSLogicComponent");
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
		
		RTS_UNIT_STATE currentState;
		if (currentRTSComp->stateStack.size() == 0)
			currentRTSComp->stateStack.push(IDLE);
		
		currentState = currentRTSComp->stateStack.top();
		bool selected = (currentSelectComp == nullptr) ? false : ((currentSelectComp->selected) ? true : false);
		
		if (currentHealthComp->health <= 0)
			currentRTSComp->stateStack.push(DEAD);
		
		// Perform behaviours of current state
		switch (currentState) {
		case IDLE:
			stateIdle(mgr, i);
			if (selected) std::cout << "IDLE" << std::endl;
			break;
			
		case WALKING:
			stateWalking(mgr, i);
			if (selected) std::cout << "WALKING" << std::endl;
			break;
			
		case MOVE_TO_ATTACK:
			stateMoveToAttack(mgr, i);
			if (selected) std::cout << "MOVE_TO_ATTACK" << std::endl;
			break;
			
		case ATTACKING:
			stateAttacking(mgr, i);
			if (selected) std::cout << "ATTACKING" << std::endl;
			break;
			
		case RELOADING:
			stateReloading(mgr, i);
			if (selected) std::cout << "RELOADING" << std::endl;
			break;
			
		case DEAD:
			stateDead(mgr, i);
			if (selected) std::cout << "DEAD" << std::endl;
			break;
			
		case TAKE_AIM:
			stateTakeAim(mgr, i);
			if (selected) std::cout << "TAKE_AIM" << std::endl;
			break;
			
		case RELEASE_AIM:
			stateReleaseAim(mgr, i);
			if (selected) std::cout << "RELEASE_AIM" << std::endl;
			break;
			
		case GARRISSONED:
			stateGarrissoned(mgr, i);
			if (selected) std::cout << "GARRISSONED" << std::endl;
			break;
			
		case CLIMB_UP:
			stateClimbUp(mgr, i);
			if (selected) std::cout << "CLIMB_UP" << std::endl;
			break;
			
		case CLIMB_DOWN:
			stateClimbDown(mgr, i);
			if (selected) std::cout << "CLIMB_DOWN" << std::endl;
			break;
			
		case MOVE_TO_TOWER:
			stateMoveToTower(mgr, i);
			if (selected) std::cout << "MOVE_TO_TOWER" << std::endl;
			break;
			
		case AIMING:
			stateAiming(mgr, i);
			if (selected) std::cout << "AIMING" << std::endl;
			break;
		}
	}
	
	rightMousePressed = false;
}

void RTSLogicSystem::draw ( float timestep ) {
    
}

void RTSLogicSystem::updateClickPoints() {
	clickedObject = -1;
	clickedTower = -1;
	
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
				if (mgr->getObjectComponent<HealthComponent>(hitID, "HealthComponent") != nullptr) {
					clickedObject = hitID;
					objectPoint = mgr->getObjectComponent<TransformComponent>(hitID, "TransformComponent")->worldPosition;
				}
				
				if (mgr->getObjectComponent<TowerComponent>(hitID, "TowerComponent") != nullptr) {
					clickedTower = hitID;
				}
			}
		}
	} else {
		if (!EventReceiver::getMouseState()->rightPressed)
			rightMouseDown = false;
	}
}

void RTSLogicSystem::setPath ( ObjectManager* mgr, int id, vector3df point ) {
	currentRTSComp->stateStack.push(WAIT);
	
	std::thread t(&RTSLogicSystem::calcPathSynch, mgr, id, point);
	t.detach();
}

void RTSLogicSystem::calcPathSynch ( ObjectManager* mgr, int id, vector3df point ) {
	
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
	
	mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->stateStack.pop();
	mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->pathSet = true;
}

bool RTSLogicSystem::animationComplete() {
	if (currentAnimComp != nullptr) {
		if (currentRendComp->sceneNode->getFrameNr() >= currentRendComp->sceneNode->getEndFrame())
			return true;
		else
			return false;
	}
	
	return true;
}

bool RTSLogicSystem::selected() {
	if (currentSelectComp == nullptr)
		return false;
	
	return currentSelectComp->selected;
}

void RTSLogicSystem::setAnimation ( std::string animation, bool loop ) {
	if (currentAnimComp != nullptr) {
		if (currentAnimComp->currentAnimation != animation) {
			currentAnimComp->setAnimation(animation, currentRendComp->sceneNode);
			currentRendComp->sceneNode->setLoopMode(loop);
		}
	}
}

float RTSLogicSystem::distanceToObjectSq ( ObjectManager* mgr, int otherID ) {
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(otherID, "TransformComponent");
	
	return currentTransComp->worldPosition.getDistanceFromSQ(otherTransComp->worldPosition);
}

bool RTSLogicSystem::targetAlive ( ObjectManager* mgr ) {
	HealthComponent* otherHealthComp = mgr->getObjectComponent<HealthComponent>(currentRTSComp->attackTargetID, "HealthComponent");
	
	if (otherHealthComp->health <= 0)
		return false;
	
	return true;
}

void RTSLogicSystem::faceTarget ( ObjectManager* mgr, int id ) {
	FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(id, "FaceDirectionComponent");
	
	if (faceComp != nullptr) {
		if (currentRTSComp->attackTargetID > -1) {
			vector3df dif = currentTransComp->worldPosition - attackTargetPosition(mgr);
			faceComp->targetYRot = radToDeg(atan2(dif.X, dif.Z));
		}
	}
}

bool RTSLogicSystem::checkTargetDifferentTeam ( ObjectManager* mgr, int target ) {
	RTSLogicComponent* otherRTSComp = mgr->getObjectComponent<RTSLogicComponent>(target, "RTSLogicComponent");
	
	return otherRTSComp->teamID != currentRTSComp->teamID;
}

vector3df RTSLogicSystem::attackTargetPosition(ObjectManager* mgr) {
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(currentRTSComp->attackTargetID, "TransformComponent");
	
	if (otherTransComp != nullptr)
		return otherTransComp->worldPosition;
	
	return currentTransComp->worldPosition;
}

vector3df RTSLogicSystem::towerTargetPosition ( ObjectManager* mgr ) {
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(currentRTSComp->towerID, "TransformComponent");
	TowerComponent* towerComp = mgr->getObjectComponent<TowerComponent>(currentRTSComp->towerID, "TowerComponent");
	
	if (otherTransComp != nullptr)
		return vector3df(otherTransComp->worldPosition.X + towerComp->doorOffset.X * mgr->worldManager->gridSize, 
						 0 + towerComp->doorOffset.Y * mgr->worldManager->gridSize, 
						 otherTransComp->worldPosition.Z + towerComp->doorOffset.Z * mgr->worldManager->gridSize);

	return currentTransComp->worldPosition;
}

void RTSLogicSystem::stateDead ( ObjectManager* mgr, int id ) {
	mgr->detachComponent(id, "SteeringComponent");
	mgr->detachComponent(id, "RTSLogicComponent");
	mgr->detachComponent(id, "SelectableComponent");
	
	AnimatorComponent* animComp = mgr->getObjectComponent<AnimatorComponent>(id, "AnimatorComponent");
	RenderComponent* rendComp = mgr->getObjectComponent<RenderComponent>(id, "RenderComponent");
	
	if (animComp != nullptr && rendComp != nullptr) {
		animComp->setAnimation("DEATH1", rendComp->sceneNode);
		rendComp->sceneNode->setLoopMode(false);
	}
}

void RTSLogicSystem::stateIdle ( ObjectManager* mgr, int id ) {
	if (currentSteerComp->velocity.getLength() > 0.01) {
		setAnimation("WALK", true);
		FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(id, "FaceDirectionComponent");
		if (faceComp != nullptr)
			faceComp->targetYRot = radToDeg(atan2(-currentSteerComp->velocity.X,-currentSteerComp->velocity.Z));
	} else {
		setAnimation("IDLE", true);
	}
	
	// Start walking to attack target
	if (selected() && clickedObject >= 0 && checkTargetDifferentTeam(mgr, clickedObject)) {
		currentRTSComp->attackTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_ATTACK);
		return;
	}
	
	// Start walking to tower target
	if (selected() && clickedTower >= 0) {
		currentRTSComp->towerID = clickedTower;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_TOWER);
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		currentRTSComp->attackTargetID = -1;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->terrainPoint = terrainPoint;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(WALKING);
		return;
	}
}

void RTSLogicSystem::stateMoveToAttack ( ObjectManager* mgr, int id ) {
	setAnimation("WALK", true);
	
	if (!currentRTSComp->pathSet) {
		setPath(mgr, id, attackTargetPosition(mgr));
		currentRTSComp->pathSet = true;
	}
	
	// Start walking to attack target
	if (selected() && clickedObject >= 0 && checkTargetDifferentTeam(mgr, clickedObject) && clickedObject != currentRTSComp->attackTargetID) {
		currentRTSComp->attackTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_ATTACK);
		return;
	}
	
	// Start walking to tower target
	if (selected() && clickedTower >= 0) {
		currentRTSComp->towerID = clickedTower;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_TOWER);
		return;
	}
	
	float distSq = (mgr->worldManager->gridSize * currentRTSComp->rangeInSquares) * (mgr->worldManager->gridSize * currentRTSComp->rangeInSquares);
	
	// Start aiming at target
	if (distanceToObjectSq(mgr, currentRTSComp->attackTargetID) < distSq) {
		currentRTSComp->pathSet = false;
		currentSteerComp->path.resetPath();
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(AIMING);
		currentRTSComp->stateStack.push(TAKE_AIM);
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		currentRTSComp->attackTargetID = -1;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->terrainPoint = terrainPoint;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(WALKING);
		return;
	}
}

void RTSLogicSystem::stateAttacking ( ObjectManager* mgr, int id ) {
	setAnimation("SHOOT", false);
	
	// Reload
	if (animationComplete()) {
		currentRTSComp->stateStack.pop();
		return;
	}
}

void RTSLogicSystem::stateReloading ( ObjectManager* mgr, int id ) {
	setAnimation("RELOAD", false);
	
	// Return to aiming
	if (animationComplete()) {
		currentRTSComp->stateStack.pop();
		return;
	}
}

void RTSLogicSystem::stateWalking ( ObjectManager* mgr, int id ) {
	setAnimation("WALK", true);
	
	if (!currentRTSComp->pathSet) {
		setPath(mgr, id, currentRTSComp->terrainPoint);
		currentRTSComp->pathSet = true;
	}
	
	// Start walking to attack target
	if (selected() && clickedObject >= 0 && checkTargetDifferentTeam(mgr, clickedObject)) {
		currentRTSComp->attackTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_ATTACK);
		return;
	}
	
	// Start walking to tower target
	if (selected() && clickedTower >= 0) {
		currentRTSComp->towerID = clickedTower;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_TOWER);
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		currentRTSComp->attackTargetID = -1;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->terrainPoint = terrainPoint;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(WALKING);
		return;
	}
	
	// Position reached
	if (currentSteerComp->path.ended()) {
		currentRTSComp->pathSet = false;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(IDLE);
		return;
	}
}

void RTSLogicSystem::stateClimbDown ( ObjectManager* mgr, int id ) {
	setAnimation("IDLE", true);
	currentRTSComp->garrissoned = true;
	
	TowerComponent* towerComp = mgr->getObjectComponent<TowerComponent>(currentRTSComp->towerID, "TowerComponent");
	TransformComponent* towerTransComp = mgr->getObjectComponent<TransformComponent>(currentRTSComp->towerID, "TransformComponent");
	
	currentTransComp->worldPosition = towerTargetPosition(mgr);
	
	for (int i = 0; i < 4; i++) {
		if (towerComp->freeSpace[i] == id) {
			towerComp->freeSpace[i] = -1;
			break;
		}
	}
	
	currentSteerComp->enabled = true;
	currentSteerComp->velocity = vector3df(0,0,0);
	currentRTSComp->stateStack.pop();
	
	FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(id, "FaceDirectionComponent");
	
	if (faceComp != nullptr) {
		faceComp->targetYRot = 0;
		faceComp->currentYRot = 0;
	}
	setAnimation("IDLE", true);
	currentRTSComp->garrissoned = false;
	
	// Climbed down
	
}

void RTSLogicSystem::stateClimbUp ( ObjectManager* mgr, int id ) {
	setAnimation("IDLE", true);
	if (!currentRTSComp->canGarrisson) {
		((StatePlaying*)Game::game.currentState())->message(SHOW_MESSAGE_BAD, "This unit is too large to be garrissoned");
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(IDLE);
		return;
	}
	
	TowerComponent* towerComp = mgr->getObjectComponent<TowerComponent>(currentRTSComp->towerID, "TowerComponent");
	TransformComponent* towerTransComp = mgr->getObjectComponent<TransformComponent>(currentRTSComp->towerID, "TransformComponent");
	
	int garrissonPos = 0;
	bool foundSpace = false;
	for (int i = 0; i < 4; i++) {
		if (towerComp->freeSpace[i] == -1) {
			garrissonPos = i;
			towerComp->freeSpace[i] = id;
			foundSpace = true;
			break;
		}
	}
	
	if (!foundSpace) {
		((StatePlaying*)Game::game.currentState())->message(SHOW_MESSAGE_BAD, "Not enough space to garrisson this unit");
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(IDLE);
		return;
	}
	
	currentRTSComp->garrissoned = true;
	
	int xTowerPos = garrissonPos % 2;
	int yTowerPos = floor(garrissonPos / 2);
	
	float gridSize = mgr->worldManager->gridSize;
	
	currentTransComp->worldPosition.Y = towerTransComp->worldPosition.Y + towerComp->platformHeight;
	currentTransComp->worldPosition.X = (towerTransComp->worldPosition.X - 0.5f * gridSize) + xTowerPos * gridSize;
	currentTransComp->worldPosition.Z = (towerTransComp->worldPosition.Z - 0.5f * gridSize) + yTowerPos * gridSize;
	
	currentSteerComp->enabled = false;
	currentRTSComp->stateStack.pop();
}

void RTSLogicSystem::stateGarrissoned ( ObjectManager* mgr, int id ) {
	if (!currentRTSComp->pathSet) {
		setPath(mgr, id, towerTargetPosition(mgr));
		currentRTSComp->pathSet = true;
	}
	
	// Start walking to attack target
	if (selected() && clickedObject >= 0 && checkTargetDifferentTeam(mgr, clickedObject)) {
		currentRTSComp->attackTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.push(CLIMB_DOWN);
		stateClimbDown(mgr, id);
		
		currentRTSComp->towerID = -1;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_ATTACK);
		return;
	}
	
	// Start walking to tower target
	if (selected() && clickedTower >= 0 && clickedTower != currentRTSComp->towerID) {
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.push(CLIMB_DOWN);
		stateClimbDown(mgr, id);
		
		currentRTSComp->towerID = clickedTower;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_TOWER);
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.push(CLIMB_DOWN);
		stateClimbDown(mgr, id);
		
		currentRTSComp->towerID = -1;
		
		currentRTSComp->terrainPoint = terrainPoint;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(WALKING);
		return;
	}
	setAnimation("IDLE", true);
}

void RTSLogicSystem::stateMoveToTower ( ObjectManager* mgr, int id ) {
	setAnimation("WALK", true);
	
	if (!currentRTSComp->canGarrisson) {
		((StatePlaying*)Game::game.currentState())->message(SHOW_MESSAGE_BAD, "This unit is too large to be garrissoned");
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(IDLE);
		return;
	}
	
	if (!currentRTSComp->pathSet) {
		setPath(mgr, id, towerTargetPosition(mgr));
		currentRTSComp->pathSet = true;
	}
	
	// Start walking to attack target
	if (selected() && clickedObject >= 0 && checkTargetDifferentTeam(mgr, clickedObject)) {
		currentRTSComp->towerID = -1;
		currentRTSComp->attackTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_ATTACK);
		return;
	}
	
	// Start walking to tower target
	if (selected() && clickedTower >= 0 && clickedTower != currentRTSComp->towerID) {
		currentRTSComp->towerID = clickedTower;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_TOWER);
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		currentRTSComp->towerID = -1;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->terrainPoint = terrainPoint;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(WALKING);
		return;
	}
	
	// Start climbing tower
	if (currentSteerComp->path.ended()) {
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(GARRISSONED);
		currentRTSComp->stateStack.push(CLIMB_UP);
	}
}

void RTSLogicSystem::stateAiming ( ObjectManager* mgr, int id ) {
	setAnimation("AIM", true);
	faceTarget(mgr, id);
	currentRTSComp->shootCounter--;
	
	currentRTSComp->pathSet = false;
	currentSteerComp->path.resetPath();
	
	// Start moving to target
	if (selected() && clickedObject >= 0 && checkTargetDifferentTeam(mgr, clickedObject)) {
		currentRTSComp->towerID = -1;
		currentRTSComp->attackTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		currentRTSComp->shootCounter = currentRTSComp->shootDelay;
		
		if (currentRTSComp->garrissoned) {
			currentRTSComp->stateStack.pop();
			currentRTSComp->stateStack.push(MOVE_TO_ATTACK);
			currentRTSComp->stateStack.push(CLIMB_DOWN);
			currentRTSComp->stateStack.push(RELEASE_AIM);
		} else {
			currentRTSComp->stateStack.pop();
			currentRTSComp->stateStack.push(MOVE_TO_ATTACK);
			currentRTSComp->stateStack.push(RELEASE_AIM);
		}
		return;
	}
	
	// Start moving to tower
	
	// Go back to idling on foot
	
	// Go back to idling in tower
	
	// Start moving to position
	if (selected() && rightMousePressed) {
		currentRTSComp->terrainPoint = terrainPoint;
		currentRTSComp->shootCounter = currentRTSComp->shootDelay;
		
		if (currentRTSComp->garrissoned) {
			currentRTSComp->towerID = -1;
			currentRTSComp->stateStack.pop();
			currentRTSComp->stateStack.push(WALKING);
			currentRTSComp->stateStack.push(CLIMB_DOWN);
			currentRTSComp->stateStack.push(RELEASE_AIM);
		} else {
			currentRTSComp->stateStack.pop();
			currentRTSComp->stateStack.push(WALKING);
			currentRTSComp->stateStack.push(RELEASE_AIM);
		}
		
		return;
	}
	
	// Attack
	if (currentRTSComp->shootCounter <= 0 && targetAlive(mgr)) {
		currentRTSComp->shootCounter = currentRTSComp->shootDelay;
		currentRTSComp->stateStack.push(RELOADING);
		currentRTSComp->stateStack.push(ATTACKING);
		
		HealthComponent* otherHealthComp = mgr->getObjectComponent<HealthComponent>(currentRTSComp->attackTargetID, "HealthComponent");
		otherHealthComp->health -= currentRTSComp->attackDamage * ((currentRTSComp->garrissoned) ? 2 : 1);
		
		currentRTSComp->shootSound->setPosition(currentTransComp->worldPosition.X, currentTransComp->worldPosition.Y, currentTransComp->worldPosition.Z);
		currentRTSComp->shootSound->setVolume(100);
		currentRTSComp->shootSound->setRelativeToListener(false);
		currentRTSComp->shootSound->setAttenuation(0.1f);
		currentRTSComp->shootSound->play();
		return;
	}
}

void RTSLogicSystem::stateReleaseAim ( ObjectManager* mgr, int id ) {
	setAnimation("REST", false);
	
	// Release aim
	if (animationComplete()) {
		currentRTSComp->stateStack.pop();
		return;
	}
}

void RTSLogicSystem::stateTakeAim ( ObjectManager* mgr, int id ) {
	setAnimation("TAKEAIM", false);
	faceTarget(mgr, id);
	
	// Take aim
	if (animationComplete()) {
		currentRTSComp->stateStack.pop();
		return;
	}
}






