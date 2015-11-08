#ifndef CLIENT_H
#define CLIENT_H

#include <enet/enet.h>
#include <string>

class Client {
private:
	ENetAddress serverAddress;
	ENetHost* client;
	ENetPeer* server;
	bool connected;
	
	void listen();
public :
	Client();
	~Client();
	void connectToServer(std::string address, enet_uint16 port = 25565);
	void disconnect();
	void beginListen();
	void syncClockWithServer();
	
	bool isConnected();
};

#endif