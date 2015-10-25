#include <iostream>
#include <Server.h>
#include <Client.h>
#include <string>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

Server server;
Client client;

void signalExit(int s) {
	server.stopServer();
	client.disconnect();
}

int main() {
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = signalExit;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	
	sigaction(SIGINT, &sigIntHandler, NULL);
	
	enet_uint16 port;
	string input;
	
	cout << "Host? y/n: ";
	cin >> input;
	
	if (input[0] == 'y') {
		server.startServer();
	} else {
		cout << "Connect to ip: ";
		cin >> input;
		
		client.connectToServer(input);
	}
	
	return 0;
}

























