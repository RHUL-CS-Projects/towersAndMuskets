#include <WaveController.h>

WaveController::WaveController() {
	waitSpeed = 1.0/(60*120);
}

double WaveController::getPercentage() {
	return (inWave) ? wavePercent : waitPercent;
}

void WaveController::init() {
	wavePercent = 0;
	waitPercent = 0;
	inWave = false;
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
}

void WaveController::updateWave() {
	
}

