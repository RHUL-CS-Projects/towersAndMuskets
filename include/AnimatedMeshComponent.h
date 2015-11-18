#ifndef ANIMATEDMESHCOMPONENT_H
#define ANIMATEDMESHCOMPONENT_H

#include <irrlicht/irrlicht.h>
#include <GameComponent.h>
#include <RenderManager.h>

class AnimatedMeshComponent : public GameComponent {
private:
	
public:
	irr::scene::IAnimatedMesh* mesh;
	irr::video::ITexture* texture;
	float meshRotOffX;
	float meshRotOffY;
	float meshRotOffZ;
	
	AnimatedMeshComponent(std::string meshName, std::string textureName, irr::core::vector3df meshRotOffset) : GameComponent("AnimatedMeshComponent") {
		std::string meshPath = RenderManager::resPath + "/models/" + meshName;
		std::string texturePath = RenderManager::resPath + "/materials/textures/" + textureName;
		
		meshRotOffX = meshRotOffset.X;
		meshRotOffY = meshRotOffset.Y;
		meshRotOffZ = meshRotOffset.Z;
		
		mesh = RenderManager::renderManager.getSceneManager()->getMesh(meshPath.c_str());
		texture = RenderManager::renderManager.getDriver()->getTexture(texturePath.c_str());
	}
};

#endif

