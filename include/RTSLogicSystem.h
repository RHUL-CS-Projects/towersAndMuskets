#ifndef RTSMOVEMENTSYSTEM_H
#define RTSMOVEMENTSYSTEM_H

#include <ComponentSystem.h>
#include <ObjectManager.h>
#include <irrlicht/irrlicht.h>
#include <RTSLogicComponent.h>
#include <SteeringComponent.h>

/**
 * System to update a selected object's path when the map is right-clicked
 */
class RTSLogicSystem : public ComponentSystem {
private:
	bool rightMouseDown = false;
	bool rightMousePressed = false;
	
	void setPath(ObjectManager* mgr, int id, irr::core::vector3df point);
	
	void stateIdle(ObjectManager* mgr, int id, RTSLogicComponent* rtsComp, SteeringComponent* steerComp, bool selected);
	void stateWalking(ObjectManager* mgr, int id, RTSLogicComponent* rtsComp, SteeringComponent* steerComp, bool selected);
	void stateReloading(ObjectManager* mgr, int id, RTSLogicComponent* rtsComp, SteeringComponent* steerComp, bool selected);
	void stateAttacking(ObjectManager* mgr, int id, RTSLogicComponent* rtsComp, SteeringComponent* steerComp, bool selected);
	void stateMoveToAttack(ObjectManager* mgr, int id, RTSLogicComponent* rtsComp, SteeringComponent* steerComp, bool selected);
	void stateDead(ObjectManager* mgr, int id, RTSLogicComponent* rtsComp, SteeringComponent* steerComp, bool selected);
	
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