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

#include <thread>

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
			if (selected) std::cout << "IDLE" << std::endl;
			break;
			
		case VILLAGER_WALKING:
			stateWalking(mgr, i);
			if (selected) std::cout << "WALKING" << std::endl;
			break;

		case VILLAGER_DEAD:
			stateDead(mgr, i);
			if (selected) std::cout << "DEAD" << std::endl;
			break;
			
		case VILLAGER_GATHER:
			stateGather(mgr, i);
			if (selected) std::cout << "GATHER" << std::endl;
			break;
			
		case VILLAGER_MOVE_TO_RESOURCE:
			stateMoveToResource(mgr, i);
			if (selected) std::cout << "MOVE_TO_RESOURCE" << std::endl;
			break;
		}
	}
	
	// Update flashing resources
	objects = mgr->getObjectsWithComponent("ResourceComponent");
	for (int i : objects) {
		ResourceComponent* resComp = mgr->getObjectComponent<ResourceComponent>(i, "ResourceComponent");
		
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
	
	rightMousePressed = false;
}

void RTSVillagerLogicSystem::draw ( float timestep ) {
	ObjectManager* mgr = Game::game.getObjMgr();
    // Get objects with steering components
	std::list<int> objects = mgr->getObjectsWithComponent("SteeringComponent");

	// Draw unit paths
	for (int i : objects) {
		// Get the steering component
		SteeringComponent* steerComp = mgr->getObjectComponent<SteeringComponent>(i, "SteeringComponent");
		
		// Check the object has a transform
		TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
		
		SelectableComponent* selectComp = mgr->getObjectComponent<SelectableComponent>(i, "SelectableComponent");
		
		if (transComp == nullptr || selectComp == nullptr || steerComp == nullptr)
			continue;
		SMaterial m;
		
		if (selectComp->selected && !steerComp->path.ended()) {
			m.Lighting = false;
			m.Thickness = 1.0f;
			Game::game.getRendMgr()->getDriver()->setMaterial(m);
			Game::game.getRendMgr()->getDriver()->setTransform(video::ETS_WORLD, IdentityMatrix);
			
			vector3df node, prevNode;
			for (int i = 1; i < steerComp->path.getWaypoints().size(); i++) {
				node = steerComp->path.getWaypoints()[i];
				prevNode = steerComp->path.getWaypoints()[i-1];
				Game::game.getRendMgr()->getDriver()->draw3DLine(prevNode+vector3df(0,2,0), node+vector3df(0,2,0), SColor(10,0,255,0));	
			}
			/*Game::game.getRendMgr()->getDriver()->draw3DLine(node+vector3df(0,1,0), node+vector3df(1,5,0), SColor(255,255,0,0));	
			Game::game.getRendMgr()->getDriver()->draw3DLine(node+vector3df(0,1,0), node+vector3df(-1,5,0), SColor(255,255,0,0));
			Game::game.getRendMgr()->getDriver()->draw3DLine(node+vector3df(0,1,0), node+vector3df(0,5,1), SColor(255,255,0,0));
			Game::game.getRendMgr()->getDriver()->draw3DLine(node+vector3df(0,1,0), node+vector3df(0,5,-1), SColor(255,255,0,0));*/
		}
	}
	
	// Draw flashing boxes under clicked resources
	objects = mgr->getObjectsWithComponent("ResourceComponent");
	for (int i : objects) {
		ResourceComponent* resComp = mgr->getObjectComponent<ResourceComponent>(i, "ResourceComponent");
		
		SMaterial m;
		
		ITerrainSceneNode* terrain = (ITerrainSceneNode*)Game::game.getRendMgr()->getSceneManager()->getSceneNodeFromName("MainTerrain");
		
		if (resComp->flashNum > 0 && resComp->flashOn) {
			m.Lighting = false;
			m.Thickness = 1.0f;
			Game::game.getRendMgr()->getDriver()->setMaterial(m);
			Game::game.getRendMgr()->getDriver()->setTransform(video::ETS_WORLD, IdentityMatrix);
			
			TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
			vector3df pos = transComp->worldPosition;
			float gridSize = mgr->worldManager->gridSize / 2;
			
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
	currentRTSComp->stateStack.push(VILLAGER_WAIT);
	
	std::thread t(&RTSVillagerLogicSystem::calcPathSynch, mgr, id, point);
	t.detach();
	
}

void RTSVillagerLogicSystem::calcPathSynch ( ObjectManager* mgr, int id, irr::core::vector3df point ) {
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

	mgr->getObjectComponent<RTSVillagerLogicComponent>(id, "RTSVillagerLogicComponent")->stateStack.pop();
	mgr->getObjectComponent<RTSVillagerLogicComponent>(id, "RTSVillagerLogicComponent")->pathSet = true;
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

vector3df RTSVillagerLogicSystem::resourceTargetPosition ( ObjectManager* mgr, int id ) {
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(currentRTSComp->resourceTargetID, "TransformComponent");
	ResourceComponent* resComp = mgr->getObjectComponent<ResourceComponent>(currentRTSComp->resourceTargetID, "ResourceComponent");
	
	if (otherTransComp == nullptr) return currentTransComp->worldPosition;

	float gridSize = mgr->worldManager->gridSize * 0.75f;
	vector3df mid = otherTransComp->worldPosition;
	vector3df nearest = currentTransComp->worldPosition;
	float distsq = -1;
	bool foundSlot = false;
	int slot = -1;
	
	float angle = 360.0f / resComp->maxGatherers;
	for (int i = 0; i < resComp->maxGatherers; i++) {
		// Construct vector in direction
		vector3df unit(cos(degToRad(i*angle)) * gridSize, 0, sin(degToRad(i*angle)) * gridSize);
		vector3df tempNearest = mid + unit;
		
		if (!mgr->worldManager->checkPassable(tempNearest)) continue;
		if (resComp->gatherSlots[i] != -1) continue;
		
		float tempDist = (currentTransComp->worldPosition-tempNearest).getLengthSQ();
		if (distsq == -1 || tempDist < distsq) {
			distsq = tempDist;
			nearest = tempNearest;
			foundSlot = true;
			slot = i;
		}
	}
	
	if (!foundSlot) {
		currentRTSComp->resourceSet = false;
	} else {
		resComp->gatherSlots[slot] = id;
		currentRTSComp->resourceSet = true;
	}
		
	return nearest;
}

bool RTSVillagerLogicSystem::canGatherHere ( ObjectManager* mgr, int id ) {
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(currentRTSComp->resourceTargetID, "TransformComponent");
	ResourceComponent* resComp = mgr->getObjectComponent<ResourceComponent>(currentRTSComp->resourceTargetID, "ResourceComponent");
	
	if (otherTransComp == nullptr) return false;

	float gridSize = mgr->worldManager->gridSize * 0.75f;
	vector3df mid = otherTransComp->worldPosition;
	vector3df nearest = currentTransComp->worldPosition;
	float distsq = -1;
	bool foundSlot = false;
	
	float angle = 360.0f / resComp->maxGatherers;
	for (int i = 0; i < resComp->maxGatherers; i++) {
		// Construct vector in direction
		vector3df unit(cos(degToRad(i*angle)) * gridSize, 0, sin(degToRad(i*angle)) * gridSize);
		vector3df tempNearest = mid + unit;
		
		if (!mgr->worldManager->checkPassable(tempNearest)) continue;
		if (resComp->gatherSlots[i] != -1) continue;
		
		float tempDist = (currentTransComp->worldPosition-tempNearest).getLengthSQ();
		if (distsq == -1 || tempDist < distsq) {
			distsq = tempDist;
			nearest = tempNearest;
			foundSlot = true;
		}
	}
	
	if (!foundSlot)
		((StatePlaying*)Game::game.currentState())->message(SHOW_MESSAGE_BAD, "There is not enough space for this unit to gather here");
	
	return foundSlot;
}

bool RTSVillagerLogicSystem::isResource ( ObjectManager* mgr, int checkid ) {
	return mgr->objectHasComponent(checkid, "ResourceComponent");
}

void RTSVillagerLogicSystem::freeResource ( ObjectManager* mgr, int id ) {
	if (currentRTSComp->resourceTargetID < 0) return;
	
	ResourceComponent* resComp = mgr->getObjectComponent<ResourceComponent>(currentRTSComp->resourceTargetID, "ResourceComponent");
	
	for (int i = 0; i < resComp->maxGatherers; i++) {
		if (resComp->gatherSlots[i] == id)
			resComp->gatherSlots[i] = -1;
	}
}

void RTSVillagerLogicSystem::stateDead ( ObjectManager* mgr, int id ) {
	mgr->detachComponent(id, "SteeringComponent");
	mgr->detachComponent(id, "RTSVillagerLogicComponent");
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

void RTSVillagerLogicSystem::stateIdle ( ObjectManager* mgr, int id ) {
	if (currentSteerComp->velocity.getLength() > 0.01) {
		setAnimation("WALK", true);
		FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(id, "FaceDirectionComponent");
		if (faceComp != nullptr)
			faceComp->targetYRot = radToDeg(atan2(-currentSteerComp->velocity.X,-currentSteerComp->velocity.Z));
	} else {
		setAnimation("IDLE", true);
	}
	
	// Start walking to resource target
	if (selected() && clickedObject >= 0 && isResource(mgr, clickedObject)) {
		freeResource(mgr, id);
		
		currentRTSComp->resourceTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		currentRTSComp->resType = clickedObjType;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_MOVE_TO_RESOURCE);
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		freeResource(mgr, id);
		
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
	
	currentSteerComp->enabled = false;
	faceTarget(mgr, id);
	
	currentRTSComp->gatherTicker--;
	if (currentRTSComp->gatherTicker < 0) {
		currentRTSComp->gatherTicker = currentRTSComp->gatherDelay;
		
		if (currentRTSComp->resType == GOLD)
			((StatePlaying*)Game::game.currentState())->getResourceCache()->gatherGold();
		else if (currentRTSComp->resType == STONE)
			((StatePlaying*)Game::game.currentState())->getResourceCache()->gatherStone();
		else if (currentRTSComp->resType == WOOD)
			((StatePlaying*)Game::game.currentState())->getResourceCache()->gatherWood();
	}
	
	// Start walking to resource
	if (selected() && clickedObject >= 0 && clickedObject != currentRTSComp->resourceTargetID && isResource(mgr, clickedObject)) {
		freeResource(mgr, id);
		
		currentRTSComp->resourceTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		currentRTSComp->resType = clickedObjType;
		
		currentSteerComp->enabled = true;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_MOVE_TO_RESOURCE);
		return;
	}
	
	if (selected() && clickedObject >= 0 && clickedObject == currentRTSComp->resourceTargetID) {
		ResourceComponent* resComp = mgr->getObjectComponent<ResourceComponent>(currentRTSComp->resourceTargetID, "ResourceComponent");
		resComp->flashNum = 8;
		resComp->flashTimer = 5;
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		freeResource(mgr, id);
		
		currentRTSComp->resourceTargetID = -1;
		currentRTSComp->pathSet = false;
		
		currentSteerComp->enabled = true;
		
		currentRTSComp->terrainPoint = terrainPoint;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_WALKING);
		return;
	}
	
	int soundFrame = currentRTSComp->resType == WOOD ? currentRTSComp->chopFrame : currentRTSComp->mineFrame;
	
	// Gather again
	if (currentRendComp->sceneNode->getFrameNr() > currentRendComp->sceneNode->getStartFrame() + soundFrame) {
		if (!currentRTSComp->soundPlayed) {
			sf::Sound* snd;
			if (currentRTSComp->resType == WOOD) {
				snd = Game::game.resources.loadSound("axesound.ogg");
			} else {
				snd = Game::game.resources.loadSound("picksound.ogg");
			}
			
			snd->setRelativeToListener(false);
			snd->setVolume(100);
			snd->setAttenuation(0.5f);
			snd->setPosition(currentTransComp->worldPosition.X, currentTransComp->worldPosition.Y, currentTransComp->worldPosition.Z);
			snd->setPitch(0.95f + (1.0f/1000*(rand()%1000))*0.15f);
			snd->play();
			
			currentRTSComp->soundPlayed = true;
		}
	} else {
		currentRTSComp->soundPlayed = false;
	}
}

void RTSVillagerLogicSystem::stateWalking ( ObjectManager* mgr, int id ) {
	setAnimation("WALK", true);
	
	if (!currentRTSComp->pathSet) {
		setPath(mgr, id, currentRTSComp->terrainPoint);
		currentRTSComp->pathSet = true;
	}

	// Start walking to resource target
	if (selected() && clickedObject >= 0 && isResource(mgr, clickedObject)) {
		freeResource(mgr, id);
		
		currentRTSComp->resourceTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		currentRTSComp->resType = clickedObjType;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_MOVE_TO_RESOURCE);
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		freeResource(mgr, id);
		
		currentRTSComp->resourceTargetID = -1;
		currentRTSComp->pathSet = false;
		
		currentRTSComp->terrainPoint = terrainPoint;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_WALKING);
		return;
	}
	
	// Position reached
	if (currentSteerComp->path.ended()) {
		currentSteerComp->path.resetPath();
		currentRTSComp->pathSet = false;
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_IDLE);
		return;
	}
}

