#include <ParticleManager.h>
#include <Game.h>
#include <TransparentMaterialShader.h>

using namespace irr;
using namespace core;
using namespace scene;

void ParticleManager::addSmokeTrailParticle(vector3df from, vector3df to) {
	ParticleData newPart;
	IMesh* planeMesh = Game::game.getRendMgr()->getSceneManager()->getGeometryCreator()->createPlaneMesh(dimension2df(1,1.5));
	newPart.node = Game::game.getRendMgr()->getSceneManager()->addMeshSceneNode(planeMesh);
	
	vector3df dif = to - from;
	
	newPart.node->setMaterialTexture(0, Game::game.getRendMgr()->getDriver()->getTexture("./res/materials/textures/smoketrail.png"));
	newPart.node->setMaterialFlag(video::EMF_BLEND_OPERATION, true);
	newPart.node->setMaterialType((video::E_MATERIAL_TYPE)TransparentMaterialShader::materialID);
	
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
	
	newPart.life = 120;
	newPart.startLife = 120;
	
	newPart.startScale = startScale;
	newPart.finalScale = startScale + vector3df(0,0,5);
	
	particles.push_back(newPart);
	
	addSmokeParticle(from);
}

void ParticleManager::addSmokeParticle ( vector3df from ) {
	ParticleData newPart;

	newPart.node = Game::game.getRendMgr()->getSceneManager()->addBillboardSceneNode(0, vector2df(10,10));
	
	newPart.node->setPosition(from);
	newPart.node->setMaterialTexture(0, Game::game.getRendMgr()->getDriver()->getTexture("./res/materials/textures/smoke.png"));
	newPart.node->setMaterialFlag(video::EMF_BLEND_OPERATION, true);
	newPart.node->setMaterialType((video::E_MATERIAL_TYPE)TransparentMaterialShader::materialID);
	
	double scale = 1.0 / 255 * (double)(rand()%255);
	vector3df startScale = vector3df(0.2 + 0.8*scale, 0.2 + 0.8*scale, 0.2 + 0.8*scale);
	
	newPart.node->setScale(startScale);
	
	newPart.startAlpha = 0.4;
	newPart.alpha = newPart.startAlpha;
	newPart.fade = true;
	
	newPart.node->getMaterial(0).MaterialTypeParam = newPart.startAlpha;
	
	newPart.life = 200;
	newPart.startLife = 200;
	
	newPart.startScale = startScale;
	newPart.finalScale = startScale + vector3df(10,10,10);
	
	particles.push_back(newPart);
}

void ParticleManager::addMuzzleFlashParticle ( vector3df unitpos, vector3df unitrotation ) {
	ParticleData newPart;
	IAnimatedMesh* flashMesh = Game::game.getRendMgr()->getSceneManager()->getMesh("./res/models/musketflash.x");
	newPart.node = Game::game.getRendMgr()->getSceneManager()->addAnimatedMeshSceneNode(flashMesh);
	
	newPart.node->setPosition(unitpos);
	newPart.node->setMaterialTexture(0, Game::game.getRendMgr()->getDriver()->getTexture("./res/materials/textures/musketflash.png"));
	newPart.node->setMaterialFlag(video::EMF_BLEND_OPERATION, true);
	newPart.node->setMaterialType((video::E_MATERIAL_TYPE)TransparentMaterialShader::materialID);
	newPart.node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	
	vector3df startScale = vector3df(1, 1, 1);
	
	newPart.node->setRotation(unitrotation);
	newPart.node->setScale(startScale);
	
	newPart.startAlpha = 1;
	newPart.alpha = newPart.startAlpha;
	newPart.fade = true;
	
	newPart.node->getMaterial(0).MaterialTypeParam = newPart.startAlpha;
	
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
			if (p->node->getParent())
				p->node->getParent()->remove();
			else
				p->node->remove();
			particles.erase(it);
		}
	}
}


