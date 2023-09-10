
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <iomanip>
#include <map>
#include <unordered_map>

#include "./includes/Configuration.hpp"



int main(int argc, char** argv){
    Configuration *conf = new Configuration(argc, argv);
//for test
    std::cout << conf->serverRepo[0].serverName << std::endl;
    std::cout << " Location Dir for root = " << conf->serverRepo[0].locations["root"].locationDir << std::endl;
    std::cout << " Location Dir for dir server 1= " << conf->serverRepo[0].locations["dir"].locationDir << std::endl;
    std::cout << " Location Dir for dir server 2 should be empty = " << conf->serverRepo[1].locations["dir"].locationDir << std::endl;
    std::cout << " Default File for dir server 2 should be empty = " << conf->serverRepo[1].locations["dir"].defaultFile << std::endl;
    std::cout << " Name server 2 = " << conf->serverRepo[1].serverName << std::endl;
    std::cout << " Error from server 2 = " << conf->serverRepo[1].error404 << std::endl;
     std::map<std::string, std::string>::iterator it = conf->serverRepo[0].errorPages.begin();
     while (it != conf->serverRepo[0].errorPages.end())
  {
    // std::cout << "**********Am I here?***********" << std::endl;
    std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    it++;
  }
  //end of test  
}
