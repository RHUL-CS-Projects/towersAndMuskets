#include <RTSLogicComponent.h>
#include <RTSLogicSystem.h>
#include <EventReceiver.h>
#include <ObjectManager.h>
#include <list>
#include <RenderManager.h>
#include <SelectableComponent.h>
#include <PathMovementComponent.h>
#include <SteeringComponent.h>
#include <TransformComponent.h>
#include <PathFinder.h>
#include <AnimatorComponent.h>
#include <RenderComponent.h>
#include <FaceDirectionComponent.h>
#include <HealthComponent.h>

#include <irrlicht/irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace io;
using namespace scene;

vector3df terrainPoint;
int clickedObject = -1;
vector3df objectPoint;

void RTSLogicSystem::update ( float timestep ) {
	clickedObject = -1;
	
	// Check if the right mouse button was clicked on the terrain or a game object
	if (EventReceiver::getMouseState()->rightPressed && !rightMouseDown) {
		rightMouseDown = true;
		rightMousePressed = true;
		
		// Calculate click point on terrain
		ISceneCollisionManager* colmgr = RenderManager::renderManager.getSceneManager()->getSceneCollisionManager();
		line3df ray = colmgr->getRayFromScreenCoordinates(EventReceiver::getMouseState()->position);
		
		triangle3df triangle;
		ISceneNode* node;
		
		// Check if the user clicked on terrain or object
		colmgr->getCollisionPoint(ray, RenderManager::renderManager.getSceneManager()->getSceneNodeFromName("MainTerrain")->getTriangleSelector(), terrainPoint, triangle, node);
		
		ObjectManager* mgr = &ObjectManager::manager;
		std::list<int> objects = mgr->getObjectsWithComponent("RenderComponent");
		
		int hitID = -1;
		if ((hitID = colmgr->getSceneNodeFromRayBB(ray)->getID()) > -1) {	
			if (mgr->getObjectComponent<TransformComponent>(hitID, "TransformComponent") != nullptr) {	
				clickedObject = hitID;
				objectPoint = mgr->getObjectComponent<TransformComponent>(hitID, "TransformComponent")->worldPosition;
			}
		}
	} else {
		if (!EventReceiver::getMouseState()->rightPressed)
			rightMouseDown = false;
	}
	
    // Get the object object manager
	ObjectManager* mgr = &ObjectManager::manager;
	
	// Get all objects with an RTSMovementComponent
	std::list<int> objects = mgr->getObjectsWithComponent("RTSLogicComponent");
	
	// Iterate over objects
	for (int i : objects) {
		RTSLogicComponent* rtsComp = mgr->getObjectComponent<RTSLogicComponent>(i, "RTSLogicComponent");
		SteeringComponent* steerComp = mgr->getObjectComponent<SteeringComponent>(i, "SteeringComponent");
		HealthComponent* healthComp = mgr->getObjectComponent<HealthComponent>(i, "HealthComponent");
		
		if (healthComp->health <= 0)
			rtsComp->currentState = DEAD;
		
		SelectableComponent* selectComp = mgr->getObjectComponent<SelectableComponent>(i, "SelectableComponent");
		bool selected = true;
		
		if (selectComp == nullptr || selectComp->selected == false)
			selected = false;
		
		// Perform behaviours of current state
		switch (rtsComp->currentState) {
		case IDLE:
			stateIdle(mgr, i, rtsComp, steerComp, selected);
			break;
			
		case WALKING:
			stateWalking(mgr, i, rtsComp, steerComp, selected);
			break;
			
		case MOVE_TO_ATTACK:
			stateMoveToAttack(mgr, i, rtsComp, steerComp, selected);
			break;
			
		case ATTACKING:
			stateAttacking(mgr, i, rtsComp, steerComp, selected);
			break;
			
		case RELOADING:
			stateReloading(mgr, i, rtsComp, steerComp, selected);
			break;
			
		case DEAD:
			stateDead(mgr, i, rtsComp, steerComp, selected);
			break;
		}
	}
	
	rightMousePressed = false;
}

void RTSLogicSystem::draw ( float timestep ) {
    
}

