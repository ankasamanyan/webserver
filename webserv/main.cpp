
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <iomanip>

#include "./includes/ServersRepository.hpp"

int main(int argc, char** argv){
    Configuration *conf = new Configuration();
    ServersRepository *serversRepository = new ServersRepository(*conf);
    
    std::ifstream configFile(argv[1]);
    std::string line = "default";
    
    if(argc == 1)
    {
        conf->getDefaultConfiguration();
    }
    
    else     if (argc == 2)
    {
        try {
            std::cout << serversRepository->servers[0].getConfigurationFromConfigFile().port << std::endl;
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    else
        std::cout << "Wrong number of arguments" << std::endl;
}
