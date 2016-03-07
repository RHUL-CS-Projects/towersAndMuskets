#ifndef RTSVILLAGERMOVEMENTSYSTEM_H
#define RTSVILLAGERMOVEMENTSYSTEM_H

#include <ComponentSystem.h>
#include <ObjectManager.h>
#include <irrlicht/irrlicht.h>
#include <Enums.h>

#include <RTSVillagerLogicComponent.h>
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
class RTSVillagerLogicSystem : public ComponentSystem {
private:
	bool rightMouseDown = false;
	bool rightMousePressed = false;
	
	void setPath(ObjectManager* mgr, int id, irr::core::vector3df point);
	static void calcPathSynch(ObjectManager* mgr, int id, irr::core::vector3df point);
	void updateClickPoints();
	
	/*	STATES	*/
	void stateIdle(ObjectManager* mgr, int id);
	void stateWalking(ObjectManager* mgr, int id);
	void stateGather(ObjectManager* mgr, int id);
	void stateMoveToResource(ObjectManager* mgr, int id);
	void stateDead(ObjectManager* mgr, int id);
	
	/*	HELPER METHODS	*/
	void faceTarget( ObjectManager* mgr, int id);
	irr::core::vector3df resourceTargetPosition(ObjectManager* mgr, int id);
	void freeResource(ObjectManager* mgr, int id);
	void setAnimation(std::string animation, bool loop);
	bool animationComplete();
	bool selected();
	
	TransformComponent* currentTransComp;
	RTSVillagerLogicComponent* currentRTSComp;
	SelectableComponent* currentSelectComp;
	AnimatorComponent* currentAnimComp;
	SteeringComponent* currentSteerComp;
	HealthComponent* currentHealthComp;
	PathMovementComponent* currentPathComp;
	RenderComponent* currentRendComp;
	
	irr::core::vector3df terrainPoint;
	irr::core::vector3df objectPoint;
	int clickedObject = -1;
	RESOURCE_TYPE clickedObjType;
	//Quadtree root(0, irr::core::rectf(0, 0, 480, 480));
public:
	RTSVillagerLogicSystem() : ComponentSystem("RTSVillagerLogicSystem") {}	
	
	/**
	 * Adds waypoints to the object's path when the right-mouse button
	 * is pressed
	 */
	void update(float timestep);
	
	void draw(float timestep);
};


#endif