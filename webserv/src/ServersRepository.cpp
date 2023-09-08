#include "../includes/ServersRepository.hpp"

#include <string>
#include <iostream>

ServersRepository::ServersRepository(){};


ServersRepository::~ServersRepository(){}

std::deque<Server> ServersRepository::getServers(){
    return servers;
}

// void ServersRepository::addServer(Server server){
//     servers.push_back(server);
// }

ServersRepository::ServersRepository(Server server) {
    servers.push_back(server);
}