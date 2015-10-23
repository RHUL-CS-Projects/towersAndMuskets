#include <iostream>
#include <Server.h>
#include <Client.h>
#include <string>

using namespace std;

int main() {
	enet_uint16 port;
	
	string input;
	
	cout << "Host? y/n: ";
	cin >> input;
	
	if (input[0] == 'y') {
		Server server;
		server.startServer();
	} else {
		cout << "Connect to ip: ";
		cin >> input;
		
		Client client;
		client.connectToServer(input);
	}
	
	return 0;
}

























