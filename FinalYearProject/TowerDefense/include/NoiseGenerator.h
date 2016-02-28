#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

class NoiseGenerator {
private:
	float** generateWhiteNoise();
	float** generateSmoothNoise(float** baseNoise, int octave);
	float lerp(float a, float b, float c);
	
	float** getArray();
	
	int width;
	int height;
	
	float** generatedNoise;
public:
	void generatePerlinNoise(int seed, int width, int height);
	float getNoiseAt(int x, int y);
};

#endif