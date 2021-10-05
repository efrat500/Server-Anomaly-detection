#include "Server.h"

Server::Server(int port)throw (const char*) {
  this->serverRun = false;
	this->file_descriptor = socket(AF_INET,SOCK_STREAM,0);
    if (file_descriptor < 0)
      throw "socket failed";
    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = INADDR_ANY;
    this->server.sin_port = htons(port);
    if(bind(file_descriptor,(struct sockaddr*) &server, sizeof(server)) < 0)
      throw "bind failure";
    if(listen(file_descriptor, 3) < 0)
      throw "listen failure";
}

void Server::start(ClientHandler& ch)throw(const char*){	
    this->serverRun = true;
    t = new thread([&ch,this]() {
      while(serverRun) {
        socklen_t clientSize = sizeof(client);
        alarm(1);
        int aClient = accept(file_descriptor, (struct sockaddr*) &client, &clientSize);
        alarm(0);
        if(aClient < 0) 
          throw "accept failure";
        ch.handle(aClient);
        close(aClient);
      }
      close(file_descriptor);
    });
      
};

void Server::stop(){
  this->serverRun = false;
	t->join();
}

Server::~Server() {}


