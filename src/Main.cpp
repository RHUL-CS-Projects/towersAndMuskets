#include <iostream>
#include <string>
#include <irrlicht/irrlicht.h>
#include <sfml/SFML/Window.hpp>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <ctime>

#include <TowerDefenseEngine.h>
#include <random>
#include <sfml/SFML/Main.hpp>
#include <sfml/SFML/Audio.hpp>

using namespace std;
using namespace chrono;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace sf;

Server server;
Client client;

void signalExit(int s) {
	server.stopServer();
	client.disconnect();
}

void runGame() {
	Game::game.init();
	Game::game.run();
	
	cout << "Game Exited" << endl;
	Game::game.dispose();
}

int main() {
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = signalExit;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	
	sigaction(SIGINT, &sigIntHandler, NULL);
	
	/*enet_uint16 port;
	string input;
	
	cout << "Host? y/n: ";
	cin >> input;
	
	if (input[0] == 'y') {
		server.startServer();
	} else {
		cout << "Connect to ip: ";
		cin >> input;
		
		client.connectToServer(input);
	}*/
	//basicGraphics();
	
	runGame();
	
	return 0;
}







