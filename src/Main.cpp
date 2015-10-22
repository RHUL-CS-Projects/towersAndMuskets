#include <iostream>
#include <Server.h>

using namespace std;

int main() {
	enet_uint16 port;
	
    cout << "Enter a port: ";
	cin >> port;
	
	Server server;
	server.startServer(port);
	
	return 0;
}

























