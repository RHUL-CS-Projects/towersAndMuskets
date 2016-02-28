#include <NoiseGenerator.h>
#include <random>
#include <iostream>

/**
 * Code adapted from How to Use Perlin Noise in Your Games by Herman Tulleken,
 * http://devmag.org.za/2009/04/25/perlin-noise/
 */

void NoiseGenerator::generatePerlinNoise ( int seed, int width, int height ) {
	this->width = width;
	this->height = height;
	srand(seed);
	
	generatedNoise = getArray();
	
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			generatedNoise[x][y] = 0;
		}
	}
	
	int numOctaves = 8;
	
	float*** octaves = new float**[numOctaves];
	for (int i = 0; i < numOctaves; i++) {
		octaves[i] = new float*[width];
		
		for (int j = 0; j < width; j++) {
			octaves[i][j] = new float[height];
		}
	}
	
	float** baseNoise = generateWhiteNoise();
	
	for (int i = 0; i < numOctaves; i++) {
		octaves[i] = generateSmoothNoise(baseNoise, i);
	}
	
	float amplitude = 1.0f;
	float totalAmplitude = 0.0f;
	float persistence = 0.5f;
	
	for (int i = numOctaves - 1; i >= 0; i--) {
		amplitude *= persistence;
		totalAmplitude += amplitude;
		
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				generatedNoise[x][y] += octaves[i][x][y] * amplitude;
			}
		}
	}
	
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			generatedNoise[x][y] /= totalAmplitude;
		}
	}
}

float** NoiseGenerator::generateSmoothNoise ( float** baseNoise, int octave ) {
	float** smoothed = getArray();
	
	int samplePeriod = 1 << octave;
	float sampleFrequency = 1.0f / samplePeriod;
	
	for (int i = 0; i < width; i++) {
		int sample_i0 = (i / samplePeriod) * samplePeriod;
		int sample_i1 = (sample_i0 + samplePeriod) % width;
		float horizBlend = (i - sample_i0) * sampleFrequency;
			
		for (int j = 0; j < height; j++) {
			int sample_j0 = (j / samplePeriod) * samplePeriod;
			int sample_j1 = (sample_j0 + samplePeriod) % height;
			float vertBlend = (j - sample_j0) * sampleFrequency;
			
			float top = lerp(baseNoise[sample_i0][sample_j0], baseNoise[sample_i1][sample_j0], horizBlend);
			float bottom = lerp(baseNoise[sample_i0][sample_j1], baseNoise[sample_i1][sample_j1], horizBlend);
			
			smoothed[i][j] = lerp(top, bottom, vertBlend);
		}
	}
	
	return smoothed;
}

float** NoiseGenerator::generateWhiteNoise () {
	float** noise = getArray();
	
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			noise[i][j] = ((float)rand()) / (float) RAND_MAX;
		}
	}
	
	return noise;
}

float NoiseGenerator::getNoiseAt ( int x, int y ) {
	std::cout << "Getting noise at " << x << ", " << y << " = " << generatedNoise[x%width][y%height] << std::endl;
	return generatedNoise[x%width][y%height];
}

float NoiseGenerator::lerp ( float a, float b, float c ) {
	return a + (b - a) * c;
}

float** NoiseGenerator::getArray() {
	float** array = new float*[width];
	
	for (int i = 0; i < width; i++) {
		array[i] = new float[height];
	}
	
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			array[i][j] = 0;
		}
	}
	
	return array;
}

