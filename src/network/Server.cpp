#include <Server.h>
#include <iostream>
#include <string>
#include <cstring>
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
	
	enet_time_set(0);
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
				case ENET_EVENT_TYPE_RECEIVE:
					string data((char *)receivedEvent.packet->data);
					if (data == "RequestTime") {
						syncClockReply(receivedEvent.peer);
					}
					cout << "Packet data         : " << data << endl;
					break;
			}
			cout << endl;
		}
		
		//cout << enet_time_get() << endl;
	}
	
	cout << "Finished listening" << endl;
}

/**
 * Reply to a peer with the current time for syncing clocks
 */
void Server::syncClockReply ( ENetPeer* peer ) {
	string data = "RequestTime" + to_string(enet_time_get());
	char buffer[data.length()+1];
	strncpy(buffer, data.c_str(), data.length());
	buffer[data.length()] = 0;
	ENetPacket* packet = enet_packet_create(buffer, data.length(), ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
	enet_host_flush(server);
}

void Server::stopServer() {
	if (isRunning()) {
		cout << "Stopping server..." << endl;
		running = false;
		enet_host_destroy(server);
		cout << "Server stopped" << endl;
	}
}

/**
 * Check if the server is running
 */
bool Server::isRunning() {
	return running;
}



