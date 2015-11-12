#ifndef ANIMATEDMESHCOMPONENT_H
#define ANIMATEDMESHCOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>
#include <RenderManager.h>

class AnimatedMeshComponent : public GameComponent {
private:
	
public:
	irr::scene::IAnimatedMesh* mesh;
	irr::video::ITexture* texture;
	
	AnimatedMeshComponent(std::string meshName, std::string textureName) : GameComponent("AnimatedMeshComponent") {
		std::string meshPath = RenderManager::resPath + "/models/" + meshName;
		std::string texturePath = RenderManager::resPath + "/materials/textures/" + textureName;
		
		mesh = RenderManager::renderManager.getSceneManager()->getMesh(meshPath.c_str());
		texture = RenderManager::renderManager.getDriver()->getTexture(texturePath.c_str());
	}
};

#endif

