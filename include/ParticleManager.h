#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include <vector>
#include <irrlicht/irrlicht.h>
#include <string>

struct ParticleData {
	irr::scene::ISceneNode* node;
	int startLife;
	int life;
	bool fade;
	double alpha;
	double startAlpha;
	irr::core::vector3df startScale;
	irr::core::vector3df finalScale;
};

enum EFFECT_TYPE { ET_MUSKET, ET_CANNON };

class ParticleManager {
private:
	std::vector<ParticleData> particles;
	bool partsVisible = true;
	
	void addSmokeTrailParticle(irr::core::vector3df from, irr::core::vector3df to);
	void addSmokeParticle(irr::core::vector3df from);
	void addMuzzleFlashParticle(irr::core::vector3df unitpos, irr::core::vector3df unitrotation, std::string mesh, std::string texture);
public:
	ParticleManager() {}
	
	void reset();
	void update();
	
	void toggleParticleVisibility();
	void spawnEffect(irr::core::vector3df from, irr::core::vector3df to, irr::core::vector3df unitrotation, EFFECT_TYPE type);
};

#endif