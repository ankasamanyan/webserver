#ifndef SERVERSREPOSITORY_HPP
#define SERVERSREPOSITORY_HPP

#include <stdlib.h>
#include <deque>
#include <iostream>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "Server.hpp"

    class ServersRepository {

        public:
        
			std::deque<Server> servers;

            ServersRepository();
            ServersRepository(Server server);
            ~ServersRepository();
            std::deque<Server> getServers();
            // void addServer(Server server);

			
    };

    #endif