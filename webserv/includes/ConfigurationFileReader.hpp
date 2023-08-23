#ifndef CONFIGURATIONFILEREADER
#define CONFIGURATIONFILEREADER

#include <iostream>
#include <iomanip>
#include <fstream>
#include <istream>

class ConfigurationFileReader {
    public:
    ConfigurationFileReader();
	~ConfigurationFileReader();
   static bool readLineByLine(std::string configFileName, std::string line);
};

#endif