#include <Server.h>
#include <iostream>
#include <pthread.h>
#include <thread>
#include <sfml/SFML/Network.hpp>

using namespace std;

Server::Server() {
	maxClients = 16;
	running = false;
}

Server::~Server() {
	if (isRunning()) {
		stopServer();
	}
}

void Server::startServer(enet_uint16 port) {
	address.host = ENET_HOST_ANY;
	address.port = port;
	
	cout << "Starting server..." << endl;
	server = enet_host_create(&address, maxClients, 2, 0, 0);
	
	if (!server) {
		cout << "There was an error starting the server!" << endl;
		return;
	}
	
	cout << "Server started" << endl;
	cout << "Server public address: " << sf::IpAddress::getPublicAddress().toString() << endl;
	cout << "Server local address : " << sf::IpAddress::getLocalAddress().toString() << endl;
	cout << "Server port          : " << port << endl;
	running = true;
	beginListen();
}

void Server::beginListen() {
	thread listenThread(&Server::listen, this);
	listenThread.join();
}

void Server::listen() {
	cout << "Listening for client activity..." << endl;
	
	ENetEvent receivedEvent;
	
	while (isRunning()) {
		if (enet_host_service(server, &receivedEvent, 0) > 0) {
			char buf[60];
			enet_address_get_host_ip(&receivedEvent.peer->address, buf, 20);
			cout << "Packet received from: " << buf << endl;
			switch (receivedEvent.type) {
				case ENET_EVENT_TYPE_CONNECT:
					cout << "Packet type         : Client connected" << endl;
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					cout << "Packet type         : Client disconnected" << endl;
					break;
			}
			cout << endl;
		}
	}
	
	cout << "Finished listening" << endl;
}

void Server::stopServer() {
	running = false;
	cout << "Stopping server..." << endl;
	enet_host_destroy(server);
	cout << "Server stopped" << endl;
}

/**
 * Check if the server is running
 */
bool Server::isRunning() {
	return running;
}



