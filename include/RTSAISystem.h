#ifndef RTSAISYSTEM_H
#define RTSAISYSTEM_H

#include <ComponentSystem.h>
#include <ObjectManager.h>
#include <irrlicht/irrlicht.h>

#include <RTSLogicComponent.h>
#include <SelectableComponent.h>
#include <PathMovementComponent.h>
#include <SteeringComponent.h>
#include <TransformComponent.h>
#include <AnimatorComponent.h>
#include <HealthComponent.h>
#include <RenderComponent.h>
#include <Quadtree.h>
#include <TeamComponent.h>
#include <GroupPathMover.h>
#include <RTSAIComponent.h>

/**
 * System to update a selected object's path when the map is right-clicked
 */
class RTSAISystem : public ComponentSystem {
private:
	GroupPathMover pathMover;
	
	void setPath(ObjectManager* mgr, int id, irr::core::vector3df point);
	static void calcPathSynch(ObjectManager* mgr, int id, irr::core::vector3df point);
	
	void stateIdle(ObjectManager* mgr, int id);
	void stateWalking(ObjectManager* mgr, int id);
	void stateReloading(ObjectManager* mgr, int id);
	void stateAttacking(ObjectManager* mgr, int id);
	void stateMoveToAttack(ObjectManager* mgr, int id);
	void stateDead(ObjectManager* mgr, int id);
	void stateAiming(ObjectManager* mgr, int id);
	void stateTakeAim(ObjectManager* mgr, int id);
	void stateReleaseAim(ObjectManager* mgr, int id);
	
	void faceTarget( ObjectManager* mgr, int id);
	irr::core::vector3df attackTargetPosition(ObjectManager* mgr);
	irr::core::vector3df towerTargetPosition(ObjectManager* mgr, int towerID);
	void setAnimation(std::string animation, bool loop);
	bool animationComplete();
	bool selected();
	bool targetAlive(ObjectManager* mgr);
	float distanceToObjectSq(ObjectManager* mgr, int otherID);
	bool checkTargetDifferentTeam(ObjectManager* mgr, int target);
	int getNearestOnOtherTeam(ObjectManager* mgr, int id);
	
	TransformComponent* currentTransComp;
	RTSAIComponent* currentRTSComp;
	AnimatorComponent* currentAnimComp;
	SteeringComponent* currentSteerComp;
	HealthComponent* currentHealthComp;
	PathMovementComponent* currentPathComp;
	RenderComponent* currentRendComp;
	TeamComponent* currentTeamComp;
	
	irr::core::vector3df terrainPoint;
	irr::core::vector3df objectPoint;
public:
	RTSAISystem() : ComponentSystem("RTSAISystem") {}	
	
	void update(float timestep);
	
	void draw(float timestep);
};


#endif