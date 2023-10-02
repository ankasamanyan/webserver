#include "../includes/Client.hpp"

Client::Client(int _serverSocket, std::map<std::string, configuration> &config):_configMap(config) 
{
	int newFd = accept(_serverSocket, (struct sockaddr *)NULL, NULL);
    configureSocket(newFd);
	_clientFd = newFd;
	_responseState = INITIALIZED;
	_clientState = INITIALIZED_;
	_responsePos = 0;
	_exitState = EXIT_OK;
	_errorPagePath = "errorHtml/";
	_requestedServerName = "default";
	_cgiChildId = PID_INITIALIZED;
	_shouldAppend = false;
	_bytesWritten = 0;
	_CGICase = false;
}

const Client	&Client::operator=(const Client &copy)
{
	_clientFd = copy._clientFd;
	_exitState = copy._exitState;
	_pollFd = copy._pollFd;
	_request = copy._request;
	_requestLine = copy._requestLine;
	_method = copy._method;
	_path = copy._path;
	_directory = copy._directory;
	_file = copy._file;
	_query = copy._query;
	_HTTPVersion = copy._HTTPVersion;
	_headersChunk = copy._headersChunk;
	_headers = copy._headers;
	_contentLength = copy._contentLength;
	_bytesWritten = copy._bytesWritten;
	_shouldAppend = copy._shouldAppend;
	_location = copy._location;
	_defaultFile = copy._defaultFile;
	_requestTarget = copy._requestTarget;
	_directoryListingCase = copy._directoryListingCase;
	_CGICase = copy._CGICase;
	_configMap = copy._configMap;
	_clientState = copy._clientState;
	_responsePos = copy._responsePos;
	_responseState = copy._responseState;
	_errorPagePath = copy._errorPagePath;
	_requestedServerName = copy._requestedServerName;
	_cgiChildId = copy._cgiChildId;
	_fileToPost = copy._fileToPost;
	_location = copy._location;
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