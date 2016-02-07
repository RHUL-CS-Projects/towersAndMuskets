#ifndef GAMERESOURCES_H
#define GAMERESOURCES_H

#include <sfml/SFML/Audio.hpp>
#include <list>
#include <map>
#include <string>

class GameResources {
private:
	std::list<sf::SoundBuffer*> soundBuffers;
	std::map<std::string, sf::Sound*> loadedSounds;
public:
	
	sf::Sound* loadSound(std::string sound);
	void freeSounds();
};

#endif