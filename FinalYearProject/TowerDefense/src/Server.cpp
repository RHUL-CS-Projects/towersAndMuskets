#include <Server.h>
#include <iostream>
#include <pthread.h>
#include <thread>

using namespace std;

Server::Server() {
	maxClients = 16;
}

Server::~Server() {
	if (isRunning()) {
		stopServer();
	}
}

void Server::startServer(enet_uint16 port = 25565) {
	cout << "Server address: " << ENET_HOST_ANY << endl;
	cout << "Server port   : " << port << endl;
	address.host = ENET_HOST_ANY;
	address.port = port;
	
	cout << "Starting server..." << endl;
	server = enet_host_create(&address, maxClients, 2, 0, 0);
	
	if (!server) {
		cout << "There was an error starting the server!" << endl;
		return;
	}
	
	cout << "Server started" << endl;
	beginListen();
	running = true;
}

void Server::beginListen() {
	thread listenThread(&Server::listen, this);
	listenThread.join();
}

void Server::listen() {
	cout << "Listening for client activity..." << endl;
	
	ENetEvent event;
	
	while (enet_host_service(server, &event, 1000)) {
		
	}
	
	cout << "Finished listening" << endl;
}

void Server::stopServer() {
	running = false;
	std::cout << "Stopping server..." << std::endl;
	enet_host_destroy(server);
	std::cout << "Server stopped" << std::endl;
}

/**
 * Check if the server is running
 */
bool Server::isRunning() {
	return running;
}



