#include <GameResources.h>
#include <iostream>

sf::Sound* GameResources::loadSound ( std::string sound ) {
	if (loadedSounds.find(sound) == loadedSounds.end()) {
		std::cout << "loading " << sound << std::endl;
		
		sf::SoundBuffer* buffer = new sf::SoundBuffer();
		
		if (!buffer->loadFromFile("res/sounds/" + sound)) 
			std::cout << "sound not loaded" << std::endl;
		
		sf::Sound* loadedSound = new sf::Sound(*buffer);
		soundBuffers.push_back(buffer);
				loadedSound->setVolume(10);
		loadedSounds.insert(std::pair<std::string, sf::Sound*>(sound, loadedSound));
	}
	
	return loadedSounds.at(sound);
}

void GameResources::freeSounds() {
	for (std::pair<std::string, sf::Sound*> p : loadedSounds) {
		delete p.second;
	}
	
	for (sf::SoundBuffer* b : soundBuffers) {
		delete b;
	}
}
