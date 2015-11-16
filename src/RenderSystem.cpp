#include <list>

#include <RenderSystem.h>
#include <ObjectManager.h>
#include <SelectableComponent.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

void RenderSystem::update ( float timestep ) {
	// Get the object manager
	ObjectManager* mgr = &ObjectManager::manager;
	irr::scene::ISceneManager* smgr = RenderManager::renderManager.getSceneManager();
	
	// Get all renderable objects
	std::list<int> renderables = mgr->getObjectsWithComponent("RenderComponent");

	if (renderables.size() > 0) {	
		for (int i : renderables) {
			RenderComponent* rendComp;
			
			// Check object is visible
			if (!(rendComp = mgr->getObjectComponent<RenderComponent>(i, "RenderComponent"))->visible)
				continue;
			
			AnimatedMeshComponent* animComp;
			TransformComponent* transComp;
			
			// Check that each object has a transform and mesh
			if ((animComp = mgr->getObjectComponent<AnimatedMeshComponent>(i, "AnimatedMeshComponent")) == nullptr 
				|| (transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent")) == nullptr)
				continue;
			
			// Check all renderable objects have been added to scene
			if (rendComp->sceneNode == nullptr) 
				addSceneNode(rendComp, animComp, transComp);
			
			// Check if object is selectable and create a selection plane to render
			SelectableComponent* selectComp;
			if ((selectComp = mgr->getObjectComponent<SelectableComponent>(i, "SelectableComponent")) != nullptr) {
				
				if (selectComp->selectionMesh == nullptr) {
					irr::core::dimension2d<irr::f32> tileSize(4,4);
					irr::core::dimension2d<irr::u32> tileCount(1,1);

					selectComp->selectionMesh = smgr->getGeometryCreator()->createPlaneMesh(tileSize, tileCount);
				}
				
				if (selectComp->sceneNode == nullptr) {
					selectComp->sceneNode = smgr->addMeshSceneNode(selectComp->selectionMesh);
					std::string texturePath = RenderManager::resPath + "/materials/textures/Selected.png";
					ITexture* texture = RenderManager::renderManager.getDriver()->getTexture(texturePath.c_str());
					
					selectComp->sceneNode->setMaterialTexture(0, texture);
					selectComp->sceneNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
					selectComp->sceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
					selectComp->sceneNode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
				}
				
				selectComp->sceneNode->setVisible(selectComp->selected);
				selectComp->sceneNode->setPosition(transComp->worldPosition + vector3df(0,0.01f,0));			
			}
			
			rendComp->sceneNode->setPosition(transComp->worldPosition);
			rendComp->sceneNode->setRotation(transComp->rotation);
			rendComp->sceneNode->setScale(transComp->scale);
		}	
	}
}


void RenderSystem::draw ( float timestep ) {
    RenderManager::renderManager.getSceneManager()->drawAll();
	RenderManager::renderManager.getGUIEnvironment()->drawAll();
}

void RenderSystem::addSceneNode ( RenderComponent* rendComp, AnimatedMeshComponent* animComp, TransformComponent* transComp ) {
	irr::video::IVideoDriver* driver = RenderManager::renderManager.getDriver();
	irr::scene::ISceneManager* smgr = RenderManager::renderManager.getSceneManager();
	
	IAnimatedMeshSceneNode* animnode = smgr->addAnimatedMeshSceneNode(animComp->mesh);
	if (animnode) {
		animnode->setMaterialFlag(video::EMF_LIGHTING, true);
		animnode->setMaterialFlag(video::EMF_GOURAUD_SHADING, true);

		animnode->setMaterialTexture(0, animComp->texture);
		animnode->setPosition(transComp->worldPosition);
		animnode->setRotation(transComp->rotation);
		animnode->setScale(transComp->scale);
		animnode->getMaterial(0).SpecularColor = SColor(0,0,0,0);

		animnode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

		animnode->addShadowVolumeSceneNode();
		
		animnode->setAnimationSpeed(50);
		animnode->setFrameLoop(0, 61);
	}
	
	rendComp->sceneNode = animnode;
}








