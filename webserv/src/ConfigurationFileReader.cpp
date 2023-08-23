#include "../includes/ConfigurationFileReader.hpp"

ConfigurationFileReader::ConfigurationFileReader(){}

ConfigurationFileReader::~ConfigurationFileReader(){}

bool ConfigurationFileReader::readLineByLine(std::string configFileName, std::string line){
    std::ifstream configFile(configFileName);
    // std::cout <<std::getline (configFile,line)<<std::endl;
    while (std::getline (configFile,line))
        return true;
    return false;

}
