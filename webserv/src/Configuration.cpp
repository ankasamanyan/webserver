#include "../includes/Configuration.hpp"

Configuration::Configuration(){}

Configuration::Configuration(int argc, char **argv){
	if(argc == 1){
		config = getDefaultConfiguration();
	}
	else if (argc == 2)
    {
		reader(argv[1], config);
    }
	else if(argc > 2){
		std::cout << "Wrong number of arguments" << std::endl;
		exit(1);
	}
}

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
            // std::cout << line << '\n';
        }
        file.close();
        
}

configuration Configuration::getDefaultConfiguration(){
	Configuration::config.serverName = "defaultName_default";
	Configuration::config.host = "0.0.0.0_default";
	Configuration::config.port = "3332_default";
	Configuration::config.maxBody = "100000000000_default";
	Configuration::config.error404  = "/assets/Error_404_default";
	Configuration::config.root = "/webserv_default";
	Configuration::config.dirListing = "yes_default";
	Configuration::config.methodGet = true;
	Configuration::config.methodPost = true;
	Configuration::config.methodDelete = true;
	host = "0.0.0.0_default";
	port = "3332_default";
	maxBody = "100000000000_default";
	error404  = "/assets/Error_404_default";
	root = "/webserv_default";
	dirListing = "yes_default";
	methodGet = true;
	methodPost = true;
	methodDelete = true;
	return Configuration::config;
}

