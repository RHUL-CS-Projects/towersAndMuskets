#ifndef WAVECONTROLLER_H
#define WAVECONTROLLER_H

class WaveController {
private:
	bool inWave;
	int waveNumber;
	
public:
	WaveController() {
		
	}
	
	void update();
	double getWaitPercentage();
	double getWavePercentage();
};

#endif