void RTSVillagerLogicSystem::stateMoveToResource ( ObjectManager* mgr, int id ) {
	setAnimation("WALK", true);
	
	if (!currentRTSComp->pathSet) {
		if (!canGatherHere(mgr, id)) {
			freeResource(mgr, id);
			currentRTSComp->resourceTargetID = -1;
			currentRTSComp->pathSet = false;
			currentSteerComp->path.resetPath();
			
			currentRTSComp->stateStack.pop();
			currentRTSComp->stateStack.push(VILLAGER_IDLE);
			return;
		}
		
		setPath(mgr, id, resourceTargetPosition(mgr, id));
		if (!currentRTSComp->resourceSet) {
			currentRTSComp->stateStack.pop();
			return;
		} 
		
		ResourceComponent* resComp = mgr->getObjectComponent<ResourceComponent>(currentRTSComp->resourceTargetID, "ResourceComponent");
		resComp->flashNum = 8;
		resComp->flashTimer = 5;
		
		currentRTSComp->pathSet = true;
		
		if (currentRTSComp->resType == WOOD) {
			Game::game.resources.loadSound("selectwood.ogg")->play();
		} else {
			Game::game.resources.loadSound("selectstone.ogg")->play();
		}
	}

	// Start walking to resource
	if (selected() && clickedObject >= 0 && clickedObject != currentRTSComp->resourceTargetID && isResource(mgr, clickedObject)) {
		freeResource(mgr, id);
		
		currentRTSComp->resourceTargetID = clickedObject;
		currentRTSComp->pathSet = false;
		currentRTSComp->resType = clickedObjType;
		
		currentRTSComp->stateStack.pop();
		currentRTSComp->stateStack.push(VILLAGER_MOVE_TO_RESOURCE);
		return;
	}
	
	if (selected() && clickedObject >= 0 && clickedObject == currentRTSComp->resourceTargetID) {
		ResourceComponent* resComp = mgr->getObjectComponent<ResourceComponent>(currentRTSComp->resourceTargetID, "ResourceComponent");
		resComp->flashNum = 8;
		resComp->flashTimer = 5;
		return;
	}
	
	// Start walking to position
	if (selected() && rightMousePressed) {
		freeResource(mgr, id);
		
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





