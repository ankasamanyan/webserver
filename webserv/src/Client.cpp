#include "../includes/Client.hpp"

Client::Client(int _serverSocket, std::map<std::string, configuration> &config):_configMap(config) 
{
	int newFd = accept(_serverSocket, (struct sockaddr *)NULL, NULL);
    configureSocket(newFd);
	_clientFd = newFd;
	_responseState = INITIALIZED;
	_responsePos = 0;
	_exitState = EXIT_OK;
	_errorPagePath = "/errorHtml/";
	_requestedServerName = "default";
}

const Client	&Client::operator=(const Client &copy)
{
	_clientFd = copy._clientFd;
	_exitState = copy._exitState;
	_reqType = copy._reqType;
	_pollFd = copy._pollFd;
	_request = copy._request;
	_method = copy._method;
	_path = copy._path;
	_HTTPVersion = copy._HTTPVersion;
	_headers = copy._headers;
	_body = copy._body;
	_requestTarget = copy._requestTarget;
	_clientState = copy._clientState;
	_responsePos = copy._responsePos;
	_responseState = copy._responseState;
	_errorPagePath = copy._errorPagePath;
	_requestedServerName = copy._requestedServerName;
	// _responseLength = copy._responseLength;
	return (*this);
}

Client::Client(const Client &copy):_configMap(copy._configMap)
{
	*this = copy;
}

Client::~Client()
{

}

configuration	&Client::getConfig()
{
		return(_configMap.at(_requestedServerName));
}

void	Client::configureSocket(int newSocket)
{
	int		flags;

	flags = fcntl(newSocket, F_GETFL, 0);
	if (flags == -1)
		Utils::printMsg("Error configure socket", PINK);
	if (fcntl(newSocket, F_SETFL, flags | O_NONBLOCK) == -1)
		Utils::printMsg("Failed to set socket to non-blocking in fcntl().",PURPLE);
}