#include <WaveController.h>
#include <Game.h>
#include <StatePlaying.h>
#include <irrlicht/irrlicht.h>
#include <TowerDefenseEngine.h>
#include <algorithm>
#include <time.h>

using namespace irr;
using namespace core;

WaveController::WaveController() {
	waitSpeed = 1.0/(60*200);
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
		updateWave();
	} else {
		updateWait();
	}
}

void WaveController::updateWait() {
	waitPercent += waitSpeed;
	
	if (waitPercent >= 1) {
		((StatePlaying*)Game::game.currentState())->message(SHOW_MESSAGE_GOOD, "Wave Started!");
		waitPercent = 0;
		inWave = true;
		toSpawn = spawnNumber;
		spawnTimeCount = spawnDelay;
		Game::game.resources.loadSound("battlehorn.ogg")->play();
	}
}

void WaveController::updateWave() {
	spawnWave();
	updateUnitsLeft();
	wavePercent = 1.0 / (double)spawnNumber * (double)killed;
	
	if (killed >= spawnNumber) {
		((StatePlaying*)Game::game.currentState())->message(SHOW_MESSAGE_GOOD, "Wave Complete!");
		inWave = false;
		killed = 0;
		wavePercent = 0;
		spawnNumber += 5;
		
		pickSpawnLocations();
	}
}

void WaveController::skipWait() {
	if (!inWave) waitPercent = 1;
}

void WaveController::spawnWave() {
	spawnTimeCount--;
	
	int left = 0;
	for (SpawnLocation s : spawnLocations) {
		left += s.leftToSpawn;
		std::cout << s.leftToSpawn << ", ";
	}
	std::cout << std::endl;
	
	if (spawnTimeCount <= 0) {
		if (left > 0) {
			toSpawn--;
			
			//std::random_shuffle(spawnLocations.begin(), spawnLocations.end());
			
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
	enemies.push_back(id);
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
	spawnLocations.clear();
	hideAllSpawnLocations();
	
	int unitsPerSpawn = 2;
	std::list<int> spawns  = Game::game.getObjMgr()->getObjectsWithComponent("SpawnLocationComponent"); 
	std::vector<int> spawnsVec { spawns.begin(), spawns.end() };
	
	srand(time(nullptr));
	
	std::random_shuffle(spawnsVec.begin(), spawnsVec.end());
	
	//int numSpawnPoints = (int)ceil(spawnNumber/unitsPerSpawn);
	int leftToAssign = spawnNumber;
	
	while (leftToAssign > 0 && spawnsVec.size() > 0) {
		//numSpawnPoints--;
		
		SpawnLocation s;
		s.id = spawnsVec.back();
		spawnsVec.pop_back();
		
		s.location = ((TransformComponent*)Game::game.getObjMgr()->getObjectComponent<TransformComponent>(s.id, "TransformComponent"))->worldPosition;
		s.leftToSpawn = std::min(unitsPerSpawn, leftToAssign);
		leftToAssign -= unitsPerSpawn;
		
		spawnLocations.push_back(s);
	}
	std::cout << spawnLocations.size() << std::endl;
	showCurrentSpawnLocations();
}

bool WaveController::unitAlive ( int id ) {
	HealthComponent* healthComp = Game::game.getObjMgr()->getObjectComponent<HealthComponent>(id, "HealthComponent");
	if (healthComp == nullptr || healthComp->health <= 0)
		return false;
	
	return true;
}

void WaveController::updateUnitsLeft() {
	for (auto it = enemies.begin(); it != enemies.end();) {
		if (!unitAlive(*it)) {
			it = enemies.erase(it);
			killed++;
		} else {
			it++;
		}
	}
}






