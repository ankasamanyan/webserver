#include "../includes/Configuration.hpp"

std::string getToken(std::string str, int n)
{
    std::istringstream line(str);
    std::string token;
    
    for (int i = 1; i <= n; ++i)
	{
        if (!(line >> token))
            return "";
    }
    return token;
}

Configuration::Configuration(){}

Configuration::Configuration(int argc, char **argv){
	if(argc == 1){
		parseConfiguration("default_config_file.conf");
		addServerToServerRepo();
	}
	else if (argc == 2)
    {
		parseConfiguration(argv[1]);
    }
	else if(argc > 2){
		std::cout << "Wrong number of arguments" << std::endl;
		exit(1);
	}
}

Configuration::~Configuration(){}

void Configuration::parseConfiguration(std::string configFile){    
    std::fstream file(configFile);
    std::string line = "default";
    if (file.fail())
        throw std::out_of_range("failed to open a file");
        while (std::getline (file,line) )
        {
			checkIfInsideOfServer(line);
			if( env == SERVER){
				while(getToken(line, 1) != "</server>"){
					getline(file, line);
					if(getToken(line, 1) == "<LOC>")
						env = LOC;
					 if(env == LOC){
						clearLocation();
						while(getToken(line, 1) != "</LOC>"){
							getline(file, line);
							parseLocation(line);
						}
						addLocation();
						env = SERVER;
					}
					parseWhateverButNotLocation(line);
				}
				addServerToServerRepo();
				env = OUT_OF_SERVER;
			}
        }
        file.close();
}

void Configuration::addServerToServerRepo(){
	serverRepo.push_back(config);
	config.locations.clear();
	clearConfiguration();
}

void Configuration::addLocation(){
	config.locations.insert(std::pair<std::string, location>(loc.locationName, loc));
}

void Configuration::parseWhateverButNotLocation(std::string line){
	parseServerName(line);	
	parseHost(line);
	parsePort(line);
	parseMaxBody(line);
	parseErroePage404(line);
	parseDirectoryListing(line);
	parseCGIdir(line);
}

void Configuration::parseServerName(std::string line){
	if(getToken(line, 1) == "serverName:")
		config.serverName = getToken(line, 2);
}

void Configuration::parseHost(std::string line){
	if(getToken(line, 1) == "host:")
		config.host = getToken(line, 2);
}

void Configuration::parsePort(std::string line){
	if(getToken(line, 1) == "port:")
		config.port = getToken(line, 2);
}

void Configuration::parseMaxBody(std::string line){
	if(getToken(line, 1) == "maxBody:")
		config.maxBody = getToken(line, 2);
}

void Configuration::parseErroePage404(std::string line){
	if(getToken(line, 1) == "errorPage404:")
		config.error404 = getToken(line, 2);
}

void Configuration::parseDirectoryListing(std::string line){
	if(getToken(line, 1) == "directoryListing:"){
		if(getToken(line, 2) == "yes")
			config.dirListing = true;
		else
			config.dirListing = false;
	}
}

void Configuration::parseCGIdir(std::string line){
	if(getToken(line, 1) == "CGIDIR:")
		config.CGIDir = getToken(line, 2);
}

void Configuration::parseLocation(std::string line){
	parseLocationName(line);
	parseLocationDir(line);
	parseLocationGet(line);
	parseLocationPost(line);
	parseLocationDelete(line);
	parseLocationDefaultFile(line);
}

void Configuration::parseLocationName(std::string line){
	if(getToken(line, 1) == "locationName:"){
		loc.locationName = getToken(line, 2);
	}
}

void Configuration::parseLocationDir(std::string line){
	if(getToken(line, 1) == "locationDir:"){
		loc.locationDir = getToken(line, 2);
	}
}

void Configuration::parseLocationDefaultFile(std::string line){
	if(getToken(line, 1) == "defaultFile:"){
		loc.defaultFile = getToken(line, 2);
	}
}

void Configuration::parseLocationGet(std::string line){
	if(getToken(line, 1) == "GET:"){
		if(getToken(line, 2) == "yes")
			loc.methodGet = true;
		else
			loc.methodGet = false;
	}
}

void Configuration::parseLocationPost(std::string line){
	if(getToken(line, 1) == "POST:"){
		if(getToken(line, 2) == "yes")
			loc.methodPost = true;
		else
			loc.methodPost = false;
	}
}

void Configuration::parseLocationDelete(std::string line){
	if(getToken(line, 1) == "DELETE:"){
		if(getToken(line, 2) == "yes")
			loc.methodDelete = true;
		else
			loc.methodDelete = false;
	}
}

void Configuration::checkIfInsideOfServer(std::string line){
	env = OUT_OF_SERVER;
	if( getToken(line, 1) == "<server>")
		env = SERVER;
}

void Configuration::clearConfiguration(){
	config.serverName = "";
	config.host = "";
	config.port = "";
	config.maxBody = "";
	config.error404 = "";
	config.dirListing = false;
	config.CGIDir = "";
}

void Configuration::clearLocation(){
	loc.locationName = "";
	loc.locationDir = "";
	loc.methodGet = false;
	loc.methodPost = false;
	loc.methodDelete = false;
	loc.defaultFile = "";
}
