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

void WaveController::init() {
	wavePercent = 0;
	waitPercent = 0;
	waveNumber = 5;
	inWave = false;
	victory = true;
	gameOver = false;
	
	pickSpawnLocations();
}

void WaveController::update() {
	if (inWave) {
		updateWave();
	} else {
		updateWait();
	}
	
	if (TeamComponent::getObjectsOnTeam(0).size() == 0) {
		victory = false;
		gameOver = true;
	}
	
	if (waveNumber > numWaves) {
		gameOver = true;
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
		spawnNumberUnits += 4 + rand()%3;
		spawnNumberCannons += 1 + rand()%2;
		
		waveNumber++;
		
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
	}
	
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
	int id;
	
	if (cannonsLeft > 0 && unitsLeft > 0) {
		if (rand()%2) {
			id = ObjectFactory::addEnemyUnit(position);
			unitsLeft--;
		} else {
			cannonsLeft--;
			id = ObjectFactory::addEnemyCannon(position);
		}
	} else if (cannonsLeft > 0) {
		cannonsLeft--;
		id = ObjectFactory::addEnemyCannon(position);
	} else {
		unitsLeft--;
		id = ObjectFactory::addEnemyUnit(position);
	}
	
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
	
	int unitsPerSpawn = 4;
	std::list<int> spawns  = Game::game.getObjMgr()->getObjectsWithComponent("SpawnLocationComponent"); 
	std::vector<int> spawnsVec { spawns.begin(), spawns.end() };
	
	spawnNumber = spawnNumberCannons + spawnNumberUnits;
	
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
	
	unitsLeft = spawnNumberUnits;
	cannonsLeft = spawnNumberCannons;
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

WaveDetails WaveController::getWaveDetails() {
	WaveDetails details;
	details.cannonsLeft = spawnNumberCannons;
	details.unitsLeft = spawnNumberUnits;
	details.waveNumber = waveNumber;
	details.percent = (inWave) ? wavePercent : waitPercent;
	details.inWave = inWave;
	details.gameOver = gameOver;
	details.victory = victory;
	
	return details;
}





