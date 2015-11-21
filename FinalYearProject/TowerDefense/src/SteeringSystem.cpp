#include <SteeringSystem.h>
#include <ObjectManager.h>
#include <list>
#include <SteeringComponent.h>
#include <TransformComponent.h>
#include <FaceDirectionComponent.h>
#include <RenderManager.h>
#include <AnimatorComponent.h>
#include <RenderComponent.h>

using namespace irr;
using namespace core;
using namespace video;

void SteeringSystem::update ( float timestep ) {
	// Get the object manager
	ObjectManager* mgr = &ObjectManager::manager;
	
	// Get objects with steering components
	std::list<int> objects = mgr->getObjectsWithComponent("SteeringComponent");
	
	for (int i : objects) {
		// Get the steering component
		SteeringComponent* steerComp = mgr->getObjectComponent<SteeringComponent>(i, "SteeringComponent");
		
		// Check the object has a transform
		TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
		
		if (transComp == nullptr)
			continue;
		
		if (!steerComp->path.ended()) {
			
			seek(steerComp->path.getCurrentNode(), steerComp, transComp);
			
			if ((steerComp->path.getCurrentNode() - transComp->worldPosition).getLengthSQ() < 50) 
				steerComp->path.nextNode();
				
		} else {
			steerComp->velocity *= 0.95;
		}
		
		// Avoid other units
		for (int j : objects) {
			if (j == i) continue;
			
			SteeringComponent* otherSteerComp = mgr->getObjectComponent<SteeringComponent>(j, "SteeringComponent");
			
			// Check the object has a transform
			TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(j, "TransformComponent");
			
			if (otherTransComp == nullptr)
				continue;
			
			if ((transComp->worldPosition - otherTransComp->worldPosition).getLengthSQ() < steerComp->radius*steerComp->radius + otherSteerComp->radius*otherSteerComp->radius)
				avoid(otherTransComp->worldPosition, steerComp, transComp);
		}
		
		transComp->worldPosition += steerComp->velocity;
		transComp->worldPosition.Y = 0;
		
		// Animate the object appropriately (TEMPORARY)
		RenderComponent* rendComp = mgr->getObjectComponent<RenderComponent>(i, "RenderComponent");
		AnimatorComponent* animComp = mgr->getObjectComponent<AnimatorComponent>(i, "AnimatorComponent");
		
		if (rendComp != nullptr && animComp != nullptr) {
			if (steerComp->path.ended() && steerComp->velocity.getLength() < 0.01) {
				animComp->setAnimation("IDLE", rendComp->sceneNode);
			} else {
				animComp->setAnimation("WALK", rendComp->sceneNode);
			}
		}
		
		// Update facing direction if necessary
		FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(i, "FaceDirectionComponent");
		
		if (faceComp != nullptr && (!steerComp->path.ended() || steerComp->velocity.getLength() > 0.01))
			faceComp->targetYRot = radToDeg(atan2(-steerComp->velocity.X,-steerComp->velocity.Z));
	}
}


void SteeringSystem::draw ( float timestep ) {
	if (!RenderManager::DEBUG_GRAPHICS) return;
	
	// Get the object manager
	ObjectManager* mgr = &ObjectManager::manager;
	
	// Get objects with steering components
	std::list<int> objects = mgr->getObjectsWithComponent("SteeringComponent");
	
	for (int i : objects) {
		// Get the steering component
		SteeringComponent* steerComp = mgr->getObjectComponent<SteeringComponent>(i, "SteeringComponent");
		
		// Check the object has a transform
		TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
		
		if (transComp == nullptr)
			continue;

		SMaterial m;
		m.Lighting = false;
		m.Thickness = 2.0f;
		RenderManager::renderManager.getDriver()->setMaterial(m);
		RenderManager::renderManager.getDriver()->setTransform(video::ETS_WORLD, IdentityMatrix);
		vector3df velToTarget = (steerComp->path.getCurrentNode() - transComp->worldPosition).normalize() * 10;
		vector3df vel = steerComp->velocity;
		vel = vel.normalize() * 10;
		
		RenderManager::renderManager.getDriver()->draw3DLine(transComp->worldPosition+vector3df(0,5,0), transComp->worldPosition+vector3df(0,5,0)+velToTarget, SColor(255,0,255,0));
		RenderManager::renderManager.getDriver()->draw3DLine(transComp->worldPosition+vector3df(0,4,0), transComp->worldPosition+vector3df(0,4,0)+vel, SColor(255,0,0,255));
		
		m.Lighting = false;
		m.Thickness = 1.0f;
		RenderManager::renderManager.getDriver()->setMaterial(m);
		RenderManager::renderManager.getDriver()->setTransform(video::ETS_WORLD, IdentityMatrix);
		
		vector3df node, prevNode;
		for (int i = 1; i < steerComp->path.getWaypoints().size(); i++) {
			node = steerComp->path.getWaypoints()[i];
			prevNode = steerComp->path.getWaypoints()[i-1];
			RenderManager::renderManager.getDriver()->draw3DLine(prevNode+vector3df(0,1,0), node+vector3df(0,1,0), SColor(130,255,0,0));			
		}
	}
}


/**
 * Use basic steering behaviours to seek a target position
 */
void SteeringSystem::seek ( vector3df targetPos, SteeringComponent* steerComp, TransformComponent* transComp ) {
	vector3df* pos = &transComp->worldPosition;
	vector3df* vel = &steerComp->velocity;
	
	vector3df velToTarget = (targetPos - *pos).normalize() * steerComp->maxSpeed;
	vector3df steerForce = velToTarget - *vel;
	
	if (steerForce.getLength() > steerComp->maxSpeed)
		steerForce = steerForce.normalize() * steerComp->maxSpeed;
	steerForce /= steerComp->mass;
	
	steerComp->velocity += steerForce;
}

/**
 * Use basic steering behaviours to avoid a target position
 */
void SteeringSystem::avoid ( vector3df avoidPos, SteeringComponent* steerComp, TransformComponent* transComp ) {
	vector3df* pos = &transComp->worldPosition;
	vector3df* vel = &steerComp->velocity;
	
	vector3df velToTarget = (*pos - avoidPos).normalize() * steerComp->maxSpeed;
	vector3df steerForce = velToTarget - *vel;
	
	if (steerForce.getLength() > steerComp->maxSpeed)
		steerForce = steerForce.normalize() * steerComp->maxSpeed;
	steerForce /= steerComp->mass;
	
	steerComp->velocity += steerForce;
}




