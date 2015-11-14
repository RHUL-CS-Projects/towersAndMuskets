#include <list>
#include <CameraSystem.h>
#include <CameraComponent.h>
#include <ObjectManager.h>
#include <RenderManager.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

void CameraSystem::update ( float timestep ) {
    ISceneManager* smgr = RenderManager::renderManager.getSceneManager();
	ICameraSceneNode* camera = smgr->getActiveCamera();
	
	// Get the object manager
	ObjectManager* mgr = &ObjectManager::manager;
	
	std::list<int> cameraObjects = mgr->getObjectsWithComponent("CameraComponent");
	
	if (cameraObjects.size() > 0) {
		for (int i : cameraObjects) {
			// Get camera component of object
			CameraComponent* camComp = mgr->getObjectComponent<CameraComponent>(i, "CameraComponent");
			
			// Check camera is active
			if (!camComp->active)
				continue;
			
			// Check the object has a transform
			TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
			
			if (transComp == nullptr)
				continue;
			
			// Update camera transform
			camera->setPosition(transComp->worldPosition);
		}
	}
}


void CameraSystem::draw ( float timestep ) {
    
}



