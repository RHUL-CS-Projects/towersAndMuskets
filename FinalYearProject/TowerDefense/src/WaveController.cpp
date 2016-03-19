#include <WaveController.h>
#include <Game.h>
#include <StatePlaying.h>
#include <irrlicht/irrlicht.h>
#include <TowerDefenseEngine.h>
#include <algorithm>

using namespace irr;
using namespace core;

WaveController::WaveController() {
	waitSpeed = 1.0/(60*120);
}

double WaveController::getPercentage() {
	return (inWave) ? wavePercent : waitPercent;
}

void WaveController::init() {
	wavePercent = 0;
	waitPercent = 0;
	waveNumber = 0;
	inWave = false;

	pickSpawnLocations();
}

void WaveController::update() {
	if (inWave) {
		//updateWave();
	} else {
		updateWait();
	}
}

void WaveController::updateWait() {
	waitPercent += waitSpeed;
	
	if (waitPercent >= 1) {
		((StatePlaying*)Game::game.currentState())->message(SHOW_MESSAGE_GOOD, "Wave Started!");
		inWave = true;
		toSpawn = spawnNumber;
		spawnTimeCount = spawnDelay;
		Game::game.resources.loadSound("battlehorn.ogg")->play();
	}
}

void WaveController::updateWave() {
	spawnWave();
}

void WaveController::skipWait() {
	if (!inWave) waitSpeed = 1;
}

void WaveController::spawnWave() {
	spawnTimeCount--;
	
	if (spawnTimeCount <= 0) {
		if (toSpawn > 0) {
			toSpawn--;
			
			std::random_shuffle(spawnLocations.begin(), spawnLocations.end());
			
			SpawnLocation* current = &spawnLocations.back();
			
			if (current->leftToSpawn > 0) {
				current->leftToSpawn--;
				spawnUnit(current->location);
			} else {
				spawnLocations.pop_back();
			}
		}
		
		spawnTimeCount = spawnDelay;
	}
}

void WaveController::spawnUnit ( vector3df position ) {
	int id = ObjectFactory::addEnemyUnit(position);
}

void WaveController::hideAllSpawnLocations() {
	std::list<int> spawns  = Game::game.getObjMgr()->getObjectsWithComponent("SpawnLocationComponent");
	for (int i : spawns) {
		((RenderComponent*)Game::game.getObjMgr()->getObjectComponent<RenderComponent>(i, "RenderComponent"))->visible = false;
	}
}

void WaveController::showCurrentSpawnLocations() {
	for (SpawnLocation s : spawnLocations) {
		((RenderComponent*)Game::game.getObjMgr()->getObjectComponent<RenderComponent>(s.id, "RenderComponent"))->visible = true;
	}
}

void WaveController::pickSpawnLocations() {
	hideAllSpawnLocations();
	spawnLocations.clear();
	
	// - Pick enough spawn points for all units to be spawned
	int unitsPerSpawn = 5;
	std::list<int> spawns  = Game::game.getObjMgr()->getObjectsWithComponent("SpawnLocationComponent"); 
	std::vector<int> spawnsVec { spawns.begin(), spawns.end() };
	
	std::random_shuffle(spawnsVec.begin(), spawnsVec.end());
	
	int numSpawnPoints = (int)ceil(spawnNumber/unitsPerSpawn);
	
	while (numSpawnPoints > 0 && spawnsVec.size() > 0) {
		numSpawnPoints--;
		
		SpawnLocation s;
		s.id = spawnsVec.back();
		spawnsVec.pop_back();
		
		s.location = ((TransformComponent*)Game::game.getObjMgr()->getObjectComponent<TransformComponent>(s.id, "TransformComponent"))->worldPosition;
		s.leftToSpawn = unitsPerSpawn;
		
		spawnLocations.push_back(s);
	}
	
	showCurrentSpawnLocations();
}








