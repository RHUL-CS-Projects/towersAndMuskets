#include <ParticleManager.h>
#include <Game.h>
#include <TransparentMaterialShader.h>

using namespace irr;
using namespace core;
using namespace scene;

void ParticleManager::spawnEffect ( vector3df from, vector3df to, vector3df unitrotation, EFFECT_TYPE type ) {
	double yrot;
	
	switch (type) {
		case ET_MUSKET:
			yrot = degToRad(unitrotation.Y);
			addSmokeTrailParticle(from + vector3df(sin(yrot)*-4, 7.5, cos(yrot)*-4), to + vector3df(0, 7.5, 0));
			addSmokeParticle(from + vector3df(sin(yrot)*-4, 7.5, cos(yrot)*-4));
			addMuzzleFlashParticle(from, vector3df(0, unitrotation.Y, 0), "musketflash.x", "musketflash.png");
			break;
		case ET_CANNON:
			yrot = degToRad(unitrotation.Y);
			addSmokeTrailParticle(from + vector3df(sin(yrot)*-6, 4.5, cos(yrot)*-6), to + vector3df(0, 7.5, 0));
			addSmokeParticle(from + vector3df(sin(yrot)*-6, 4.5, cos(yrot)*-6));
			addMuzzleFlashParticle(from, vector3df(0, unitrotation.Y, 0), "cannonflash.x", "musketflash.png");
			break;
		default:
			break;
	}
}

void ParticleManager::addSmokeTrailParticle(vector3df from, vector3df to) {
	// Top down plane	
	ParticleData newPart;
	IMesh* planeMesh = Game::game.getRendMgr()->getSceneManager()->getGeometryCreator()->createPlaneMesh(dimension2df(1,1.5));
	newPart.node = Game::game.getRendMgr()->getSceneManager()->addMeshSceneNode(planeMesh);
	
	vector3df dif = to - from;
	
	newPart.node->setMaterialTexture(0, Game::game.getRendMgr()->getDriver()->getTexture("./res/materials/textures/smoketrail.png"));
	newPart.node->setMaterialFlag(video::EMF_BLEND_OPERATION, true);
	newPart.node->setMaterialType((video::E_MATERIAL_TYPE)TransparentMaterialShader::materialID);
	newPart.node->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	
	vector3df startScale = vector3df(dif.getLength(), 1, 1);

	ISceneNode* parent = Game::game.getRendMgr()->getSceneManager()->addEmptySceneNode();
	parent->addChild(newPart.node);
	parent->setRotation(vector3df(0, 90+radToDeg(atan2(dif.X, dif.Z)), 0));
	parent->setPosition(from + dif/2);
	newPart.node->setPosition(vector3df(0,0,0));
	
	double h = dif.getLength();
	double zangle = asin(-dif.Y / h);
	
	newPart.node->setRotation(vector3df(0, 0, radToDeg(zangle)));
	newPart.node->setScale(startScale);
	
	newPart.startAlpha = 0.4;
	newPart.alpha = newPart.startAlpha;
	newPart.fade = true;
	
	newPart.node->getMaterial(0).MaterialTypeParam = newPart.startAlpha;
	newPart.node->getMaterial(0).MaterialTypeParam2 = -1;
	
	newPart.life = 300;
	newPart.startLife = 300;
	
	newPart.startScale = startScale;
	newPart.finalScale = startScale + vector3df(0,0,5);
	
	particles.push_back(newPart);
	
	// Sideways plane
	ParticleData newPart2;
	parent = Game::game.getRendMgr()->getSceneManager()->addEmptySceneNode();
	planeMesh = Game::game.getRendMgr()->getSceneManager()->getGeometryCreator()->createPlaneMesh(dimension2df(1,1.5));
	newPart2.node = Game::game.getRendMgr()->getSceneManager()->addMeshSceneNode(planeMesh);
	
	dif = to - from;
	
	newPart2.node->setMaterialTexture(0, Game::game.getRendMgr()->getDriver()->getTexture("./res/materials/textures/smoketrail.png"));
	newPart2.node->setMaterialFlag(video::EMF_BLEND_OPERATION, true);
	newPart2.node->setMaterialType((video::E_MATERIAL_TYPE)TransparentMaterialShader::materialID);
	newPart2.node->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	
	startScale = vector3df(dif.getLength(), 1, 1);

	parent->addChild(newPart2.node);
	parent->setRotation(vector3df(0, 90+radToDeg(atan2(dif.X, dif.Z)), 0));
	parent->setPosition(from + dif/2);
	newPart2.node->setPosition(vector3df(0,0,0));
	
	h = dif.getLength();
	zangle = asin(-dif.Y / h);
	
	newPart2.node->setRotation(vector3df(90, 0, radToDeg(zangle)));
	newPart2.node->setScale(startScale);
	
	newPart2.startAlpha = 0.4;
	newPart2.alpha = newPart2.startAlpha;
	newPart2.fade = true;
	
	newPart2.node->getMaterial(0).MaterialTypeParam = newPart2.startAlpha;
	newPart2.node->getMaterial(0).MaterialTypeParam2 = -1;
	
	newPart2.node->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
	
	newPart2.life = 300;
	newPart2.startLife = 300;
	
	newPart2.startScale = startScale;
	newPart2.finalScale = startScale + vector3df(0,0,5);
	
	particles.push_back(newPart2);
}

