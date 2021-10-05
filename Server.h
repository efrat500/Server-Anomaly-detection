#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <netinet/in.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "CLI.h"


using namespace std;

class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

class SocketIO:public DefaultIO {
	int adrss_client;
	public:
		SocketIO(int ID) { this->adrss_client = ID;}
		~SocketIO() {};
		virtual string read() {
			string serverInput="";
			char c = 0;
			recv(adrss_client, &c, sizeof(char), 0);
			while(c!='\n') {				
				serverInput+=c;
				recv(adrss_client,&c,sizeof(char) , 0);
			}
	 	return serverInput;
		}
		virtual void write(string text) {send(adrss_client, text.c_str(),text.length(), 0);}
		virtual void write(float f) {}
		virtual void read(float* f) {}

};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{ 
	public:
    virtual void handle(int clientID){
		SocketIO dio(clientID);
		CLI clihandler(&dio);
		clihandler.start();
    }
};


// implement on Server.cpp
class Server {
	int file_descriptor;
  	sockaddr_in server;
 	sockaddr_in client;
	thread* t; 
	bool serverRun;

	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
