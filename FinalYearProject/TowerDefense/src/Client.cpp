#include <Client.h>
#include <iostream>
#include <string>
#include <sfml/SFML/Network.hpp>
#include <pthread.h>
#include <thread>

using namespace std;

Client::Client() {
	connected = false;
}

Client::~Client() {
	if (isConnected()) {
		disconnect();
	}
}

/**
 * Connect to a server on a specific address and port.
 */
void Client::connectToServer ( string address, enet_uint16 port ) {
	serverAddress.port = port;
	
	// Resolve string address to an actual host
	if (enet_address_set_host(&serverAddress, address.c_str()) < 0) {
		cout << "Invalid server address" << endl;
		return;
	}
	
	// Create the client with 2 channels and no bandwidth limit
	client = enet_host_create(NULL, 1, 2, 0, 0);
	
	if (!client) {
		cout << "There was an error starting the client!" << endl;
		return;
	}
	
	cout << "Client started" << endl;
	cout << "Client public address: " << sf::IpAddress::getPublicAddress().toString() << endl;
	cout << "Client local address : " << sf::IpAddress::getLocalAddress().toString() << endl;
	cout << "Client port          : " << port << endl;
	
	// Connect to the server
	cout << "Connecting to server at " << address << "..." << endl;
	server = enet_host_connect(client, &serverAddress, 2, 0);
	
	// Wait for server to acknowledge connection
	ENetEvent event;
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		cout << "Connected successfully" << endl;
	} else {
		cout << "Could not connect to server!" << endl;
		return;
	}
	
	connected = true;
	
	// Sync clocks with the server
	syncClockWithServer();
	
	// Begin listening for packets
	beginListen();
}

/**
 * Start a new thread to listen for packets
 */
void Client::beginListen() {
	thread listenThread(&Client::listen, this);
	listenThread.join();
}

/**
 * Use ENet's host service to receive packets from the server
 */
void Client::listen() {
	cout << "Listening for server activity..." << endl;
	
	ENetEvent receivedEvent;
	
	while (isConnected()) {
		if (enet_host_service(client, &receivedEvent, 0) > 0) {
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

/**
 * Close the connection and destroy the client
 */
void Client::disconnect() {
	if (isConnected()) {
		cout << "Stopping client..." << endl;
		enet_peer_disconnect(server, 0);
		enet_host_flush(client);
		connected = false;
		enet_host_destroy(client);
		cout << "Client stopped" << endl;
	}
}

/**
 * Synchronise ENet time with the server's time
 */
void Client::syncClockWithServer() {
	// Ping the server a few times
	for (int i = 0; i < 5; ++i) {
		enet_peer_ping(server);
		cout << "Average round trip time: " << server->roundTripTime << endl;
	}
	
	
}


/**
 * Returns whether or not the client is currently connected to a server
 */
bool Client::isConnected() {
	return connected;
}










