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

void	Client::createEnv(std::vector<std::string> &env)
{
	envFromFirstLine(env);
	envFromHeaders(env);
	castTheVector(env, _env);
}

void	Client::envFromHeaders(std::vector<std::string> &env)
{
	pushBackEnv("CONTENT_TYPE=", _headers.find("Content-type"), env);
	pushBackEnv("CONTENT_TYPE=", _headers.find("content-type"), env);
	pushBackEnv("CONTENT_LENGTH=", _headers.find("Content-Length"), env);
	pushBackEnv("HTTP_COOKIE=", _headers.find("cookie"), env);
	pushBackEnv("HTTP_USER_AGENT=", _headers.find("User-Agent"), env);
	pushBackEnv("SERVER_NAME=", _headers.find("Server-name"), env);
	pushBackEnv("PATH_INFO=", _headers.find("Server-name"), env);
}

void	Client::envFromFirstLine(std::vector<std::string> &env)
{
	env.push_back(std::string("SCRIPT_NAME=" + _path));
	env.push_back(std::string("REQUEST_METHOD=" + _method));
	env.push_back(std::string("SERVER_PORT=" + getConfig().port));
	env.push_back(std::string("PATH_INFO=" + _requestTarget));
	env.push_back(std::string("OUTPUT_FILE=" + _cgiOutFile));
	if (_method.compare("GET") == 0)
		env.push_back(std::string("QUERY_STRING=" + _query));
	else
		env.push_back(std::string("QUERY_STRING=" + _request));
}

void	Client::castTheVector(std::vector<std::string> &src, std::vector<char *> dest)
{
	for (size_t i = 0; i < src.size(); ++i)
		dest.push_back(const_cast<char*>(src[i].c_str()));
	dest.push_back(NULL);
}

void	Client::pushBackEnv(const char *varName, std::map<std::string, std::string>::iterator iter, std::vector<std::string> &env)
{
	if (iter != _headers.end())
		env.push_back(const_cast<char*>(std::string(varName + iter->second).c_str()));
}

void	Client::startCgiThingy()
{
	std::string		str;

	PRINT << YELLOW "\r\nREQUEST TARGET IN CGI: "<<_requestTarget << RESET <<"\n\n" << RESET_LINE;
	Utils::ft_itoa(_clientFd,str);
	_cgiOutFile = "." + getConfig().root + getConfig().CGIDir.substr(1) + ("cgi"+str);

	PRINT << ON_PURPLE << " CGI OUTFILE " << _cgiOutFile.c_str() << RESET << "\r\n" << RESET_LINE;  

	// int cgiFd = open(_cgiOutFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);
	
	// PRINT << ON_PINK << " CGI FD " << cgiFd << "\r\n" << RESET_LINE;  
    // if (cgiFd == -1) 
	// {
    //     perror("open");
    //     exit(EXIT_FAILURE);
    // }

	_cgiChildId = fork();
	PRINT << GREEN << " CGI FORK ID " << _cgiChildId << "\r\n" << RESET_LINE;  
	
	PRINT << BLUE << " INFILE DATA \n\n" << _query << RESET_LINE;

	if (_cgiChildId == 0)
	{
		std::vector<std::string>	argvVector;
		std::vector<std::string>	envVector;

		argvVector.push_back("/usr/bin/python3");
		argvVector.push_back(_requestTarget.substr(1));

		castTheVector(argvVector, _argv);

		createEnv(envVector);
		if (!_argv.empty())
			execve(_argv[0], _argv.data(), _env.data());

		std::cerr << "Child Process error: ";
		std::cerr << strerror(errno) << RESET_LINE;
		exit(1);
	}
	else
	{

		PRINT << BLUE << " PRINT FROM CGI PARENT\n\n" << RESET_LINE;
		_cgiChildTimer = time(NULL);
		// close(cgiFd);
	}
}