void ParticleManager::addSmokeParticle ( vector3df from ) {
	ParticleData newPart;

	newPart.node = Game::game.getRendMgr()->getSceneManager()->addBillboardSceneNode(0, vector2df(10,10));
	
	newPart.node->setPosition(from);
	newPart.node->setMaterialTexture(0, Game::game.getRendMgr()->getDriver()->getTexture("./res/materials/textures/smoke.png"));
	newPart.node->setMaterialFlag(video::EMF_BLEND_OPERATION, true);
	newPart.node->setMaterialType((video::E_MATERIAL_TYPE)TransparentMaterialShader::materialID);
	newPart.node->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	
	double scale = 1.0 / 255 * (double)(rand()%255);
	vector3df startScale = vector3df(0.2 + 0.8*scale, 0.2 + 0.8*scale, 0.2 + 0.8*scale);
	
	newPart.node->setScale(startScale);
	
	newPart.startAlpha = 0.4;
	newPart.alpha = newPart.startAlpha;
	newPart.fade = true;
	
	newPart.node->getMaterial(0).MaterialTypeParam = newPart.startAlpha;
	newPart.node->getMaterial(0).MaterialTypeParam2 = -1;
	
	newPart.life = 400;
	newPart.startLife = 400;
	
	newPart.startScale = startScale;
	newPart.finalScale = startScale + vector3df(10,10,10);
	
	particles.push_back(newPart);
}

void ParticleManager::addMuzzleFlashParticle ( vector3df unitpos, vector3df unitrotation, std::string mesh, std::string texture ) {
	ParticleData newPart;
	IAnimatedMesh* flashMesh = Game::game.getRendMgr()->getSceneManager()->getMesh(("./res/models/" + mesh).c_str());
	newPart.node = Game::game.getRendMgr()->getSceneManager()->addAnimatedMeshSceneNode(flashMesh);
	
	newPart.node->setPosition(unitpos);
	newPart.node->setMaterialTexture(0, Game::game.getRendMgr()->getDriver()->getTexture(("./res/materials/textures/" + texture).c_str()));
	newPart.node->setMaterialFlag(video::EMF_BLEND_OPERATION, true);
	newPart.node->setMaterialType((video::E_MATERIAL_TYPE)TransparentMaterialShader::materialID);
	newPart.node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	newPart.node->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	
	vector3df startScale = vector3df(1, 1, 1);
	
	newPart.node->setRotation(unitrotation);
	newPart.node->setScale(startScale);
	
	newPart.startAlpha = 1;
	newPart.alpha = newPart.startAlpha;
	newPart.fade = true;
	
	newPart.node->getMaterial(0).MaterialTypeParam = newPart.startAlpha;
	newPart.node->getMaterial(0).MaterialTypeParam2 = -1;
	
	newPart.life = 2;
	newPart.startLife = 2;
	
	newPart.startScale = startScale;
	newPart.finalScale = startScale + vector3df(0,0,0);
	
	particles.push_back(newPart);
}

void ParticleManager::reset() {
	for (auto it = particles.begin(); it < particles.end();) {
		ParticleData* p = &(*it);
		if (p->node->getParent())
			p->node->getParent()->remove();
		else
			p->node->remove();
		it++;
	}
	
	particles.clear();
}

void ParticleManager::update() {
	for (auto it = particles.begin(); it < particles.end();) {
		ParticleData* p = &(*it);
		
		if (p->life > 0) {
			p->life--;
			
			if (p->fade) 
				p->alpha = p->startAlpha * (1.0 / (double)p->startLife * p->life);
			else
				p->alpha = p->startAlpha;
			
			p->node->setScale(p->finalScale + (p->startScale-p->finalScale) * (1.0 / (double)p->startLife * p->life));
			p->node->getMaterial(0).MaterialTypeParam = p->alpha;
			
			it++;
		} else {
			p->node->remove();
			particles.erase(it);
		}
	}
}

void ParticleManager::toggleParticleVisibility() {
	partsVisible = !partsVisible;
	
	for (ParticleData p : particles) {
		p.node->setVisible(partsVisible);
	}
}

