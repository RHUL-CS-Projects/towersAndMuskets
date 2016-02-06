#ifndef ANIMATEDMESHCOMPONENT_H
#define ANIMATEDMESHCOMPONENT_H

#include <irrlicht/irrlicht.h>
#include <GameComponent.h>
#include <RenderManager.h>
#include <Game.h>

/**
 * Component for storing animated 3D mesh data, as well as its
 * texture and a rotation offset if necessary
 */
class AnimatedMeshComponent : public GameComponent {
private:
	
public:
	// The animated mesh
	irr::scene::IAnimatedMesh* mesh;
	
	// The mesh texture
	irr::video::ITexture* texture;
	
	// X, Y, and Z rotation offsets for rendering the mesh
	float meshRotOffX;
	float meshRotOffY;
	float meshRotOffZ;
	
	/**
	 * Constructor taking the name of the mesh and texture files, and the
	 * rotation offset. Uses Irrlicht to load the mesh and texture data
	 */
	AnimatedMeshComponent(std::string meshName, std::string textureName, irr::core::vector3df meshRotOffset) : GameComponent("AnimatedMeshComponent") {
		std::string meshPath = RenderManager::resPath + "/models/" + meshName;
		std::string texturePath = RenderManager::resPath + "/materials/textures/" + textureName;
		
		meshRotOffX = meshRotOffset.X;
		meshRotOffY = meshRotOffset.Y;
		meshRotOffZ = meshRotOffset.Z;
		
		mesh = Game::game.getRendMgr()->getSceneManager()->getMesh(meshPath.c_str());
		texture = Game::game.getRendMgr()->getDriver()->getTexture(texturePath.c_str());
	}
};

#endif

