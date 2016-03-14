#include <SteeringSystem.h>
#include <ObjectManager.h>
#include <list>
#include <SteeringComponent.h>
#include <TransformComponent.h>
#include <FaceDirectionComponent.h>
#include <RenderManager.h>
#include <AnimatorComponent.h>
#include <RenderComponent.h>
#include <Quadtree.h>
#include <DebugValues.h>
#include <Game.h>
#include <SelectableComponent.h>
#include <WorldManager.h>

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

void SteeringSystem::update ( float timestep ) {
	// Get the object manager
	ObjectManager* mgr = Game::game.getObjMgr();
	
	// Get objects with steering components
	std::list<int> objects = mgr->getObjectsWithComponent("SteeringComponent");
	
	int collisionCalls = 0;
	
	Quadtree collisionTree(0, Game::game.getObjMgr()->worldManager->getWorldBoundsF());
	buildQuadtree(objects, collisionTree);
	
	std::list<int> possibleCollisions;
	for (int i : objects) {
		// Get the steering component
		SteeringComponent* steerComp = mgr->getObjectComponent<SteeringComponent>(i, "SteeringComponent");
		
		if (!steerComp->enabled)
			continue;
			
		// Check the object has a transform
		TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
		
		if (transComp == nullptr)
			continue;
		
		if (!steerComp->path.ended()) {
			if (!steerComp->path.finalNode())
				seek(steerComp->path.getCurrentNode(), steerComp, transComp);
			else
				arrive(steerComp->path.getCurrentNode(), steerComp, transComp);
			
			vector3df myPos = transComp->worldPosition;
			vector3df nodePos = steerComp->path.getCurrentNode();
			myPos.Y = 0;
			nodePos.Y = 0;
			
			if (!steerComp->path.finalNode()) {
				if ((nodePos - myPos).getLengthSQ() < 50) {
					steerComp->path.nextNode();
				}
			} else {
				if ((nodePos - myPos).getLengthSQ() < 1) {
					steerComp->path.nextNode();
					steerComp->velocity *= 0;
				}
			}
		} else {
			steerComp->velocity *= 0.9;
		}
		
		if (steerComp->velocity.getLength() > 0.01) {
			// Get list of possible collisions from the quadtree
			possibleCollisions.clear();
			collisionTree.getObjects(possibleCollisions, i, transComp->worldPosition, steerComp->radius);
			
			// Avoid other units
			for (int j : possibleCollisions) {
				if (j == i) continue;
				
				collisionCalls++;
				
				SteeringComponent* otherSteerComp = mgr->getObjectComponent<SteeringComponent>(j, "SteeringComponent");
				
				if (!otherSteerComp->enabled)
					continue;
				
				// Check the object has a transform
				TransformComponent* otherTransComp = mgr->getObjectComponent<TransformComponent>(j, "TransformComponent");
				
				if (otherTransComp == nullptr)
					continue;
				
				if ((transComp->worldPosition - otherTransComp->worldPosition).getLengthSQ() < steerComp->radius*steerComp->radius + otherSteerComp->radius*otherSteerComp->radius) {
					if (steerComp->path.ended())
						avoid(otherTransComp->worldPosition, steerComp, transComp);
					
					//avoid(transComp->worldPosition, otherSteerComp, otherTransComp);

				}
			}
		}
		
		transComp->worldPosition += steerComp->velocity;
		transComp->worldPosition.Y = 
		((ITerrainSceneNode*)Game::game.getRendMgr()->getSceneManager()->getSceneNodeFromName("MainTerrain"))->getHeight(transComp->worldPosition.X, transComp->worldPosition.Z);
		
		// Animate the object appropriately (TEMPORARY)
		RenderComponent* rendComp = mgr->getObjectComponent<RenderComponent>(i, "RenderComponent");
		AnimatorComponent* animComp = mgr->getObjectComponent<AnimatorComponent>(i, "AnimatorComponent");
		
		if (rendComp != nullptr && animComp != nullptr) {
			if (steerComp->path.ended() && steerComp->velocity.getLength() < 0.03) {
				//animComp->setAnimation("IDLE", rendComp->sceneNode);
			} else {
				//animComp->setAnimation("WALK", rendComp->sceneNode);
			}
		}
		
		// Update facing direction if necessary
		FaceDirectionComponent* faceComp = mgr->getObjectComponent<FaceDirectionComponent>(i, "FaceDirectionComponent");
		
		if (faceComp != nullptr && (!steerComp->path.ended() && steerComp->velocity.getLength() > 0.01))
			faceComp->targetYRot = radToDeg(atan2(-steerComp->velocity.X,-steerComp->velocity.Z));
	}
	
	//std::cout << "Collision calls: " << collisionCalls << std::endl;
}


