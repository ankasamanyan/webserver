#include "../includes/Configuration.hpp"

Configuration::Configuration(){}

Configuration::~Configuration(){}

configuration Configuration::getConfigurationFromConfigFile(){
	Configuration::config = getDefaultConfiguration();
	return Configuration::config;
}

configuration Configuration::getDefaultConfiguration(){
	Configuration::config.serverName = "defaultName";
	Configuration::config.host = "0.0.0.0";
	Configuration::config.port = "3332";
	Configuration::config.maxBody = "100000000000";
	Configuration::config.error404  = "/assets/Error_404";
	Configuration::config.root = "/webserv";
	Configuration::config.dirListing = "yes";
	Configuration::config.methodGet = true;
	Configuration::config.methodPost = true;
	Configuration::config.methodDelete = true;
	host = "0.0.0.0";
	port = "3332";
	maxBody = "100000000000";
	error404  = "/assets/Error_404";
	root = "/webserv";
	dirListing = "yes";
	methodGet = true;
	methodPost = true;
	methodDelete = true;
	return Configuration::config;
}
