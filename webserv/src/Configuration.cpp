#include "../includes/Configuration.hpp"

Configuration &Configuration::operator=(const Configuration &copy)
{
	std::cout << "THE EQUAL SIGN OPERATOR GOT CALLED!" << std::endl; 
	(void)copy;
	return (*this);
}

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

void removeSlashBeforeString(std::string str){
	str.erase(0, 1);
}

void checkSlashBeforeString(std::string str){
	if(str[0] == '/')
		removeSlashBeforeString(str);
}

void addBackSlashInTheEnd(std::string path){
	path = path + "/";
}

void checkIsThereSlashInTheEnd(std::string path){
	if(path[path.length()] != '/')
		addBackSlashInTheEnd(path);	
}


Configuration::Configuration(){}

Configuration::Configuration(int argc, char **argv){
    temp = 0;
	if(argc == 1){
		parseConfiguration("default_config_file.conf");
//		addServerToServerRepo();
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

char print_bool(bool b)
{
    if (b == true)
    std::cout << "T";
    else
    std::cout << "F";
    return (' ');
}

void Configuration::print_loc(location &l)
{
    std::cout << "Print location" << std::endl;

    std::cout << "locDir: " << l.locationDir << std::endl;
    std::cout << "redir: " << l.redirection << std::endl;
    std::cout << "dirList: " << print_bool(l.dirListing) << ", Get, Post, Delete: " << print_bool(l.methodGet) << print_bool(l.methodPost) << print_bool(l.methodDelete) << std::endl;
    std::cout << "defFile: " << l.defaultFile << std::endl;
    std::cout << "uploaddir: " << l.uploadsDir << std::endl;
}


void Configuration::print_loc_map(std::map<std::string, location> &m)
{
    size_t i = 0;
    std::cout << "Print loc map size: " << m.size() << std::endl;
    for(std::map<std::string, location>::iterator it = m.begin(); it != m.end(); it++)
    {
        std::cout << "Loc nr: " << i++  << " name = " << it->first << std::endl;
        print_loc(it->second);
        std::cout << std::endl;
    }
}

void Configuration::print_err_map(std::map<std::string, std::string> &m)
{
    std::cout << "Print err map size: " << m.size() << std::endl;
    for(std::map<std::string, std::string>::iterator it = m.begin(); it != m.end(); it++)
    {
        std::cout << it->first << ": " << it->second << std::endl;
        std::cout << std::endl;
    }
}


void    Configuration::print_single_config(configuration &conf)
{
    std::cout << "new deque entry" << std::endl;
    std::cout << "Root: " << conf.root << std::endl;
    std::cout << "serverName: " << conf.serverName << std::endl;
    std::cout << "host: " << conf.host << std::endl;
    std::cout << "port: " << conf.port << std::endl;
    std::cout << "maxBody: " << conf.maxBody << std::endl;
    std::cout << "CGIDir: " << conf.CGIDir << std::endl;
    print_loc_map(conf.locations);
    print_err_map(conf.errorPages);
}


void Configuration::print_all_config()
{
    for(std::deque<configuration>::iterator it = serverRepo.begin(); it != serverRepo.end(); it++)
    {
        print_single_config(*it);
    }
}

Configuration::~Configuration(){}

void Configuration::parseConfiguration(std::string configFile){   	//const char * 
    std::fstream file(configFile.c_str());
    std::string line = "default";
    if (file.fail())
	{
        throw std::out_of_range("failed to open a file");
	}
        while (std::getline (file,line) )
        {
			checkIfInsideOfServer(line);
			if( env == SERVER){
				clearConfiguration();
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
							if(getToken(line, 1 )!= "</errorPages>"){
								std::string tempString = getToken(line, 2);
								removeSlashBeforeString(tempString);
								config.errorPages.insert(std::pair<std::string, std::string>(getToken(line, 1), tempString));
							}	
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
//	print_single_config(config);
	serverRepo.push_back(config);
//	print_single_config(serverRepo.back());
	config.locations.clear();
	config.errorPages.clear();
//	clearConfiguration();
}

void Configuration::addLocation(){
	config.locations.insert(std::make_pair(loc.locationDir, loc));
//	std::cout << "ADDED LOCATION: " << loc.locationDir << " " << loc.methodGet << loc.methodDelete << loc.methodPost << " dirListing: " << loc.dirListing << std::endl;
//	 euqal_sign_operator_for_loc(config.locations.at(loc.locationDir), loc);
//    print_loc(config.locations.at(loc.locationDir));
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
	checkIsThereSlashInTheEnd(config.root);
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
	checkIsThereSlashInTheEnd(config.CGIDir);
}

void Configuration::checkUploadsLocationForRedirections(){
	if(loc.locationDir == "/uploads/"){
		if(loc.redirection != "")
			std::cout << "Redirections are not allowed in Uploads location";
	}
}

void Configuration::parseLocation(std::string line){
	parseLocationDir(line);
	parseLocationRedirection(line);
	parseLocationDirectoryListing(line);
	parseLocationGet(line);
	parseLocationPost(line);
	parseLocationDelete(line);
	parseLocationDefaultFile(line);
	parseLocationUploadsDir(line);
	checkUploadsLocationForRedirections();
}

void Configuration::parseLocationDirectoryListing(std::string line){
	if(getToken(line, 1) == "dirListing:"){
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
	checkIsThereSlashInTheEnd(loc.redirection);
}

void Configuration::parseLocationDir(std::string line){
	if(getToken(line, 1) == "locationDir:"){
		loc.locationDir = getToken(line, 2);
	}
	checkIsThereSlashInTheEnd(loc.locationDir);
}

void Configuration::parseLocationDefaultFile(std::string line){
	if(getToken(line, 1) == "defaultFile:"){
		loc.defaultFile = getToken(line, 2);
	}
//	checkSlashBeforeString(loc.defaultFile);
}

void Configuration::parseLocationUploadsDir(std::string line){
	if(getToken(line, 1) == "uploadsDir:"){
		loc.uploadsDir = getToken(line, 2);
	}
	checkIsThereSlashInTheEnd(loc.uploadsDir);
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
//    clearLocation();
	config.root = "/html/";
	config.serverName = "";
	config.host = "0.0.0.0";
	config.port = "2000";
	config.maxBody = "80000";
	config.CGIDir = "/cgi-bin/";
    if(config.locations.find("/") != config.locations.end())
    {
        std::cout << "Added default /" << std::endl;
        loc.locationDir = "/";
    	config.locations.insert(std::make_pair(loc.locationDir, loc));
    }
    if(config.locations.find("/uploads") != config.locations.end())
    {
        std::cout << "Added default uploads" << std::endl;
	    loc.locationDir = "/uploads/";
	    config.locations.insert(std::make_pair(loc.locationDir, loc));
    }
    if(config.locations.find("/errorHtml/") != config.locations.end())
    {
        std::cout << "Added default errorHtml" << std::endl;
	    loc.locationDir = "/errorHtml/";
	    config.locations.insert(std::make_pair(loc.locationDir, loc));
    }
	
	// config.errorPages["404"] = "errorHtml/404.html";
}

void Configuration::clearLocation(){
	loc.redirection = "";
	loc.dirListing = true;
	loc.locationDir = "/";
	loc.methodGet = true;
	loc.methodPost = false;
	loc.methodDelete = false;
	loc.defaultFile = "";
}

