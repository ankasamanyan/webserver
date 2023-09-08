#include "../includes/Server.hpp"

#include <string>
#include <iostream>

Server::Server(){};
Server::Server(Configuration configuration){
    configuration.getDefaultConfiguration();

}


Server::~Server(){}