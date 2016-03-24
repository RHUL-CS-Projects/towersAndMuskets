#ifndef WAVECONTROLLER_H
#define WAVECONTROLLER_H

#include <vector>
#include <irrlicht/irrlicht.h>

struct SpawnLocation {
	int id;
	irr::core::vector3df location;
	int leftToSpawn;
};

class WaveController {
private:
	std::vector<SpawnLocation> spawnLocations;
	std::vector<int> enemies;
	
	bool inWave;
	int waveNumber;
	double wavePercent;
	double waitPercent;
	double waveSpeed;
	double waitSpeed;
	
	int killed = 0;
	int spawnNumber = 5;
	int toSpawn = 0;
	int spawnTimeCount = 0;
	int spawnDelay = 60;
	
	void spawnUnit(irr::core::vector3df position);
	void spawnWave();
	void updateWait();
	void updateWave();
	void pickSpawnLocations();
	void hideAllSpawnLocations();
	void showCurrentSpawnLocations();
	void updateUnitsLeft();
	
	bool unitAlive(int id);
public:
	WaveController();
	
	void init();
	void update();
	double getPercentage();
	void skipWait();
};

#endif