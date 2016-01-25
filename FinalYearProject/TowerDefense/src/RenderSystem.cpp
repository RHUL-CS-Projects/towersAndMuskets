#include <list>

#include <RenderSystem.h>
#include <ObjectManager.h>
#include <SelectableComponent.h>
#include <FaceDirectionComponent.h>
#include <AnimatorComponent.h>

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
			
			AnimatedMeshComponent* animComp = mgr->getObjectComponent<AnimatedMeshComponent>(i, "AnimatedMeshComponent");
			TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
			
			// Check that each object has a transform and mesh
			if (animComp == nullptr || transComp == nullptr)
				continue;
			
			// Check all renderable objects have been added to scene
			if (rendComp->sceneNode == nullptr) {
				addSceneNode(rendComp, animComp, transComp, i);
				
				// Initialise the object's animation if it has an AnimatorComponent
				AnimatorComponent* animatorComp = mgr->getObjectComponent<AnimatorComponent>(i, "AnimatorComponent");
				
				if (animatorComp != nullptr) {
					animatorComp->setAnimation(animatorComp->currentAnimation, rendComp->sceneNode);
				}
			}
			
			// Check if object is selectable and create a selection plane to render
			SelectableComponent* selectComp = mgr->getObjectComponent<SelectableComponent>(i, "SelectableComponent");
			if (selectComp != nullptr) {
				
				if (selectComp->selectionMesh == nullptr) {
					irr::core::dimension2d<irr::f32> tileSize(4*selectComp->selectionXScale,4*selectComp->selectionZScale);
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
			
			// Check if the object has a facing direction
			FaceDirectionComponent* dirComp = mgr->getObjectComponent<FaceDirectionComponent>(i, "FaceDirectionComponent");
			vector3df facingDirection(0,0,0);
			vector3df meshRot(animComp->meshRotOffX, animComp->meshRotOffY, animComp->meshRotOffZ);
			
			if (dirComp != nullptr) {
				facingDirection = vector3df(0, dirComp->currentYRot, 0);
				rendComp->sceneNode->setRotation(facingDirection + meshRot);
			} else {
				rendComp->sceneNode->setRotation(transComp->rotation + meshRot);
			}
			
			rendComp->sceneNode->setPosition(transComp->worldPosition);
			rendComp->sceneNode->setScale(transComp->scale);
		}	
	}
}


void RenderSystem::draw ( float timestep ) {
	RenderManager::renderManager.getSceneManager()->drawAll();
}

void RenderSystem::addSceneNode (RenderComponent* rendComp, AnimatedMeshComponent* animComp, TransformComponent* transComp, int id ) {
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
		animnode->setFrameLoop(62, 142);
		animnode->setCurrentFrame(0 + rand() % 62);
		
		ITriangleSelector* selector = smgr->createTriangleSelector(animnode);
		animnode->setTriangleSelector(selector);
		animnode->setID(id);
	}
	
	//terrain->setTriangleSelector(terrainSelector);
	//terrain->setName("MainTerrain");
	
	rendComp->sceneNode = animnode;
}








