#include <FaceDirectionSystem.h>
#include <ObjectManager.h>
#include <FaceDirectionComponent.h>
#include <list>

void FaceDirectionSystem::update ( float timestep ) {
    // Get the object manager
	ObjectManager* mgr = &ObjectManager::manager;
	
	// Get objects with FaceDirectionComponents
	std::list<int> objects = mgr->getObjectsWithComponent("FaceDirectionComponent");
	
	for (int i : objects) {
		FaceDirectionComponent* dirComp = mgr->getObjectComponent<FaceDirectionComponent>(i, "FaceDirectionComponent");
		
		// Only lerp the angle if there is a significant difference between facing and target directions
		if (abs(dirComp->currentYRot - dirComp->targetYRot) > 1)
			dirComp->currentYRot = interpolateAngles(dirComp->currentYRot, dirComp->targetYRot, dirComp->rotateSpeed);
	}
}


void FaceDirectionSystem::draw ( float timestep ) {
    ComponentSystem::draw ( timestep );
}


float FaceDirectionSystem::interpolateAngles ( float a, float b, float c ) {
	float shortest = fmod(((fmod(b - a, 360)) + 540), 360) - 180;
    return a + shortest * c;
}