void SteeringSystem::draw ( float timestep ) {
	// Get the object manager
	ObjectManager* mgr = Game::game.getObjMgr();
	
	// Get objects with steering components
	std::list<int> objects = mgr->getObjectsWithComponent("SteeringComponent");
	
	if (DebugValues::DRAW_PATHS || DebugValues::DRAW_STEER) {
		for (int i : objects) {
			// Get the steering component
			SteeringComponent* steerComp = mgr->getObjectComponent<SteeringComponent>(i, "SteeringComponent");
			
			// Check the object has a transform
			TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
			
			if (transComp == nullptr)
				continue;
			SMaterial m;
			
			if (DebugValues::DRAW_STEER) {
				m.Lighting = false;
				m.Thickness = 2.0f;
				Game::game.getRendMgr()->getDriver()->setMaterial(m);
				Game::game.getRendMgr()->getDriver()->setTransform(video::ETS_WORLD, IdentityMatrix);
				if (!steerComp->path.ended())
					steerComp->prevTargetDir = (steerComp->path.getCurrentNode() - transComp->worldPosition).normalize() * 10;
				vector3df velToTarget = steerComp->prevTargetDir;
				vector3df vel = steerComp->velocity;
				vel = vel.normalize() * 10;
				
				Game::game.getRendMgr()->getDriver()->draw3DLine(transComp->worldPosition+vector3df(0,5,0), transComp->worldPosition+vector3df(0,5,0)+velToTarget, SColor(255,0,255,0));
				Game::game.getRendMgr()->getDriver()->draw3DLine(transComp->worldPosition+vector3df(0,4,0), transComp->worldPosition+vector3df(0,4,0)+vel, SColor(255,0,0,255));
			}
			
			
			if (DebugValues::DRAW_PATHS) {
				m.Lighting = false;
				m.Thickness = 2.0f;
				Game::game.getRendMgr()->getDriver()->setMaterial(m);
				Game::game.getRendMgr()->getDriver()->setTransform(video::ETS_WORLD, IdentityMatrix);
				
				vector3df node, prevNode;
				for (int i = 1; i < steerComp->path.getWaypoints().size(); i++) {
					node = steerComp->path.getWaypoints()[i];
					prevNode = steerComp->path.getWaypoints()[i-1];
					Game::game.getRendMgr()->getDriver()->draw3DLine(prevNode+vector3df(0,1,0), node+vector3df(0,1,0), SColor(80,255,255,255));	
					Game::game.getRendMgr()->getDriver()->draw3DLine(prevNode+vector3df(0,1,0), prevNode+vector3df(0,5,0), SColor(130,255,255,0));	
				}
				Game::game.getRendMgr()->getDriver()->draw3DLine(node+vector3df(0,1,0), node+vector3df(0,5,0), SColor(130,255,255,0));	
			}
		}
	}
	
	if (DebugValues::DRAW_QUADTREE) {
		SMaterial m;
		m.Lighting = false;
		m.Thickness = 2.0f;
		Game::game.getRendMgr()->getDriver()->setMaterial(m);
		Game::game.getRendMgr()->getDriver()->setTransform(video::ETS_WORLD, IdentityMatrix);
		
		Quadtree collisionTree(0, Game::game.getObjMgr()->worldManager->getWorldBoundsF());
		buildQuadtree(objects, collisionTree);
		
		collisionTree.draw();
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

void SteeringSystem::arrive ( vector3df arrivePos, SteeringComponent* steerComp, TransformComponent* transComp ) {
	vector3df* pos = &transComp->worldPosition;
	vector3df* vel = &steerComp->velocity;
	
	float lerpAmount = ((arrivePos - *pos).getLengthSQ() / 50);
	lerpAmount /= lerpAmount;
	lerpAmount /= lerpAmount;
	lerpAmount /= lerpAmount;
	
	vector3df velToTarget = (arrivePos - *pos).normalize() * steerComp->maxSpeed * lerpAmount;
	vector3df steerForce = velToTarget - *vel;
	
	if (steerForce.getLength() > steerComp->maxSpeed)
		steerForce = steerForce.normalize() * steerComp->maxSpeed;
	steerForce /= steerComp->mass;
	
	steerComp->velocity += steerForce;
	
	// Point velocity directly at the target
	steerComp->velocity = (arrivePos - *pos).normalize() * steerComp->velocity.getLength();
	
	if (steerComp->velocity.getLength() > steerComp->maxSpeed)
		steerComp->velocity = steerComp->velocity.normalize() * steerComp->maxSpeed;
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
	
	steerComp->velocity += steerForce*2;
}

/**
 * Build a quadtree of all units with steering components for efficient
 * collision checking
 */
void SteeringSystem::buildQuadtree ( std::list< int > objects, Quadtree& root ) {
	// Get the object manager
	ObjectManager* mgr = Game::game.getObjMgr();
	
	root.clear();

	for (int i : objects) {
		SteeringComponent* steerComp = mgr->getObjectComponent<SteeringComponent>(i, "SteeringComponent");
		
		// Check the object has a transform
		TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
		
		if (transComp == nullptr)
			continue;
		
		vector3df pos = transComp->worldPosition;
		float rad = steerComp->radius;
		
		root.insert(i, pos, rad);
	}
}



