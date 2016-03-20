#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include <vector>
#include <irrlicht/irrlicht.h>

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

class ParticleManager {
private:
	std::vector<ParticleData> particles;

public:
	ParticleManager() {}
	
	void reset();
	void update();
	
	void addSmokeTrailParticle(irr::core::vector3df from, irr::core::vector3df to);
	void addSmokeParticle(irr::core::vector3df from);
	void addMuzzleFlashParticle(irr::core::vector3df unitpos, irr::core::vector3df unitrotation);
};

#endif