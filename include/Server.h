#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>

/**
 * A network server that can be used to handle several client connections
 * and send/receive messages between them using the ENet library
 */
class Server {
private:
	// Listen address of the server
	ENetAddress address;
	
	// The server itself
	ENetHost* server;
	
	// Whether or not the server is currently running
	bool running;
	
	// The maximum number of clients this server can connect to
	int maxClients;
	
	/**
	 * Listens for client activity
	 */
	void listen();
public :
	Server();
	~Server();
	
	/**
	 * Binds the server to the listen address and port and then begins
	 * listening for client activity
	 */
	void startServer(enet_uint16 port = 25565);
	
	/**
	 * Stops the server and closes connections to clients
	 */
	void stopServer();
	
	/**
	 * Begins a new thread to call the listen method
	 */
	void beginListen();
	
	/**
	 * A helper method to synchronise clock times with clients.
	 * Currently not in a working state
	 */
	void syncClockReply(ENetPeer* peer);
	
	/**
	 * Returns whether or not the server is currently running
	 */
	bool isRunning();
};

#endif