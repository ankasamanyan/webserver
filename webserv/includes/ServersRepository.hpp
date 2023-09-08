#ifndef SERVERSREPOSITORY_HPP
#define SERVERSREPOSITORY_HPP

#include <stdlib.h>
#include <deque>
#include <iostream>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "Configuration.hpp"

    class ServersRepository {

        public:
        
			// std::deque<Server> servers;
            std::deque<Configuration> servers;

            ServersRepository();
            ServersRepository(Configuration server);
            ~ServersRepository();
            std::deque<Configuration> getServers();		
    };

    #endif