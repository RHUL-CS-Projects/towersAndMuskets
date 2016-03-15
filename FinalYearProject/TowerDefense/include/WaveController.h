#ifndef WAVECONTROLLER_H
#define WAVECONTROLLER_H

class WaveController {
private:
	bool inWave;
	int waveNumber;
	double wavePercent;
	double waitPercent;
	double waveSpeed;
	double waitSpeed;
	
	void updateWait();
	void updateWave();
public:
	WaveController();
	
	void init();
	void update();
	double getPercentage();
};

#endif