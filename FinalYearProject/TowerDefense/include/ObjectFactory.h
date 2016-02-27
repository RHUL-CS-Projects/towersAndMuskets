#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include <ObjectManager.h>
#include <Game.h>
#include <TransformComponent.h>
#include <StaticMeshComponent.h>
#include <RenderComponent.h>
#include <SteeringComponent.h>
#include <SelectableComponent.h>
#include <FaceDirectionComponent.h>
#include <AnimatedMeshComponent.h>
#include <TowerComponent.h>
#include <AnimatorComponent.h>
#include <RTSLogicComponent.h>
#include <HealthComponent.h>
#include <irrlicht/irrlicht.h>
#include <sfml/SFML/Audio.hpp>
#include <string>

class ObjectFactory {
public:
	static int addRock(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		objmgr->attachComponent(id, new StaticMeshComponent("rock.x", "RockTexture.png", irr::core::vector3df(-90,0,0)));
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new SteeringComponent(0, 80, 8));
		
		return id;
	}
	
	static int addTree(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		objmgr->attachComponent(id, new StaticMeshComponent("tree.x", "TreeTexture.png", irr::core::vector3df(-90,0,0)));
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new SteeringComponent(0, 80));
		
		return id;
	}
	
	static int addTower(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		objmgr->attachComponent(id, new StaticMeshComponent("tower2.x", "TowerTexture.png", irr::core::vector3df(-90,0,0)));
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new SteeringComponent(0, 80));
		objmgr->attachComponent(id, new TowerComponent());
		
		return id;
	}
	
	static int addEnemyUnit(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		
		objmgr->attachComponent(id, new AnimatedMeshComponent("humantest.x", "ManTexture2.png",irr::core::vector3df(0,0,0)));
		
		AnimatorComponent* animComp = new AnimatorComponent();
		animComp->addAnimation("IDLE", 0, 62, 30);
		animComp->addAnimation("WALK", 63, 142, 90);
		animComp->addAnimation("TAKEAIM", 143, 153, 20);
		animComp->addAnimation("AIM", 154, 156, 30);
		animComp->addAnimation("SHOOT", 157, 165, 30);
		animComp->addAnimation("RELOAD", 166, 275, 20);
		animComp->addAnimation("REST", 276, 287, 30);
		animComp->addAnimation("DEATH1", 288, 313, 20);
		
		objmgr->attachComponent(id, animComp);
		
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new FaceDirectionComponent(0, 0.08f));
		objmgr->attachComponent(id, new RTSLogicComponent(1, 1, Game::game.resources.loadSound("musketshot.ogg"), 60, false));
		objmgr->attachComponent(id, new SteeringComponent(0.2, 80));
		objmgr->attachComponent(id, new HealthComponent(10, 10));
	}
	
	static int addPlayerUnit(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		
		objmgr->attachComponent(id, new AnimatedMeshComponent("humantest.x", "ManTexture.png",irr::core::vector3df(0,0,0)));
		
		AnimatorComponent* animComp = new AnimatorComponent();
		animComp->addAnimation("IDLE", 0, 62, 30);
		animComp->addAnimation("WALK", 63, 142, 90);
		animComp->addAnimation("TAKEAIM", 143, 153, 20);
		animComp->addAnimation("AIM", 154, 156, 30);
		animComp->addAnimation("SHOOT", 157, 165, 30);
		animComp->addAnimation("RELOAD", 166, 275, 20);
		animComp->addAnimation("REST", 276, 287, 30);
		animComp->addAnimation("DEATH1", 288, 313, 20);
		
		objmgr->attachComponent(id, animComp);
		
		objmgr->attachComponent(id, new SelectableComponent());
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new FaceDirectionComponent(0, 0.08f));
		objmgr->attachComponent(id, new RTSLogicComponent(0, 1, Game::game.resources.loadSound("musketshot.ogg"), 60, true));
		objmgr->attachComponent(id, new SteeringComponent(0.2, 80));
		objmgr->attachComponent(id, new HealthComponent(10, 10));
	}
	
	static int addPlayerCannon(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		
		std::vector<std::string> textures;
		textures.push_back("ManTexture.png");
		textures.push_back("CannonTexture.png");
		
		objmgr->attachComponent(id, new AnimatedMeshComponent("cannon.x", textures, irr::core::vector3df(0,0,0)));
		
		AnimatorComponent* animComp = new AnimatorComponent();
		animComp->addAnimation("IDLE", 0, 62, 30);
		animComp->addAnimation("WALK", 63, 142, 90);
		animComp->addAnimation("TAKEAIM", 143, 153, 20);
		animComp->addAnimation("AIM", 154, 156, 30);
		animComp->addAnimation("SHOOT", 157, 165, 30);
		animComp->addAnimation("RELOAD", 166, 275, 20);
		animComp->addAnimation("REST", 276, 287, 30);
		animComp->addAnimation("DEATH1", 288, 313, 20);
		
		objmgr->attachComponent(id, animComp);
		
		objmgr->attachComponent(id, new SelectableComponent(3,3));
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new FaceDirectionComponent(0, 0.08f));
		objmgr->attachComponent(id, new RTSLogicComponent(0, 1, Game::game.resources.loadSound("musketshot.ogg"), 60, false));
		objmgr->attachComponent(id, new SteeringComponent(0.1, 80, 8));
		objmgr->attachComponent(id, new HealthComponent(10, 10));
	}
};

#endif