void RTSLogicSystem::setPath ( ObjectManager* mgr, int id, vector3df point ) {
	// Get only selected selectable objects
	/*SelectableComponent* selectComp = mgr->getObjectComponent<SelectableComponent>(id, "SelectableComponent");
	
	if (selectComp == nullptr || selectComp->selected == false)
		return;*/
	
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

void RTSLogicSystem::stateAttacking ( ObjectManager* mgr, int id, RTSLogicComponent* rtsComp, SteeringComponent* steerComp, bool selected ) {
	AnimatorComponent* animComp = mgr->getObjectComponent<AnimatorComponent>(id, "AnimatorComponent");
	RenderComponent* rendComp = mgr->getObjectComponent<RenderComponent>(id, "RenderComponent");
	TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(id, "TransformComponent");
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(rtsComp->attackTargetID, "TransformComponent");
	HealthComponent* healthComp = mgr->getObjectComponent<HealthComponent>(rtsComp->attackTargetID, "HealthComponent");
	
	if (rtsComp->shootCounter > 0)
			rtsComp->shootCounter--;
	
	if (animComp != nullptr && rendComp != nullptr) {
		rendComp->sceneNode->setLoopMode(false);
		
		if (animComp->currentAnimation != "TAKEAIM" && animComp->currentAnimation != "SHOOT" && animComp->currentAnimation != "AIM") {
			if (animComp->currentAnimation != "RELOAD")
				animComp->setAnimation("TAKEAIM", rendComp->sceneNode);
			else
				animComp->setAnimation("AIM", rendComp->sceneNode);
		}
		
		if (rtsComp->shootCounter <= 0 && (animComp->currentAnimation == "TAKEAIM" || animComp->currentAnimation == "AIM") && 
			rendComp->sceneNode->getFrameNr() >= rendComp->sceneNode->getEndFrame()) {
			animComp->setAnimation("SHOOT", rendComp->sceneNode);
		}
		
		if (animComp->currentAnimation == "SHOOT") {
			if (rtsComp->shootCounter <= 0 && floor(rendComp->sceneNode->getFrameNr()) == rendComp->sceneNode->getStartFrame() + rtsComp->attackActionFrame) {
				rtsComp->shootSound.setPosition(transComp->worldPosition.X, transComp->worldPosition.Y, transComp->worldPosition.Z);
				rtsComp->shootSound.play();
				rtsComp->shootCounter = rand() % rtsComp->shootDelay;

				if (healthComp != nullptr)
					healthComp->health -= 5;
			}
			
			if (rendComp->sceneNode->getFrameNr() >= rendComp->sceneNode->getEndFrame()) {
				rtsComp->currentState = RELOADING;
			}
		}
	}
	
	// Update facing direction if necessary
	FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(id, "FaceDirectionComponent");
	
	if (faceComp != nullptr)
		faceComp->targetYRot = radToDeg(atan2(transComp->worldPosition.X - otherTransComp->worldPosition.X, transComp->worldPosition.Z - otherTransComp->worldPosition.Z));
	
	if (rightMousePressed && selected && animComp != nullptr && (animComp->currentAnimation == "TAKEAIM" || animComp->currentAnimation == "AIM")) {
		if (clickedObject > -1) {
			RTSLogicComponent* otherRTSComp = mgr->getObjectComponent<RTSLogicComponent>(clickedObject, "RTSLogicComponent");	
			if (rtsComp->teamID == -1 || otherRTSComp == nullptr || rtsComp->teamID != otherRTSComp->teamID) {
				rtsComp->attackTargetID = clickedObject;
				rendComp->sceneNode->setLoopMode(true);
				setPath(mgr, id, objectPoint);
				rtsComp->currentState = MOVE_TO_ATTACK;
			}
		} else {
			rtsComp->attackTargetID = -1;
			rendComp->sceneNode->setLoopMode(true);
			rtsComp->currentState = WALKING;
			setPath(mgr, id, terrainPoint);
		}
	}
}

void RTSLogicSystem::stateDead ( ObjectManager* mgr, int id, RTSLogicComponent* rtsComp, SteeringComponent* steerComp, bool selected ) {
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

void RTSLogicSystem::stateIdle ( ObjectManager* mgr, int id, RTSLogicComponent* rtsComp, SteeringComponent* steerComp, bool selected ) {
	// Started walking
	if (rightMousePressed && selected) {
		if (clickedObject > -1) {
			RTSLogicComponent* otherRTSComp = mgr->getObjectComponent<RTSLogicComponent>(clickedObject, "RTSLogicComponent");	
			if (rtsComp->teamID == -1 || otherRTSComp == nullptr || rtsComp->teamID != otherRTSComp->teamID) {
				rtsComp->attackTargetID = clickedObject;
				setPath(mgr, id, objectPoint);
				rtsComp->currentState = MOVE_TO_ATTACK;
			}
		} else {
			rtsComp->currentState = WALKING;
			setPath(mgr, id, terrainPoint);
		}
	} else {
		if (steerComp != nullptr && steerComp->path.ended())
			rtsComp->currentState = IDLE;
	}
	
	if (rtsComp->attackTargetID == -1) {
		// Get all objects with an RTSMovementComponent
		std::list<int> objects = mgr->getObjectsWithComponent("RTSLogicComponent");
		TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(id, "TransformComponent");
		
		for (int i : objects) {
			if (i == id) continue;
			TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
			
			if (transComp->worldPosition.getDistanceFromSQ(otherTransComp->worldPosition) < 19600) {
				RTSLogicComponent* otherRTSComp = mgr->getObjectComponent<RTSLogicComponent>(i, "RTSLogicComponent");
				
				if (rtsComp->teamID == -1 || otherRTSComp == nullptr || rtsComp->teamID != otherRTSComp->teamID) {
					rtsComp->attackTargetID = i;
					setPath(mgr, id, otherTransComp->worldPosition);
					rtsComp->currentState = MOVE_TO_ATTACK;
					
					break;
				}
			}
		}
	}
	
	RenderComponent* rendComp = mgr->getObjectComponent<RenderComponent>(id, "RenderComponent");
	AnimatorComponent* animComp = mgr->getObjectComponent<AnimatorComponent>(id, "AnimatorComponent");

	if (rendComp != nullptr && animComp != nullptr) {
		rendComp->sceneNode->setLoopMode(true);
		if (steerComp->path.ended() && steerComp->velocity.getLength() < 0.03)
			animComp->setAnimation("IDLE", rendComp->sceneNode);
	}
}

void RTSLogicSystem::stateMoveToAttack ( ObjectManager* mgr, int id, RTSLogicComponent* rtsComp, SteeringComponent* steerComp, bool selected ) {
	TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(id, "TransformComponent");
	
	if (rtsComp->attackTargetID > -1) {
		TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(rtsComp->attackTargetID, "TransformComponent");
		if (transComp->worldPosition.getDistanceFromSQ(otherTransComp->worldPosition) < 10000) {
			rtsComp->shootCounter = rand() % rtsComp->shootDelay;
			rtsComp->currentState = ATTACKING;
			steerComp->path.resetPath();
		}
	}
	
	if (rightMousePressed && selected) {
		if (clickedObject > -1) {
			RTSLogicComponent* otherRTSComp = mgr->getObjectComponent<RTSLogicComponent>(clickedObject, "RTSLogicComponent");	
			if (rtsComp->teamID == -1 || otherRTSComp == nullptr || rtsComp->teamID != otherRTSComp->teamID) {
				rtsComp->attackTargetID = clickedObject;
				setPath(mgr, id, objectPoint);
				rtsComp->currentState = MOVE_TO_ATTACK;
			}
		} else {
			rtsComp->currentState = WALKING;
			setPath(mgr, id, terrainPoint);
		}
	}
	
	RenderComponent* rendComp = mgr->getObjectComponent<RenderComponent>(id, "RenderComponent");
	AnimatorComponent* animComp = mgr->getObjectComponent<AnimatorComponent>(id, "AnimatorComponent");
	
	if (rendComp != nullptr && animComp != nullptr) {
		if (!steerComp->path.ended())
			animComp->setAnimation("WALK", rendComp->sceneNode);
	}
}

void RTSLogicSystem::stateReloading ( ObjectManager* mgr, int id, RTSLogicComponent* rtsComp, SteeringComponent* steerComp, bool selected ) {
	AnimatorComponent* animComp = mgr->getObjectComponent<AnimatorComponent>(id, "AnimatorComponent");
	RenderComponent* rendComp = mgr->getObjectComponent<RenderComponent>(id, "RenderComponent");
	HealthComponent* healthComp = mgr->getObjectComponent<HealthComponent>(rtsComp->attackTargetID, "HealthComponent");
	
	if (animComp != nullptr && rendComp != nullptr) {
		rendComp->sceneNode->setLoopMode(false);
		
		if (animComp->currentAnimation == "REST" && rendComp->sceneNode->getFrameNr() >= rendComp->sceneNode->getEndFrame()) {
			rtsComp->currentState = IDLE;
			rtsComp->attackTargetID = -1;
			return;
		}
		
		if (animComp->currentAnimation != "RELOAD" && animComp->currentAnimation != "REST")
			animComp->setAnimation("RELOAD", rendComp->sceneNode);
		
		if (animComp->currentAnimation == "RELOAD" && rendComp->sceneNode->getFrameNr() >= rendComp->sceneNode->getEndFrame()) {
			rtsComp->shootCounter = rand() % rtsComp->shootDelay;
			
			if (healthComp->health <= 0) {
				animComp->setAnimation("REST", rendComp->sceneNode);
			} else {
				rtsComp->currentState = ATTACKING;
			}
		}
	}
	
	// Update facing direction if necessary
	FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(id, "FaceDirectionComponent");
	TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(rtsComp->attackTargetID, "TransformComponent");
	TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(id, "TransformComponent");
	
	if (faceComp != nullptr)
		faceComp->targetYRot = radToDeg(atan2(transComp->worldPosition.X - otherTransComp->worldPosition.X, transComp->worldPosition.Z - otherTransComp->worldPosition.Z));
	
	if (rightMousePressed && selected) {
		if (clickedObject > -1) {
			RTSLogicComponent* otherRTSComp = mgr->getObjectComponent<RTSLogicComponent>(clickedObject, "RTSLogicComponent");	
			if (clickedObject != rtsComp->attackTargetID && (rtsComp->teamID == -1 || otherRTSComp == nullptr || rtsComp->teamID != otherRTSComp->teamID)) {
				rtsComp->attackTargetID = clickedObject;
				setPath(mgr, id, objectPoint);
				rtsComp->currentState = MOVE_TO_ATTACK;
			}
		} else {
			rtsComp->currentState = WALKING;
			rendComp->sceneNode->setLoopMode(true);
			setPath(mgr, id, terrainPoint);
		}
	}
}

void RTSLogicSystem::stateWalking ( ObjectManager* mgr, int id, RTSLogicComponent* rtsComp, SteeringComponent* steerComp, bool selected ) {
	if (rightMousePressed && selected) {
		if (clickedObject > -1) {
			RTSLogicComponent* otherRTSComp = mgr->getObjectComponent<RTSLogicComponent>(clickedObject, "RTSLogicComponent");	
			if (rtsComp->teamID == -1 || otherRTSComp == nullptr || rtsComp->teamID != otherRTSComp->teamID) {
				rtsComp->attackTargetID = clickedObject;
				setPath(mgr, id, objectPoint);
				rtsComp->currentState = MOVE_TO_ATTACK;
			}
		} else {
			setPath(mgr, id, terrainPoint);
		}
	} else {
		if (steerComp != nullptr && steerComp->path.ended())
			rtsComp->currentState = IDLE;
	}
	
	RenderComponent* rendComp = mgr->getObjectComponent<RenderComponent>(id, "RenderComponent");
	AnimatorComponent* animComp = mgr->getObjectComponent<AnimatorComponent>(id, "AnimatorComponent");
	
	if (rendComp != nullptr && animComp != nullptr) {
		if (!steerComp->path.ended())
			animComp->setAnimation("WALK", rendComp->sceneNode);
	}
}
