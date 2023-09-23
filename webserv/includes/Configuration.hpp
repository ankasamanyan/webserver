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
//can we also check that <LOC>locationDir: /uploads</LOC> does not have a redirection link inside? 
//since we should only allow our users to post into /uploads directory, 
//we shouldn't allow redirection for this specific location
	std::string locationDir;
	std::string redirection;
	bool dirListing;
	bool methodGet;
	bool methodPost;
	bool methodDelete;
	std::string defaultFile;
	std::string uploadsDir;
}location;

typedef struct configuration{
	std::string root;
    std::string serverName;
	std::string host;
	std::string port;
	std::string maxBody;
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

	void parseServerName(std::string line);
	void parseRoot(std::string line);
	void parseHost(std::string line);
	void parsePort(std::string line);
	void parseMaxBody(std::string line);
	void parseLocationGet(std::string line);
	void parseLocationPost(std::string line);
	void parseLocationDelete(std::string line);
	void parseLocationDirectoryListing(std::string line);
	void parseLocationRedirection(std::string line);
	void parseCGIdir(std::string line);
	void parseLocationDir(std::string line);
	void parseLocationDefaultFile(std::string line);
	void parseLocationUploadsDir(std::string line);
	void parseLocation(std::string line);
	void parseWhateverButNotLocation(std::string line);
	void checkIfInsideOfServer(std::string line);
	void addServerToServerRepo();
	void addLocation();

};


#endif