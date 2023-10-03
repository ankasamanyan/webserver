#include "../includes/Server_handler.hpp"

void	Client::handleCGI()
{
	if (_cgiChildId == PID_INITIALIZED)
	{
		startCgiThingy();
		_clientState = DONE_;
		return ;
	}	
}

void	Client::CgiParentHandler()
{
	int status;

	if (waitpid(_cgiChildId, &status, WNOHANG) == 0) // kiddo isnt finished yet
	{
		if (_cgiChildTimer + TIMEOUT < time(NULL))	// check if alloted time for kiddo to play has been surpased
		{
			std::cerr << ON_RED << "Kiddo plays for too long!"  << RESET_LINE << std::endl;
			kill(_cgiChildId, SIGKILL);
			waitpid(_cgiChildId, &status, 0);
			_exitState = INTERNAL_SERVER_ERROR;
			_clientState = DONE_;
		}
	}
	else	// kiddo is done playing
	{
		if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0)
		{
			std::cerr << ON_PINK << "THE SCRIPT HAS FAILED OR HASN'T FINISHED :(" << RESET_LINE;
			_exitState = INTERNAL_SERVER_ERROR;
		}
		if (access(_cgiOutFile.c_str(), R_OK) != 0)
		{
			std::cerr << ON_PINK << "THE OUTPUT FILE IS NOT ACCESSABLE :(" << RESET_LINE;
			_exitState = INTERNAL_SERVER_ERROR;
		}
		sendResponse();
		if (_responseState == FULLY_SENT)
			remove(_cgiOutFile.c_str());
	}
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

void	Client::castTheVector(std::vector<std::string> &src, std::vector<char *> &dest)
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
	std::string		fd;

	Utils::ft_itoa(_clientFd,fd);
	_cgiOutFile = "." + getConfig().root + getConfig().CGIDir.substr(1) + ("cgi"+ fd +".html");
	_cgiChildId = fork();
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
		std::cerr << ON_PURPLE << strerror(errno) << RESET_LINE;
		exit(1);
	}
	else
		_cgiChildTimer = time(NULL);
}
