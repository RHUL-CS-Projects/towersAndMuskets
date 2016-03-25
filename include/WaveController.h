#ifndef WAVECONTROLLER_H
#define WAVECONTROLLER_H

#include <vector>
#include <irrlicht/irrlicht.h>

struct SpawnLocation {
	int id;
	irr::core::vector3df location;
	int leftToSpawn;
};

struct WaveDetails {
	int waveNumber;
	int cannonsLeft;
	int unitsLeft;
	double percent;
	bool inWave;
};

class WaveController {
private:
	std::vector<SpawnLocation> spawnLocations;
	std::vector<int> enemies;
	
	bool inWave;
	int waveNumber = 1;
	double wavePercent;
	double waitPercent;
	double waveSpeed;
	double waitSpeed;
	
	int killed = 0;
	int spawnNumber = 0;
	int spawnNumberUnits = 5;
	int spawnNumberCannons = 0;
	int unitsLeft = 0;
	int cannonsLeft = 0;
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
	WaveDetails getWaveDetails();
	void skipWait();
};

#endif