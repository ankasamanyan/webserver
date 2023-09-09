
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <iomanip>

#include "./includes/ServersRepository.hpp"

int main(int argc, char** argv){
    Configuration *conf = new Configuration(argc, argv);
    ServersRepository *serversRepository = new ServersRepository(*conf);
    
    std::cout << serversRepository->servers[0].getConfigurationFromConfigFile().port << std::endl;
    
}
