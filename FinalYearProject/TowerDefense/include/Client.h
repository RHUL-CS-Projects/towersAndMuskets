#ifndef CLIENT_H
#define CLIENT_H

#include <enet/enet.h>
#include <string>

/**
 * A network client that can be used to connect to a server and send/receive
 * messages using the ENet library
 */
class Client {
private:
	// IP Address of the server
	ENetAddress serverAddress;
	
	// The client itself
	ENetHost* client;
	
	// Peer that stores data about the server
	ENetPeer* server;

	// Connection status of the client
	bool connected;
	
	/**
	 * Listens for server activity
	 */
	void listen();
public :
	Client();
	~Client();
	
	/**
	 * Attempts to establish a connection with a server on a given address
	 * and port
	 */
	void connectToServer(std::string address, enet_uint16 port = 25565);
	
	/**
	 * Shuts down the connection to the server
	 */
	void disconnect();
	
	/**
	 * Begins a new thread to call the listen method
	 */
	void beginListen();
	
	/**
	 * A helper method to synchronise clock times with the server.
	 * Currently not in a working state
	 */
	void syncClockWithServer();
	
	/**
	 * Returns whether or not the client is currently connected
	 * to the server
	 */
	bool isConnected();
};

#endif