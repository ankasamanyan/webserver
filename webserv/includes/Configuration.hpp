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

typedef struct configuration{
    std::string serverName;
	std::string host;
	std::string port;
	std::string maxBody;
	std::string error404;
	std::string root;
	std::string CGIDir;
	std::string dir;
	std::string uploadDir;
	std::string HTTPversion;
	std::string dirListing;
	bool methodGet;
	bool methodPost;
	bool methodDelete;

}configuration;

class Configuration
{
private:

	configuration config;
	std::string serverName;
	std::string host;
	std::string port;
	std::string maxBody;
	std::string error404;
	std::string root;
	std::string CGIDir;
	std::string dir;
	std::string uploadDir;
	std::string HTTPversion;
	std::string dirListing;
	bool methodGet;
	bool methodPost;
	bool methodDelete;
public:	
	Configuration();
	~Configuration();
	void reader(std::string configFile, configuration config);
	configuration getConfigurationFromConfigFile();
	configuration getDefaultConfiguration();
	configuration getConfiguration();

	/* TO IMPLEMENT

			getServerName
			getHost
			getPort
			getMaxBody
			getError404
			getRoot
			getCGIDir
			getDir
			getPathToUploads
			getHttpVersion
			getDirListing
			getGetAvailability
			getPostAvailability
			getDeletAvailability
			
	*/
};




#endif