#ifndef RTSMOVEMENTSYSTEM_H
#define RTSMOVEMENTSYSTEM_H

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

/**
 * System to update a selected object's path when the map is right-clicked
 */
class RTSLogicSystem : public ComponentSystem {
private:
	bool rightMouseDown = false;
	bool rightMousePressed = false;
	
	void setPath(ObjectManager* mgr, int id, irr::core::vector3df point);
	static void calcPathSynch(ObjectManager* mgr, int id, irr::core::vector3df point);
	void updateClickPoints();
	
	void stateIdle(ObjectManager* mgr, int id);
	void stateWalking(ObjectManager* mgr, int id);
	void stateReloading(ObjectManager* mgr, int id);
	void stateAttacking(ObjectManager* mgr, int id);
	void stateMoveToAttack(ObjectManager* mgr, int id);
	void stateDead(ObjectManager* mgr, int id);
	void stateMoveToTower(ObjectManager* mgr, int id);
	void stateClimbUp(ObjectManager* mgr, int id);
	void stateClimbDown(ObjectManager* mgr, int id);
	void stateGarrissoned(ObjectManager* mgr, int id);
	void stateAiming(ObjectManager* mgr, int id);
	void stateTakeAim(ObjectManager* mgr, int id);
	void stateReleaseAim(ObjectManager* mgr, int id);
	
	void faceTarget( ObjectManager* mgr, int id);
	irr::core::vector3df attackTargetPosition(ObjectManager* mgr);
	irr::core::vector3df towerTargetPosition(ObjectManager* mgr);
	void setAnimation(std::string animation, bool loop);
	bool animationComplete();
	bool selected();
	bool targetAlive(ObjectManager* mgr);
	float distanceToObjectSq(ObjectManager* mgr, int otherID);
	bool checkTargetDifferentTeam(ObjectManager* mgr, int target);
	
	TransformComponent* currentTransComp;
	RTSLogicComponent* currentRTSComp;
	SelectableComponent* currentSelectComp;
	AnimatorComponent* currentAnimComp;
	SteeringComponent* currentSteerComp;
	HealthComponent* currentHealthComp;
	PathMovementComponent* currentPathComp;
	RenderComponent* currentRendComp;
	
	irr::core::vector3df terrainPoint;
	irr::core::vector3df objectPoint;
	int clickedObject = -1;
	int clickedTower = -1;
	//Quadtree root(0, irr::core::rectf(0, 0, 480, 480));
public:
	RTSLogicSystem() : ComponentSystem("RTSLogicSystem") {}	
	
	/**
	 * Adds waypoints to the object's path when the right-mouse button
	 * is pressed
	 */
	void update(float timestep);
	
	void draw(float timestep);
};


#endif