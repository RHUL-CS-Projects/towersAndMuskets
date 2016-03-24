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
#include <GroupPathMover.h>

#include <thread>

using namespace irr;
using namespace core;
using namespace video;
using namespace io;
using namespace scene;

void RTSLogicSystem::update ( float timestep ) {
	updateClickPoints();
	pathMover = GroupPathMover();
	
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
		currentTeamComp = mgr->getObjectComponent<TeamComponent>(i, "TeamComponent");
		
		// Assert that the object has the necessary components to operate correctly
		if (!currentRTSComp || !currentSteerComp || !currentHealthComp 
			|| !currentTransComp || !currentRendComp || !currentTeamComp)
			continue;
		
		currentSelectComp = mgr->getObjectComponent<SelectableComponent>(i, "SelectableComponent");
		currentAnimComp = mgr->getObjectComponent<AnimatorComponent>(i, "AnimatorComponent");
		currentPathComp = mgr->getObjectComponent<PathMovementComponent>(i, "PathMovementComponent");
		
		RTS_UNIT_STATE currentState;
		if (currentRTSComp->stateStack.size() == 0)
			currentRTSComp->stateStack.push(IDLE);
		
		currentState = currentRTSComp->stateStack.top();
		bool selected = (currentSelectComp == nullptr) ? false : ((currentSelectComp->selected) ? true : false);
		
		if (currentHealthComp->health <= 0) {
			currentRTSComp->stateStack.push(DEAD);
			currentState = currentRTSComp->stateStack.top();
		}
		
		// Perform behaviours of current state
		switch (currentState) {
		case IDLE:
			stateIdle(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "IDLE" << std::endl;
			break;
			
		case WALKING:
			stateWalking(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "WALKING" << std::endl;
			break;
			
		case MOVE_TO_ATTACK:
			stateMoveToAttack(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "MOVE_TO_ATTACK" << std::endl;
			break;
			
		case ATTACKING:
			stateAttacking(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "ATTACKING" << std::endl;
			break;
			
		case RELOADING:
			stateReloading(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "RELOADING" << std::endl;
			break;
			
		case DEAD:
			stateDead(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "DEAD" << std::endl;
			break;
			
		case TAKE_AIM:
			stateTakeAim(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "TAKE_AIM" << std::endl;
			break;
			
		case RELEASE_AIM:
			stateReleaseAim(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "RELEASE_AIM" << std::endl;
			break;
			
		case GARRISSONED:
			stateGarrissoned(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "GARRISSONED" << std::endl;
			break;
			
		case CLIMB_UP:
			stateClimbUp(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "CLIMB_UP" << std::endl;
			break;
			
		case CLIMB_DOWN:
			stateClimbDown(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "CLIMB_DOWN" << std::endl;
			break;
			
		case MOVE_TO_TOWER:
			stateMoveToTower(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "MOVE_TO_TOWER" << std::endl;
			break;
			
		case AIMING:
			stateAiming(mgr, i);
			if (DebugValues::UNIT_STATES && selected) std::cout << "AIMING" << std::endl;
			break;
		}
	}
	
	// Update flashing towers
	objects = mgr->getObjectsWithComponent("TowerComponent");
	for (int i : objects) {
		TowerComponent* resComp = mgr->getObjectComponent<TowerComponent>(i, "TowerComponent");
		
		if (resComp->flashNum > 0) {
			if (resComp->flashTimer > 0) {
				resComp->flashTimer--;				
			} else {
				resComp->flashNum--;
				resComp->flashTimer = 5;
				resComp->flashOn = !resComp->flashOn;
			}
		}
	}
	
	pathMover.startCalc();
	rightMousePressed = false;
}

void RTSLogicSystem::draw ( float timestep ) {
    // Draw flashing boxes under clicked towers
	std::list<int> objects = Game::game.getObjMgr()->getObjectsWithComponent("TowerComponent");
	
	for (int i : objects) {
		TowerComponent* towerComp = Game::game.getObjMgr()->getObjectComponent<TowerComponent>(i, "TowerComponent");
		
		SMaterial m;
		
		ITerrainSceneNode* terrain = (ITerrainSceneNode*)Game::game.getRendMgr()->getSceneManager()->getSceneNodeFromName("MainTerrain");
		
		if (towerComp->flashNum > 0 && towerComp->flashOn) {
			m.Lighting = false;
			m.Thickness = 1.0f;
			Game::game.getRendMgr()->getDriver()->setMaterial(m);
			Game::game.getRendMgr()->getDriver()->setTransform(video::ETS_WORLD, IdentityMatrix);
			
			TransformComponent* transComp = Game::game.getObjMgr()->getObjectComponent<TransformComponent>(i, "TransformComponent");
			vector3df pos = transComp->worldPosition;
			float gridSize = Game::game.getObjMgr()->worldManager->gridSize;
			
			vector3df p1 = pos+vector3df(-gridSize,2,-gridSize);
			vector3df p2 = pos+vector3df(-gridSize,2,gridSize);
			vector3df p3 = pos+vector3df(gridSize,2,-gridSize);
			vector3df p4 = pos+vector3df(gridSize,2,gridSize);
			
			p1.Y = terrain->getHeight(p1.X, p1.Z) + 0.5f;
			p2.Y = terrain->getHeight(p2.X, p2.Z) + 0.5f;
			p3.Y = terrain->getHeight(p3.X, p3.Z) + 0.5f;
			p4.Y = terrain->getHeight(p4.X, p4.Z) + 0.5f;

			Game::game.getRendMgr()->getDriver()->draw3DLine(p1, p2, SColor(255,255,255,255));
			Game::game.getRendMgr()->getDriver()->draw3DLine(p1, p3, SColor(255,255,255,255));
			Game::game.getRendMgr()->getDriver()->draw3DLine(p3, p4, SColor(255,255,255,255));
			Game::game.getRendMgr()->getDriver()->draw3DLine(p2, p4, SColor(255,255,255,255));
		}
	}
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
	
	if (currentSelectComp != nullptr && currentSelectComp->selected && currentRTSComp->towerID == -1) {
		pathMover.requestPath(id, point);
	} else {
		std::thread t(&RTSLogicSystem::calcPathSynch, mgr, id, point);
		t.detach();
	}
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
	
	RTSLogicComponent* rtsComp = mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent");
	if (rtsComp != nullptr) {
		rtsComp->stateStack.pop();
		rtsComp->pathSet = true;
	}
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
	
	if (otherHealthComp == nullptr || otherHealthComp->health <= 0)
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
	TeamComponent* otherTeamComp = mgr->getObjectComponent<TeamComponent>(target, "TeamComponent");
	
	return otherTeamComp->teamID != currentTeamComp->teamID;
}

vector3df RTSLogicSystem::attackTargetPosition(ObjectManager* mgr) {
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(currentRTSComp->attackTargetID, "TransformComponent");
	
	if (otherTransComp != nullptr)
		return otherTransComp->worldPosition;
	
	return currentTransComp->worldPosition;
}

vector3df RTSLogicSystem::towerTargetPosition ( ObjectManager* mgr, int towerID ) {
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(towerID, "TransformComponent");
	TowerComponent* towerComp = mgr->getObjectComponent<TowerComponent>(towerID, "TowerComponent");
	
	if (otherTransComp != nullptr) {
		/*return vector3df(otherTransComp->worldPosition.X + towerComp->doorOffset.X * mgr->worldManager->gridSize, 
						 0 + towerComp->doorOffset.Y * mgr->worldManager->gridSize, 
						 otherTransComp->worldPosition.Z + towerComp->doorOffset.Z * mgr->worldManager->gridSize);*/
		vector3df nearest = otherTransComp->worldPosition;
		double dist = -1;
		double angle = 360 / 8;
		double gridSize = mgr->worldManager->gridSize;
		vector3df mid = otherTransComp->worldPosition;
		
		for (int i = 0; i < 8; i++) {
			vector3df unit(cos(degToRad(i*angle)) * gridSize, 0, sin(degToRad(i*angle)) * gridSize);
			unit *= 1.5;
			vector3df tempNearest = mid + unit;
		
			if (!mgr->worldManager->checkPassable(tempNearest)) continue;
			
			double tempDist = (currentTransComp->worldPosition - tempNearest).getLengthSQ();
			if (dist == -1 || tempDist < dist) {
				dist = tempDist;
				nearest = tempNearest;
			}
		}
		
		return nearest;
	}

	return currentTransComp->worldPosition;
}

int RTSLogicSystem::getNearestOnOtherTeam ( ObjectManager* mgr, int id ) {
	std::vector<int> objects = TeamComponent::getObjectsOnTeam((currentTeamComp->teamID + 1) % 2);
	
	double dist = currentRTSComp->rangeInSquares * mgr->worldManager->gridSize;
	// Multiply range by 1.5 to get a tentative line of sight
	dist *= 1.5;
	dist *= dist;
	
	int best = -1;
	
	for (int i : objects) {
		if (!mgr->objectExists(i)) {
			continue;
		}
		
		if (!mgr->objectHasComponent(i, "TeamComponent") || !mgr->objectHasComponent(i, "HealthComponent") || !mgr->objectHasComponent(i, "TransformComponent"))
			continue;
		
		TeamComponent* otherTeamComp = mgr->getObjectComponent<TeamComponent>(i, "TeamComponent");
		TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
		HealthComponent* otherHealthComp = mgr->getObjectComponent<HealthComponent>(i, "HealthComponent");
		
		if (otherHealthComp->health <= 0) continue;
		
		double dx = otherTransComp->worldPosition.X - currentTransComp->worldPosition.X;
		double dz = otherTransComp->worldPosition.Z - currentTransComp->worldPosition.Z;
		double distsq = dx*dx + dz*dz;
		
		if (distsq < dist) {
			dist = distsq;
			best = i;
		}
	}
	
	return best;
}

void RTSLogicSystem::stateDead ( ObjectManager* mgr, int id ) {
	if (currentRTSComp->garrissoned) {
		TowerComponent* towerComp = mgr->getObjectComponent<TowerComponent>(currentRTSComp->towerID, "TowerComponent");
		for (int i = 0; i < 4; i++) {
			if (towerComp->freeSpace[i] == id) {
				towerComp->freeSpace[i] = -1;
				break;
			}
		}
	}
	
	mgr->detachComponent(id, "SteeringComponent");
	mgr->detachComponent(id, "RTSLogicComponent");
	mgr->detachComponent(id, "TeamComponent");
	
	AnimatorComponent* animComp = mgr->getObjectComponent<AnimatorComponent>(id, "AnimatorComponent");
	RenderComponent* rendComp = mgr->getObjectComponent<RenderComponent>(id, "RenderComponent");
	
	SelectableComponent* selectComp = mgr->getObjectComponent<SelectableComponent>(id, "SelectableComponent");
	if (selectComp != nullptr) {
		selectComp->selected = false;
		selectComp->sceneNode->setVisible(false);
	}
	mgr->detachComponent(id, "SelectableComponent");
	
	HealthComponent* healthComp = mgr->getObjectComponent<HealthComponent>(id, "HealthComponent");
	if (healthComp != nullptr) {
		healthComp->alpha = 0;
		healthComp->billboardNode->setVisible(false);
	}
	mgr->detachComponent(id, "HealthComponent");
	
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
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->walkSound->play();
		return;
	}
	
	int nearestEnemy = getNearestOnOtherTeam(mgr, id);
	// Start walking to attack nearby target
	if (nearestEnemy >= 0) {
		currentRTSComp->attackTargetID = nearestEnemy;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_ATTACK);
		return;
	}
	
	// Start walking to tower target
	if (selected() && clickedTower >= 0) {
		currentRTSComp->towerID = clickedTower;
		currentRTSComp->pathSet = false;
		currentSteerComp->path.resetPath();
		
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
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->walkSound->play();
		return;
	}
}

void RTSLogicSystem::stateMoveToAttack ( ObjectManager* mgr, int id ) {
	setAnimation("WALK", true);
	
	if (!currentRTSComp->pathSet) {
		RTSLogicComponent* otherRTSComp = mgr->getObjectComponent<RTSLogicComponent>(currentRTSComp->attackTargetID, "RTSLogicComponent");
		
		if (otherRTSComp == nullptr || !otherRTSComp->garrissoned) {
			setPath(mgr, id, attackTargetPosition(mgr));
			currentRTSComp->pathSet = true;
		} else {
			setPath(mgr, id, towerTargetPosition(mgr, otherRTSComp->towerID));
			currentRTSComp->pathSet = true;
		}
		
		return;
	}
	
	// Start walking to attack target
	if (selected() && clickedObject >= 0 && checkTargetDifferentTeam(mgr, clickedObject) && clickedObject != currentRTSComp->attackTargetID) {
		currentRTSComp->attackTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_ATTACK);
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->walkSound->play();
		return;
	}
	
	// Start walking to tower target
	if (selected() && clickedTower >= 0) {
		currentRTSComp->towerID = clickedTower;
		currentRTSComp->pathSet = false;
		currentSteerComp->path.resetPath();
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_TOWER);
		return;
	}
	
	float distSq = (mgr->worldManager->gridSize * currentRTSComp->rangeInSquares) * (mgr->worldManager->gridSize * currentRTSComp->rangeInSquares);
	//std::cout << currentRTSComp->attackTargetID << std::endl;
	
	// Start aiming at target
	if (currentRTSComp->attackTargetID >= 0 && distanceToObjectSq(mgr, currentRTSComp->attackTargetID) < distSq) {
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
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->walkSound->play();
		return;
	}
	
	// Finished walking
	if (currentSteerComp->path.ended()) {
		currentRTSComp->attackTargetID = -1;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(IDLE);
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
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->walkSound->play();
		return;
	}
	
	// Start walking to tower target
	if (selected() && clickedTower >= 0) {
		currentRTSComp->towerID = clickedTower;
		currentRTSComp->pathSet = false;
		currentSteerComp->path.resetPath();
		
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
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->walkSound->play();
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
	
	currentTransComp->worldPosition = towerTargetPosition(mgr, currentRTSComp->towerID);
	
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
	currentSteerComp->path.resetPath();
	
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
		currentRTSComp->towerID = -1;
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
	/*if (!currentRTSComp->pathSet) {
		setPath(mgr, id, towerTargetPosition(mgr));
		currentRTSComp->pathSet = true;
	}*/
	
	// Start walking to attack target
	if (selected() && clickedObject >= 0 && checkTargetDifferentTeam(mgr, clickedObject)) {
		currentRTSComp->attackTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.push(CLIMB_DOWN);
		stateClimbDown(mgr, id);
		
		currentRTSComp->towerID = -1;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_ATTACK);
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->walkSound->play();
		return;
	}
	
	// Start walking to tower target
	if (selected() && clickedTower >= 0 && clickedTower != currentRTSComp->towerID) {
		currentRTSComp->pathSet = false;
		currentSteerComp->path.resetPath();
		
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
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->walkSound->play();
		return;
	}
	setAnimation("IDLE", true);
	
	float rangeBonus = currentRTSComp->garrissoned ? 1.5f : 1;
	float dist = (mgr->worldManager->gridSize * currentRTSComp->rangeInSquares * rangeBonus);
	float distSq = dist * dist;
	
	int nearestEnemy = getNearestOnOtherTeam(mgr, id);
	// Start walking to attack nearby target
	if (nearestEnemy >= 0 && distanceToObjectSq(mgr, nearestEnemy) < distSq) {
		currentRTSComp->attackTargetID = nearestEnemy;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.push(AIMING);
		return;
	}
}

void RTSLogicSystem::stateMoveToTower ( ObjectManager* mgr, int id ) {
	setAnimation("WALK", true);
	
	if (!currentRTSComp->canGarrisson) {
		((StatePlaying*)Game::game.currentState())->message(SHOW_MESSAGE_BAD, "This unit is too large to be garrissoned");
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(IDLE);
		
		currentRTSComp->pathSet = false;
		currentSteerComp->path.resetPath();
		return;
	}
	
	if (!currentRTSComp->pathSet) {
		setPath(mgr, id, towerTargetPosition(mgr, currentRTSComp->towerID));
		currentRTSComp->pathSet = true;
		
		TowerComponent* resComp = mgr->getObjectComponent<TowerComponent>(currentRTSComp->towerID, "TowerComponent");
		resComp->flashNum = 8;
		resComp->flashTimer = 5;
		
		Game::game.resources.loadSound("garrisson.ogg")->play();
		return;
	}
	
	// Could not move to tower
	if (currentSteerComp->path.ended() && distanceToObjectSq(mgr, currentRTSComp->towerID) > (mgr->worldManager->gridSize*2)*(mgr->worldManager->gridSize*2)) {
		currentRTSComp->towerID = -1;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(IDLE);
		
		currentRTSComp->pathSet = false;
		currentSteerComp->path.resetPath();
		return;
	}
	
	// Start walking to attack target
	if (selected() && clickedObject >= 0 && checkTargetDifferentTeam(mgr, clickedObject)) {
		currentRTSComp->towerID = -1;
		currentRTSComp->attackTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(MOVE_TO_ATTACK);
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->walkSound->play();
		return;
	}
	
	// Start walking to tower target
	if (selected() && clickedTower >= 0 && clickedTower != currentRTSComp->towerID) {
		currentRTSComp->towerID = clickedTower;
		currentRTSComp->pathSet = false;
		currentSteerComp->path.resetPath();
		
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
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->walkSound->play();
		return;
	}
	
	// Start climbing tower
	if (currentSteerComp->path.ended() || distanceToObjectSq(mgr, currentRTSComp->towerID) < (mgr->worldManager->gridSize*1.5)*(mgr->worldManager->gridSize*1.5)) {
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(GARRISSONED);
		currentRTSComp->stateStack.push(CLIMB_UP);
		return;
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
		
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->walkSound->play();
		return;
	}
	
	// Start moving to tower
	
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
		
		mgr->getObjectComponent<RTSLogicComponent>(id, "RTSLogicComponent")->walkSound->play();
		return;
	}
	
	// Attack
	if (currentRTSComp->shootCounter <= 0 && targetAlive(mgr)) {
		currentRTSComp->shootCounter = currentRTSComp->shootDelay;
		currentRTSComp->stateStack.push(RELOADING);
		currentRTSComp->stateStack.push(ATTACKING);
		
		HealthComponent* otherHealthComp = mgr->getObjectComponent<HealthComponent>(currentRTSComp->attackTargetID, "HealthComponent");
		otherHealthComp->health -= currentRTSComp->attackDamage * ((currentRTSComp->garrissoned) ? 2 : 1);
		otherHealthComp->alpha = 2;
		
		currentRTSComp->shootSound->setPosition(currentTransComp->worldPosition.X, currentTransComp->worldPosition.Y, currentTransComp->worldPosition.Z);
		currentRTSComp->shootSound->setVolume(100);
		currentRTSComp->shootSound->setRelativeToListener(false);
		currentRTSComp->shootSound->setAttenuation(0.1f);
		currentRTSComp->shootSound->setPitch(0.9f + (1.0f/1000*(rand()%1000))*0.2f);
		currentRTSComp->shootSound->play();
		
		vector3df enemyPos = attackTargetPosition(mgr);
		vector3df dif = enemyPos - currentTransComp->worldPosition;
		
		vector3df start = currentTransComp->worldPosition + vector3df(sin(atan2(dif.X, dif.Z))*4, 7.5, cos(atan2(dif.X, dif.Z))*4);
		vector3df end = enemyPos + vector3df(0, 7.5, 0);
		((StatePlaying*)Game::game.currentState())->particleManager.addSmokeTrailParticle(start, end);
		
		FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(id, "FaceDirectionComponent");
	
		if (faceComp != nullptr)
			((StatePlaying*)Game::game.currentState())->particleManager.addMuzzleFlashParticle(currentTransComp->worldPosition, vector3df(0, faceComp->currentYRot, 0));
		
		return;
	}
	
	float rangeBonus = currentRTSComp->garrissoned ? 1.5f : 1;
	float dist = (mgr->worldManager->gridSize * currentRTSComp->rangeInSquares * rangeBonus);
	float distSq = dist * dist;
	
	// Go back to idling
	if (!targetAlive(mgr) || distanceToObjectSq(mgr, currentRTSComp->attackTargetID) > distSq) {
		currentRTSComp->attackTargetID = -1;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(currentRTSComp->garrissoned ? GARRISSONED : IDLE);
		currentRTSComp->stateStack.push(RELEASE_AIM);
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






