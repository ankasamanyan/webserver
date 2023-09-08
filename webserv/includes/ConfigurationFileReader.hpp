#ifndef CONFIGURATIONFILEREADER
#define CONFIGURATIONFILEREADER

#include <iostream>
#include <iomanip>
#include <fstream>
#include <istream>
#include "ConfigurationFileParser.hpp"
#include "Configuration.hpp"

class ConfigurationFileReader {
    public:

    ConfigurationFileReader();
    ConfigurationFileReader(std::string configFile, Configuration config);
	~ConfigurationFileReader();
};

#endif