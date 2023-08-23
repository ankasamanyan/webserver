#ifndef CONFIGURATIONFILE_HPP
#define CONFIGURATIONFILE_HPP

#include <stdlib.h>

#include <iostream>
#include <time.h>
#include <string.h>
#include <stdbool.h>

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
public:
	configuration config;
	Configuration();
	~Configuration();
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