#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdlib.h>

#include <iostream>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "Configuration.hpp"
#include "ConfigurationFileReader.hpp"

    class Server {

        public:
			Configuration configuration;

            Server();
            Server(Configuration configuration);
            ~Server();
			
    };

    #endif