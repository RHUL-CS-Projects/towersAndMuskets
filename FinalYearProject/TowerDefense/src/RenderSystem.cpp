#include <RenderSystem.h>
#include <RenderManager.h>
#include <list>
#include <AnimatedMeshComponent.h>
#include <TransformComponent.h>

void RenderSystem::update ( float timestep ) {
	// Get the object manager
	ObjectManager* mgr = &ObjectManager::manager;
	
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
			
			rendComp->sceneNode->setPosition(transComp->worldPosition);
			rendComp->sceneNode->setRotation(transComp->rotation);
			rendComp->sceneNode->setScale(transComp->scale);
		}	
	}
	
	RenderManager::renderManager.getDriver()->beginScene(true, true, SColor(255,159,200,214));	
	RenderManager::renderManager.getSceneManager()->drawAll();
	RenderManager::renderManager.getDriver()->endScene();
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
		
		animnode->setAnimationSpeed(70);
		animnode->setFrameLoop(63, 142);
	}
	
	rendComp->sceneNode = animnode;
}








