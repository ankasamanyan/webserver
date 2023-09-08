#include <iostream>
#include <iostream>
#include <fstream>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "./includes/Configuration.hpp"
// #include "./includes/ConfigurationFileReader.hpp"
#include "./includes/Server.hpp"
#include "./includes/ServersRepository.hpp"

int main(int argc, char** argv){
    Configuration *conf = new Configuration();
    Server *server = new Server();
    ServersRepository *serversRepository = new ServersRepository(*server);
    
    std::ifstream configFile(argv[1]);
    std::string line = "default";
    
    if(argc == 1)
    {
        conf->getDefaultConfiguration();
    }
    
    else     if (argc == 2)
    {
        try {
            std::cout << serversRepository->servers[0].configuration.getConfigurationFromConfigFile().port << std::endl;
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    else
        std::cout << "Wrong number of arguments" << std::endl;
}
