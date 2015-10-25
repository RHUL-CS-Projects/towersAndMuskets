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
	void startServer(enet_uint16 port = 25565);
	void stopServer();
	void beginListen();
	void syncClockReply(ENetPeer* peer);
	
	bool isRunning();
};