#include "../includes/Configuration.hpp"

Configuration::Configuration(){}

Configuration::~Configuration(){}

configuration Configuration::getConfigurationFromConfigFile(){
	Configuration::config = getDefaultConfiguration();
	return Configuration::config;
}

void Configuration::reader(std::string configFile, configuration config){
    (void)config;
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

