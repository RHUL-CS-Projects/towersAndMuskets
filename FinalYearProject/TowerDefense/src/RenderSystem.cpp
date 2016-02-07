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
	ObjectManager* mgr = Game::game.getObjMgr();
	irr::scene::ISceneManager* smgr = Game::game.getRendMgr()->getSceneManager();
	
	// Get all renderable objects
	std::list<int> renderables = mgr->getObjectsWithComponent("RenderComponent");

	if (renderables.size() > 0) {	
		for (int i : renderables) {
			RenderComponent* rendComp;
			
			// Check object is visible
			if (!(rendComp = mgr->getObjectComponent<RenderComponent>(i, "RenderComponent"))->visible)
				continue;
			
			AnimatedMeshComponent* animComp = mgr->getObjectComponent<AnimatedMeshComponent>(i, "AnimatedMeshComponent");
			StaticMeshComponent* meshComp = mgr->getObjectComponent<StaticMeshComponent>(i, "StaticMeshComponent");
			TransformComponent* transComp = mgr->getObjectComponent<TransformComponent>(i, "TransformComponent");
			
			// Check that each object has a transform and mesh
			if (transComp == nullptr || (meshComp == nullptr && animComp == nullptr))
				continue;
			
			// Check all renderable objects have been added to scene
			if (rendComp->sceneNode == nullptr || rendComp->sceneNodeStatic == nullptr) {
				
				if (animComp != nullptr && rendComp->sceneNode == nullptr)
					addAnimatedSceneNode(rendComp, animComp, transComp, i);
				
				if (meshComp != nullptr && rendComp->sceneNodeStatic == nullptr)
					addStaticSceneNode(rendComp, meshComp, transComp, i);
				
				if (animComp != nullptr) {
					// Initialise the object's animation if it has an AnimatorComponent
					AnimatorComponent* animatorComp = mgr->getObjectComponent<AnimatorComponent>(i, "AnimatorComponent");
					
					if (animatorComp != nullptr) {
						animatorComp->setAnimation(animatorComp->currentAnimation, rendComp->sceneNode);
					}
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
					ITexture* texture = Game::game.getRendMgr()->getDriver()->getTexture(texturePath.c_str());
					
					selectComp->sceneNode->setMaterialTexture(0, texture);
					selectComp->sceneNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
					selectComp->sceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
					selectComp->sceneNode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
				}
				
				selectComp->sceneNode->setVisible(selectComp->selected);
				selectComp->sceneNode->setPosition(transComp->worldPosition + vector3df(0,0.2f,0));			
			}
			
			// Check if the object has a facing direction
			FaceDirectionComponent* dirComp = mgr->getObjectComponent<FaceDirectionComponent>(i, "FaceDirectionComponent");
			vector3df facingDirection(0,0,0);
			vector3df meshRot(0,0,0);
			
			if (animComp != nullptr)
				meshRot = vector3df(animComp->meshRotOffX, animComp->meshRotOffY, animComp->meshRotOffZ);
			else if (meshComp != nullptr)
				meshRot = vector3df(meshComp->meshRotOffX, meshComp->meshRotOffY, meshComp->meshRotOffZ);
			
			if (dirComp != nullptr) {
				facingDirection = vector3df(0, dirComp->currentYRot, 0);
				
				if (rendComp->sceneNode != nullptr)
					rendComp->sceneNode->setRotation(facingDirection + meshRot);
				
				if (rendComp->sceneNodeStatic != nullptr)
					rendComp->sceneNodeStatic->setRotation(facingDirection + meshRot);
			} else {
				if (rendComp->sceneNode != nullptr)
					rendComp->sceneNode->setRotation(transComp->rotation + meshRot);
				
				if (rendComp->sceneNodeStatic != nullptr)
					rendComp->sceneNodeStatic->setRotation(transComp->rotation + meshRot);
			}
			
			if (rendComp->sceneNode != nullptr) {
				rendComp->sceneNode->setPosition(transComp->worldPosition);
				rendComp->sceneNode->setScale(transComp->scale);
			}
			
			
			if (rendComp->sceneNodeStatic != nullptr) {
				rendComp->sceneNodeStatic->setPosition(transComp->worldPosition);
				rendComp->sceneNodeStatic->setScale(transComp->scale);
			}
		}	
	}
}

void RenderSystem::draw ( float timestep ) {
	Game::game.getRendMgr()->getSceneManager()->drawAll();
}

void RenderSystem::addAnimatedSceneNode (RenderComponent* rendComp, AnimatedMeshComponent* animComp, TransformComponent* transComp, int id ) {
	irr::video::IVideoDriver* driver = Game::game.getRendMgr()->getDriver();
	irr::scene::ISceneManager* smgr = Game::game.getRendMgr()->getSceneManager();
	
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
	
	rendComp->sceneNode = animnode;
}

void RenderSystem::addStaticSceneNode (RenderComponent* rendComp, StaticMeshComponent* meshComp, TransformComponent* transComp, int id ) {
	irr::video::IVideoDriver* driver = Game::game.getRendMgr()->getDriver();
	irr::scene::ISceneManager* smgr = Game::game.getRendMgr()->getSceneManager();
	
	IMeshSceneNode* meshnode = smgr->addMeshSceneNode(meshComp->mesh);
	if (meshnode) {
		meshnode->setMaterialFlag(video::EMF_LIGHTING, true);
		meshnode->setMaterialFlag(video::EMF_GOURAUD_SHADING, true);

		meshnode->setMaterialTexture(0, meshComp->texture);
		meshnode->setPosition(transComp->worldPosition);
		meshnode->setRotation(transComp->rotation);
		meshnode->setScale(transComp->scale);
		meshnode->getMaterial(0).SpecularColor = SColor(0,0,0,0);

		meshnode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
		meshnode->addShadowVolumeSceneNode();
		
		ITriangleSelector* selector = smgr->createTriangleSelector(meshComp->mesh, meshnode);
		meshnode->setTriangleSelector(selector);
		meshnode->setID(id);
	}
	
	rendComp->sceneNodeStatic = meshnode;
}






