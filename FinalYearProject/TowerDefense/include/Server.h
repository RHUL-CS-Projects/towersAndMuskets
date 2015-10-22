#include <enet/enet.h>

class Server {
private:
	ENetAddress address;
	ENetHost* server;
	bool running;
	int maxClients;
	
	void listen();
public :
	Server();
	~Server();
	void startServer(enet_uint16 port);
	void stopServer();
	void beginListen();
	
	bool isRunning();
};