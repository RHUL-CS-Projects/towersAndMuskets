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
#include <TownhallComponent.h>
#include <AnimatorComponent.h>
#include <RTSLogicComponent.h>
#include <RTSVillagerLogicComponent.h>
#include <HealthComponent.h>
#include <ResourceComponent.h>
#include <TeamComponent.h>
#include <SpawnLocationComponent.h>
#include <RTSAIComponent.h>
#include <irrlicht/irrlicht.h>
#include <sfml/SFML/Audio.hpp>
#include <string>
#include <Enums.h>

class ObjectFactory {
public:
	static int addRock(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		objmgr->attachComponent(id, new StaticMeshComponent("rock.x", "RockTexture.png", irr::core::vector3df(-90,0,0)));
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new ResourceComponent(STONE));
		
		return id;
	}
	
	static int addGold(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		objmgr->attachComponent(id, new StaticMeshComponent("rock.x", "GoldTexture.png", irr::core::vector3df(-90,0,0)));
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new ResourceComponent(GOLD));
		
		return id;
	}
	
	static int addTree(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		objmgr->attachComponent(id, new StaticMeshComponent("tree.x", "TreeTexture.png", irr::core::vector3df(-90,0,0)));
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new ResourceComponent(WOOD));
		
		return id;
	}
	
	static int addSpawnLocation(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		objmgr->attachComponent(id, new StaticMeshComponent("flag.x", "FlagTexture.png", irr::core::vector3df(-90,0,0)));
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new SpawnLocationComponent());
		
		return id;
	}
	
	static int addTower(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		objmgr->attachComponent(id, new StaticMeshComponent("tower2.x", "TowerTexture.png", irr::core::vector3df(-90,0,0)));
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new TowerComponent());
		
		return id;
	}
	
	static int addTownhall(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		objmgr->attachComponent(id, new StaticMeshComponent("townhall.x", "TownhallTexture.png", irr::core::vector3df(-90,0,0)));
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new HealthComponent(20, 20));
		objmgr->attachComponent(id, new TeamComponent(id, 0));
		objmgr->attachComponent(id, new TownhallComponent());
		objmgr->attachComponent(id, new SelectableComponent(10, 10));
		
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
		objmgr->attachComponent(id, new RTSAIComponent(1, Game::game.resources.loadSound("musketshot.ogg"), Game::game.resources.loadSound(" "), 60));
		objmgr->attachComponent(id, new SteeringComponent(0.2, 80));
		objmgr->attachComponent(id, new HealthComponent(10, 10));
		objmgr->attachComponent(id, new TeamComponent(id, 1));
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
		objmgr->attachComponent(id, new RTSLogicComponent(1, Game::game.resources.loadSound("musketshot.ogg"), Game::game.resources.loadSound("unitmove.ogg"), 60, true));
		objmgr->attachComponent(id, new SteeringComponent(0.2, 80));
		objmgr->attachComponent(id, new HealthComponent(10, 10));
		objmgr->attachComponent(id, new TeamComponent(id, 0));
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
		animComp->addAnimation("IDLE", 0, 28, 10);
		animComp->addAnimation("WALK", 30, 189, 30);
		animComp->addAnimation("TAKEAIM", 190, 219, 30);
		animComp->addAnimation("AIM", 221, 230, 30);
		animComp->addAnimation("SHOOT", 232, 263, 30);
		animComp->addAnimation("RELOAD", 266, 576, 30);
		animComp->addAnimation("REST", 580, 609, 30);
		animComp->addAnimation("DEATH1", 612, 623, 20);
		
		objmgr->attachComponent(id, animComp);
		
		objmgr->attachComponent(id, new SelectableComponent(3,3));
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new FaceDirectionComponent(0, 0.08f));
		objmgr->attachComponent(id, new RTSLogicComponent(1, Game::game.resources.loadSound("cannonfire.ogg"), Game::game.resources.loadSound("cannonmove.ogg"), 60, false, 10, 15));
		objmgr->attachComponent(id, new SteeringComponent(0.1, 80, 8));
		objmgr->attachComponent(id, new HealthComponent(10, 10));
		objmgr->attachComponent(id, new TeamComponent(id, 0));
	}
	
	static int addPlayerVillager(irr::core::vector3df pos) {
		ObjectManager* objmgr = Game::game.getObjMgr();
		int id = objmgr->createObject();
		
		objmgr->attachComponent(id, new TransformComponent(pos));
		
		objmgr->attachComponent(id, new AnimatedMeshComponent("villager.x", "VillagerTexture.png",irr::core::vector3df(0,0,0)));
		
		AnimatorComponent* animComp = new AnimatorComponent();
		animComp->addAnimation("IDLE", 0, 69, 30);
		animComp->addAnimation("WALK", 71, 100, 30);
		animComp->addAnimation("CHOP", 101, 136, 30);
		animComp->addAnimation("DIG", 138, 182, 30);
		animComp->addAnimation("DEATH1", 184, 214, 20);
		
		objmgr->attachComponent(id, animComp);
		
		objmgr->attachComponent(id, new SelectableComponent());
		objmgr->attachComponent(id, new RenderComponent(true));
		objmgr->attachComponent(id, new FaceDirectionComponent(0, 0.08f));
		objmgr->attachComponent(id, new RTSVillagerLogicComponent(5, 26));
		objmgr->attachComponent(id, new SteeringComponent(0.2, 80));
		objmgr->attachComponent(id, new HealthComponent(10, 10));
		objmgr->attachComponent(id, new TeamComponent(id, 0));
	}
};

#endif