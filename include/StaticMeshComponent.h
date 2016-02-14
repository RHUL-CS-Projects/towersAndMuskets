#ifndef STATICMESHCOMPONENT_H
#define STATICMESHCOMPONENT_H

#include <irrlicht/irrlicht.h>
#include <GameComponent.h>
#include <RenderManager.h>
#include <Game.h>

/**
 * Component for storing animated 3D mesh data, as well as its
 * texture and a rotation offset if necessary
 */
class StaticMeshComponent : public GameComponent {
private:
	
public:
	// The animated mesh
	irr::scene::IMesh* mesh;
	
	// The mesh texture
	//irr::video::ITexture* texture;
	
	std::vector<irr::video::ITexture*> textures;
	
	// X, Y, and Z rotation offsets for rendering the mesh
	float meshRotOffX;
	float meshRotOffY;
	float meshRotOffZ;
	
	/**
	 * Constructor taking the name of the mesh and texture files, and the
	 * rotation offset. Uses Irrlicht to load the mesh and texture data
	 */
	StaticMeshComponent(std::string meshName, std::string textureName, irr::core::vector3df meshRotOffset) : GameComponent("StaticMeshComponent") {
		std::string meshPath = RenderManager::resPath + "/models/" + meshName;
		std::string texturePath = RenderManager::resPath + "/materials/textures/" + textureName;
		
		meshRotOffX = meshRotOffset.X;
		meshRotOffY = meshRotOffset.Y;
		meshRotOffZ = meshRotOffset.Z;
		
		mesh = Game::game.getRendMgr()->getSceneManager()->getMesh(meshPath.c_str());
		textures.push_back(Game::game.getRendMgr()->getDriver()->getTexture(texturePath.c_str()));
	}
	
	/**
	 * Constructor taking the name of the mesh and a vector of multiple texture files, and the
	 * rotation offset. Uses Irrlicht to load the mesh and texture data
	 */
	StaticMeshComponent(std::string meshName, std::vector<std::string> textureNames, irr::core::vector3df meshRotOffset) : GameComponent("AnimatedMeshComponent") {
		std::string meshPath = RenderManager::resPath + "/models/" + meshName;
		
		meshRotOffX = meshRotOffset.X;
		meshRotOffY = meshRotOffset.Y;
		meshRotOffZ = meshRotOffset.Z;
		
		mesh = Game::game.getRendMgr()->getSceneManager()->getMesh(meshPath.c_str());
		
		for (std::string s : textureNames) {
			std::string texturePath = RenderManager::resPath + "/materials/textures/" + s;
			textures.push_back(Game::game.getRendMgr()->getDriver()->getTexture(texturePath.c_str()));
		}
	}
};

#endif

