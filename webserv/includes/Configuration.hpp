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

Configuration::Configuration(){}

Configuration::~Configuration(){}

configuration Configuration::getConfigurationFromConfigFile(){
Configuration::config = getDefaultConfiguration();
return Configuration::config;
}

configuration Configuration::getDefaultConfiguration(){
	// Configuration::config.serverName = "defaultName";
	Configuration::config.host = "0.0.0.0";
	Configuration::config.port = "3332";
	Configuration::config.maxBody = "100000000000";
	Configuration::config.error404  = "/assets/Error_404";
	Configuration::config.root = "/webserv";
	Configuration::config.dirListing = "yes";
	Configuration::config.methodGet = true;
	Configuration::config.methodPost = true;
	Configuration::config.methodDelete = true;
	return Configuration::config;
}


#endif