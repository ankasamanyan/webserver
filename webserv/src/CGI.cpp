#include "../includes/Server_handler.hpp"

void	Client::handleCGI()
{
	PRINT << BLUE << "\r\n HandkeCGI called" << RESET_LINE;
	if (_cgiChildId == PID_INITIALIZED)
	{
		startCgiThingy();
		return ;
	}
	int status;

	if (waitpid(_cgiChildId, &status, WNOHANG) == 0)
	{
		if (_cgiChildTimer + TIMEOUT < time(NULL))
		{
			std::cerr << "Kiddo plays for too long!" << std::endl;
			kill(_cgiChildId, SIGKILL);
			waitpid(_cgiChildId, &status, 0);
			_exitState = INTERNAL_SERVER_ERROR;
		}
		else
			return ;
	}
	if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0)
	{
		_exitState = INTERNAL_SERVER_ERROR;
		PRINT << BLUE << "\r\n Setting 500 in first if clause" << RESET_LINE;
	}
	if (access(_cgiOutFile.c_str(), R_OK) != 0)
	{
		_exitState = INTERNAL_SERVER_ERROR;
		PRINT << BLUE << "\r\n Setting 500 in second if clause" << RESET_LINE;
	}
	
	sendResponse();
	if (_responseState == FULLY_SENT)
		remove(_cgiOutFile.c_str());
}

void	Client::createEnv(std::vector<char*> &env)
{
	env.push_back(const_cast<char*>(std::string("SCRIPT_NAME=" + _path).c_str()));
	env.push_back(const_cast<char*>(std::string("REQUEST_METHOD=" + _method).c_str()));
	env.push_back(const_cast<char*>(std::string("SERVER_PORT=" + getConfig().port).c_str()));
	env.push_back(const_cast<char*>(std::string("PATH_INFO=" + _requestTarget).c_str()));
	if (_method.compare("GET") == 0)
		env.push_back(const_cast<char*>(std::string("QUERY_STRING=" + _query).c_str()));
	else
		env.push_back(const_cast<char*>(std::string("QUERY_STRING=" + _body).c_str()));

	pushBackEnv("CONTENT_TYPE=", _headers.find("Content-type"), env);
	pushBackEnv("CONTENT_TYPE=", _headers.find("content-type"), env);
	pushBackEnv("CONTENT_LENGTH=", _headers.find("Content-Length"), env);
	pushBackEnv("HTTP_COOKIE=", _headers.find("cookie"), env);
	pushBackEnv("HTTP_USER_AGENT=", _headers.find("User-Agent"), env);
	pushBackEnv("SERVER_NAME=", _headers.find("Server-name"), env);
	pushBackEnv("PATH_INFO=", _headers.find("Server-name"), env);
	pushBackEnv("OUTPUT_FILE=", _headers.find("Server-name"), env);
}

void	Client::pushBackEnv(const char *varName, std::map<std::string, std::string>::iterator iter, std::vector<char *> &env)
{
	if (iter != _headers.end())
		env.push_back(const_cast<char*>(std::string(varName + iter->second).c_str()));
}

void	Client::startCgiThingy()
{
	std::string		str;

	PRINT << YELLOW "\r\nREQUEST TARGET IN CGI: "<<_requestTarget << "\r\n" << RESET_LINE;
	Utils::ft_itoa(_clientFd,str);
	_cgiOutFile = "." + getConfig().root + getConfig().CGIDir + str;

	PRINT << ON_PURPLE "\r\n CGI OUTFILE " << _cgiOutFile << RESET_LINE;  

	// int cgiFd = open(_cgiOutFile.c_str(), O_WRONLY | O_CREAT, O_TRUNC, 0777);
    // if (cgiFd == -1) 
	// {
    //     perror("open");
    //     exit(EXIT_FAILURE);
    // }

	_cgiChildId = fork();
	
	if (_cgiChildId == 0)
	{
		std::vector<char*>	env;
		std::vector<char*>	argv;

		// dup2(cgiFd, STDOUT_FILENO);
		argv.push_back((char *)_requestTarget.c_str());
		argv.push_back(NULL);
		createEnv(env);
		execve(str.c_str(), argv.data(), env.data());
		std::cerr << strerror(errno) << RESET_LINE;
		exit(1);
	}
	else
	{
		_cgiChildTimer = time(NULL);
		// close(cgiFd);
	}
}
