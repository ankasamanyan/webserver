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


int main(int argc, char** argv){
    Configuration *conf = new Configuration();
    std::ifstream configFile(argv[1]);
    std::string line = "default";
    if(argc == 1)
    {
        conf->getDefaultConfiguration();
        // std::cout << conf->config.port << std::endl;
    }
    
    else if(argc == 2)
    { 
        // if (configFile.is_open())
        // {
        //     while ( std::getline (configFile,line) )
        //     {
        //         std::cout << line << '\n';
        //     }
        //     configFile.close();
        // }
        //TO IMPLEMENT use input file as config file
    }
    else{
        std::cout<<"Too many arguments!" << std::endl; 
    }
}
