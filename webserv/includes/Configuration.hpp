#ifndef CONFIGURATIONFILE_HPP
#define CONFIGURATIONFILE_HPP

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <istream>
#include <sstream>
#include <map>
#include <deque>

enum environment {
	SERVER,
	OUT_OF_SERVER,
	LOC
};

typedef struct location{
	std::string locationName;
	std::string locationDir; //root, dir, upload
	bool methodGet;
	bool methodPost;
	bool methodDelete;
	std::string defaultFile;
}location;

typedef struct configuration{
    std::string serverName;
	std::string host;
	std::string port;
	std::string maxBody;
	std::string error404;
	bool dirListing;
	std::string CGIDir;
	std::map<std::string, location> locations;
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