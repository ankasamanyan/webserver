#ifndef CONFIGURATIONFILE_HPP
#define CONFIGURATIONFILE_HPP

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <map>
#include <deque>
#include <unordered_map>
#include <memory>
#include <iomanip>

enum environment {
	SERVER,
	OUT_OF_SERVER,
	LOC,
	ERRORS
};

typedef struct location{
	std::string locationName; // do need anymore
	std::string locationDir; //should be key for map // can we make sure it always has "/" at the end?
	// add dirListing right here
	// add redirection (std::string)
	bool methodGet;
	bool methodPost;
	bool methodDelete;
	std::string defaultFile;
}location;

typedef struct configuration{ //can we have a root here?
    std::string serverName;
	std::string host;
	std::string port;
	std::string maxBody;
	std::string error404; //remove
	bool dirListing; //should be moved to location
	std::string CGIDir;
	std::map<std::string, location> locations;
	std::map<std::string, std::string> errorPages;
}configuration;



class Configuration
{
private:
	environment env;
public:	
	configuration config;
	location loc;

	std::deque<configuration> serverRepo;

	Configuration();
	Configuration(int argc, char **argv);
	~Configuration();

	void clearConfiguration();
	void clearLocation();

	void parseConfiguration(std::string configFile);
	// void getDefaultConfiguration();

	void parseServerName(std::string line);
	void parseHost(std::string line);
	void parsePort(std::string line);
	void parseMaxBody(std::string line);
	void parseLocationGet(std::string line);
	void parseLocationPost(std::string line);
	void parseLocationDelete(std::string line);
	void parseErroePage404(std::string line);
	void parseDirectoryListing(std::string line);
	void parseCGIdir(std::string line);
	void parseLocationName(std::string line);
	void parseLocationDir(std::string line);
	void parseLocationDefaultFile(std::string line);
	void parseLocation(std::string line);
	void parseWhateverButNotLocation(std::string line);
	void checkIfInsideOfServer(std::string line);
	void addServerToServerRepo();
	void addLocation();

};


#endif