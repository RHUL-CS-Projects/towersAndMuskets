#include <RTSAISystem.h>
#include <EventReceiver.h>
#include <ObjectManager.h>
#include <list>
#include <RenderManager.h>
#include <StatePlaying.h>
#include <TransparentMaterialShader.h>

#include <RTSAIComponent.h>
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

void RTSAISystem::update ( float timestep ) {
	pathMover = GroupPathMover();
	
    // Get the object object manager
	ObjectManager* mgr = Game::game.getObjMgr();
	
	// Get all objects with an RTSAIComponent
	std::list<int> objects = mgr->getObjectsWithComponent("RTSAIComponent");
	
	// Iterate over objects
	for (int i : objects) {
		
		currentRTSComp = mgr->getObjectComponent<RTSAIComponent>(i, "RTSAIComponent");
		currentSteerComp = mgr->getObjectComponent<SteeringComponent>(i, "SteeringComponent");
		currentHealthComp = mgr->getObjectComponent<HealthComponent>(i, "HealthComponent");
		currentTransComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
		currentRendComp = mgr->getObjectComponent<RenderComponent>(i, "RenderComponent");
		currentTeamComp = mgr->getObjectComponent<TeamComponent>(i, "TeamComponent");
		
		// Assert that the object has the necessary components to operate correctly
		if (!currentRTSComp || !currentSteerComp || !currentHealthComp 
			|| !currentTransComp || !currentRendComp || !currentTeamComp)
			continue;
		
		currentAnimComp = mgr->getObjectComponent<AnimatorComponent>(i, "AnimatorComponent");
		currentPathComp = mgr->getObjectComponent<PathMovementComponent>(i, "PathMovementComponent");
		
		RTS_UNIT_STATE currentState;
		if (currentRTSComp->stateStack.size() == 0)
			currentRTSComp->stateStack.push(IDLE);
		
		currentState = currentRTSComp->stateStack.top();
		
		if (currentHealthComp->health <= 0) {
			currentRTSComp->stateStack.push(DEAD);
			currentState = currentRTSComp->stateStack.top();
		}
			
		// Perform behaviours of current state
		switch (currentState) {
		case IDLE:
			stateIdle(mgr, i);
			break;
			
		case WALKING:
			stateWalking(mgr, i);
			break;
			
		case MOVE_TO_ATTACK:
			stateMoveToAttack(mgr, i);
			break;
			
		case ATTACKING:
			stateAttacking(mgr, i);
			break;
			
		case RELOADING:
			stateReloading(mgr, i);
			break;
			
		case DEAD:
			stateDead(mgr, i);
			break;
			
		case TAKE_AIM:
			stateTakeAim(mgr, i);
			break;
			
		case RELEASE_AIM:
			stateReleaseAim(mgr, i);
			break;
			
		case AIMING:
			stateAiming(mgr, i);
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
}

void RTSAISystem::draw ( float timestep ) {
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

void RTSAISystem::setPath ( ObjectManager* mgr, int id, vector3df point ) {
	currentRTSComp->stateStack.push(WAIT);
	
	/*if (currentSelectComp != nullptr && currentSelectComp->selected && currentRTSComp->towerID == -1) {
		pathMover.requestPath(id, point);
	} else*/ {
		std::thread t(&RTSAISystem::calcPathSynch, mgr, id, point);
		t.detach();
	}
}

void RTSAISystem::calcPathSynch ( ObjectManager* mgr, int id, vector3df point ) {
	
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
	
	mgr->getObjectComponent<RTSAIComponent>(id, "RTSAIComponent")->stateStack.pop();
	mgr->getObjectComponent<RTSAIComponent>(id, "RTSAIComponent")->pathSet = true;
}

bool RTSAISystem::animationComplete() {
	if (currentAnimComp != nullptr) {
		if (currentRendComp->sceneNode->getFrameNr() >= currentRendComp->sceneNode->getEndFrame())
			return true;
		else
			return false;
	}
	
	return true;
}

void RTSAISystem::setAnimation ( std::string animation, bool loop ) {
	if (currentAnimComp != nullptr) {
		if (currentAnimComp->currentAnimation != animation) {
			currentAnimComp->setAnimation(animation, currentRendComp->sceneNode);
			currentRendComp->sceneNode->setLoopMode(loop);
		}
	}
}

float RTSAISystem::distanceToObjectSq ( ObjectManager* mgr, int otherID ) {
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(otherID, "TransformComponent");
	
	return currentTransComp->worldPosition.getDistanceFromSQ(otherTransComp->worldPosition);
}

bool RTSAISystem::targetAlive ( ObjectManager* mgr ) {
	HealthComponent* otherHealthComp = mgr->getObjectComponent<HealthComponent>(currentRTSComp->attackTargetID, "HealthComponent");
	
	if (otherHealthComp == nullptr || otherHealthComp->health <= 0)
		return false;
	
	return true;
}

void RTSAISystem::faceTarget ( ObjectManager* mgr, int id ) {
	FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(id, "FaceDirectionComponent");
	
	if (faceComp != nullptr) {
		if (currentRTSComp->attackTargetID > -1) {
			vector3df dif = currentTransComp->worldPosition - attackTargetPosition(mgr);
			faceComp->targetYRot = radToDeg(atan2(dif.X, dif.Z));
		}
	}
}

bool RTSAISystem::checkTargetDifferentTeam ( ObjectManager* mgr, int target ) {
	TeamComponent* otherTeamComp = mgr->getObjectComponent<TeamComponent>(target, "TeamComponent");
	
	return otherTeamComp->teamID != currentTeamComp->teamID;
}

vector3df RTSAISystem::attackTargetPosition(ObjectManager* mgr) {
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(currentRTSComp->attackTargetID, "TransformComponent");
	
	if (otherTransComp != nullptr)
		return otherTransComp->worldPosition;
	
	return currentTransComp->worldPosition;
}

vector3df RTSAISystem::towerTargetPosition ( ObjectManager* mgr, int towerID ) {
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

int RTSAISystem::getNearestOnOtherTeam ( ObjectManager* mgr, int id ) {
	std::vector<int> objects = TeamComponent::getObjectsOnTeam((currentTeamComp->teamID + 1) % 2);
	
	double dist = -1;//currentRTSComp->rangeInSquares * mgr->worldManager->gridSize;
	// Multiply range by 1.5 to get a tentative line of sight
	//dist *= 1.5;
	//dist *= dist;
	
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
		
		//std::cout << i << std::endl;
		
		if (otherHealthComp->health <= 0) continue;
		
		double dx = otherTransComp->worldPosition.X - currentTransComp->worldPosition.X;
		double dz = otherTransComp->worldPosition.Z - currentTransComp->worldPosition.Z;
		double distsq = dx*dx + dz*dz;
		
		if (dist == -1 || distsq < dist) {
			dist = distsq;
			best = i;
		}
	}
	
	return best;
}

void RTSAISystem::stateDead ( ObjectManager* mgr, int id ) {
	mgr->detachComponent(id, "SteeringComponent");
	mgr->detachComponent(id, "RTSAIComponent");
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

void RTSAISystem::stateIdle ( ObjectManager* mgr, int id ) {
	
	if (currentSteerComp->velocity.getLength() > 0.01) {
		setAnimation("WALK", true);
		FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(id, "FaceDirectionComponent");
		if (faceComp != nullptr)
			faceComp->targetYRot = radToDeg(atan2(-currentSteerComp->velocity.X,-currentSteerComp->velocity.Z));
	} else {
		setAnimation("IDLE", true);
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
}

void RTSAISystem::stateMoveToAttack ( ObjectManager* mgr, int id ) {
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

	// Finished walking
	if (currentSteerComp->path.ended()) {
		currentRTSComp->attackTargetID = -1;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(IDLE);
		return;
	}
}

void RTSAISystem::stateAttacking ( ObjectManager* mgr, int id ) {
	setAnimation("SHOOT", false);
	
	// Reload
	if (animationComplete()) {
		currentRTSComp->stateStack.pop();
		return;
	}
}

void RTSAISystem::stateReloading ( ObjectManager* mgr, int id ) {
	setAnimation("RELOAD", false);
	
	// Return to aiming
	if (animationComplete()) {
		currentRTSComp->stateStack.pop();
		return;
	}
}

void RTSAISystem::stateWalking ( ObjectManager* mgr, int id ) {
	setAnimation("WALK", true);
	
	if (!currentRTSComp->pathSet) {
		setPath(mgr, id, currentRTSComp->terrainPoint);
		currentRTSComp->pathSet = true;
	}

	// Position reached
	if (currentSteerComp->path.ended()) {
		currentRTSComp->pathSet = false;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(IDLE);
		return;
	}
}

void RTSAISystem::stateAiming ( ObjectManager* mgr, int id ) {
	setAnimation("AIM", true);
	faceTarget(mgr, id);
	currentRTSComp->shootCounter--;
	
	currentRTSComp->pathSet = false;
	currentSteerComp->path.resetPath();

	// Attack
	if (currentRTSComp->shootCounter <= 0 && targetAlive(mgr)) {
		currentRTSComp->shootCounter = currentRTSComp->shootDelay;
		currentRTSComp->stateStack.push(RELOADING);
		currentRTSComp->stateStack.push(ATTACKING);
		
		HealthComponent* otherHealthComp = mgr->getObjectComponent<HealthComponent>(currentRTSComp->attackTargetID, "HealthComponent");
		otherHealthComp->health -= currentRTSComp->attackDamage;
		otherHealthComp->alpha = 2;
		
		currentRTSComp->shootSound->setPosition(currentTransComp->worldPosition.X, currentTransComp->worldPosition.Y, currentTransComp->worldPosition.Z);
		currentRTSComp->shootSound->setVolume(100);
		currentRTSComp->shootSound->setRelativeToListener(false);
		currentRTSComp->shootSound->setAttenuation(0.1f);
		currentRTSComp->shootSound->setPitch(0.9f + (1.0f/1000*(rand()%1000))*0.2f);
		currentRTSComp->shootSound->play();
		
		vector3df start = currentTransComp->worldPosition;
		vector3df end = attackTargetPosition(mgr);
		FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(id, "FaceDirectionComponent");
		
		((StatePlaying*)Game::game.currentState())->particleManager.spawnEffect(start, end, vector3df(0, faceComp->currentYRot, 0), currentRTSComp->effectType);
		
		return;
	}
	
	float rangeBonus = 1;
	float dist = (mgr->worldManager->gridSize * currentRTSComp->rangeInSquares * rangeBonus);
	float distSq = dist * dist;
	
	// Go back to idling
	if (!targetAlive(mgr) || distanceToObjectSq(mgr, currentRTSComp->attackTargetID) > distSq) {
		currentRTSComp->attackTargetID = -1;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(IDLE);
		currentRTSComp->stateStack.push(RELEASE_AIM);
	}
}

void RTSAISystem::stateReleaseAim ( ObjectManager* mgr, int id ) {
	setAnimation("REST", false);
	
	// Release aim
	if (animationComplete()) {
		currentRTSComp->stateStack.pop();
		return;
	}
}

void RTSAISystem::stateTakeAim ( ObjectManager* mgr, int id ) {
	setAnimation("TAKEAIM", false);
	faceTarget(mgr, id);
	
	// Take aim
	if (animationComplete()) {
		currentRTSComp->stateStack.pop();
		return;
	}
}






