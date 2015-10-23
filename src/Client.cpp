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

void Client::connectToServer ( string address, enet_uint16 port ) {
	serverAddress.port = port;
	
	if (enet_address_set_host(&serverAddress, address.c_str()) < 0) {
		cout << "Invalid server address" << endl;
		return;
	}
	
	client = enet_host_create(NULL, 1, 2, 0, 0);
	
	if (!client) {
		cout << "There was an error starting the client!" << endl;
		return;
	}

	cout << "Client started" << endl;
	cout << "Client public address: " << sf::IpAddress::getPublicAddress().toString() << endl;
	cout << "Client local address : " << sf::IpAddress::getLocalAddress().toString() << endl;
	cout << "Client port          : " << port << endl;
	
	cout << "Connecting to server at " << address << "..." << endl;
	ENetPeer* server = enet_host_connect(client, &serverAddress, 2, 0);
	
	ENetEvent event;
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		cout << "Connected successfully" << endl;
	} else {
		cout << "Could not connect to server!" << endl;
		return;
	}
	
	connected = true;
	beginListen();
}

void Client::beginListen() {
	thread listenThread(&Client::listen, this);
	listenThread.join();
}

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

void Client::disconnect() {
	connected = false;
	cout << "Stopping client..." << endl;
	enet_host_destroy(client);
	cout << "Client stopped" << endl;
}

bool Client::isConnected() {
	return connected;
}
