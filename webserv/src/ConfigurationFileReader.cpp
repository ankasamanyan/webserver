#include "../includes/ConfigurationFileReader.hpp"

#include <string>
#include <iostream>

ConfigurationFileReader::ConfigurationFileReader(){};
ConfigurationFileReader::ConfigurationFileReader(std::string configFile, Configuration config){
    (void)config;
    ConfigurationFileParser parser(Configuration config);
    std::fstream file(configFile);
    std::string line = "default";
    if (file.fail())
        throw std::out_of_range("failed to open a file");
        while (std::getline (file,line) )
        {
            
            std::cout << line << '\n';
        }
        file.close();
}

ConfigurationFileReader::~ConfigurationFileReader(){}

