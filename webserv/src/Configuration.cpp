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

void addBackSlashInTheEnd(std::string path){
	path = path + "/";
}

void checkIsThereBackSlashInTheEnd(std::string path){
	if(path[path.length()] != '/'){
		addBackSlashInTheEnd(path);
	}
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

void Configuration::parseConfiguration(std::string configFile){   	//const char * 
    std::fstream file(configFile.c_str());
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
							if(getToken(line, 1) != "</LOC>")
								parseLocation(line);
						}
						addLocation();
						env = SERVER;
					}
					if(getToken(line, 1) == "<errorPages>")
						env = ERRORS;
					if(env == ERRORS){
						while(getToken(line,1) != "</errorPages>"){
							getline(file, line);
							if(getToken(line, 1 )!= "</errorPages>")
								config.errorPages.insert(std::pair<std::string, std::string>(getToken(line, 1), getToken(line, 2)));	
						}
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
	config.errorPages.clear();
	clearConfiguration();
}

void Configuration::addLocation(){
	config.locations.insert(std::pair<std::string, location>(loc.locationDir, loc));
}

void Configuration::parseWhateverButNotLocation(std::string line){
	parseServerName(line);	
	parseHost(line);
	parsePort(line);
	parseMaxBody(line);
	parseCGIdir(line);
	parseRoot(line);
}

void Configuration::parseRoot(std::string line){
	if(getToken(line, 1) == "root:")
		config.root = getToken(line, 2);
	checkIsThereBackSlashInTheEnd(config.root);
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

void Configuration::parseCGIdir(std::string line){
	if(getToken(line, 1) == "CGIDIR:")
		config.CGIDir = getToken(line, 2);
	checkIsThereBackSlashInTheEnd(config.CGIDir);
}

void Configuration::parseLocation(std::string line){
	parseLocationRedirection(line);
	parseLocationDir(line);
	parseLocationDirectoryListing(line);
	parseLocationGet(line);
	parseLocationPost(line);
	parseLocationDelete(line);
	parseLocationDefaultFile(line);
	parseLocationUploadsDir(line);
}

void Configuration::parseLocationDirectoryListing(std::string line){
	if(getToken(line, 1) == "directoryListing:"){
		if(getToken(line, 2) == "yes")
			loc.dirListing = true;
		else
			loc.dirListing = false;
	}
}

void Configuration::parseLocationRedirection(std::string line){
	if(getToken(line, 1) == "redirection:"){
		loc.redirection = getToken(line, 2);
	}
	checkIsThereBackSlashInTheEnd(loc.redirection);
}

void Configuration::parseLocationDir(std::string line){
	if(getToken(line, 1) == "locationDir:"){
		loc.locationDir = getToken(line, 2);
	}
	checkIsThereBackSlashInTheEnd(loc.locationDir);
}

void Configuration::parseLocationDefaultFile(std::string line){
	if(getToken(line, 1) == "defaultFile:"){
		loc.defaultFile = getToken(line, 2);
	}
}

void Configuration::parseLocationUploadsDir(std::string line){
	if(getToken(line, 1) == "uploadsDir:"){
		loc.uploadsDir = getToken(line, 2);
	}
	checkIsThereBackSlashInTheEnd(loc.uploadsDir);
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
	config.root = "";
	config.serverName = "";
	config.host = "";
	config.port = "";
	config.maxBody = "";
	config.CGIDir = "";
}

void Configuration::clearLocation(){
	loc.redirection = "";
	loc.dirListing = false;
	loc.locationDir = "";
	loc.methodGet = false;
	loc.methodPost = false;
	loc.methodDelete = false;
	loc.defaultFile = "";
	loc.uploadsDir = "";
}
