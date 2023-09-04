#include "../includes/Client.hpp"

Client::Client(int clientFd, parsingStruct &config):_configuration(config) 
{
	_clientFd = clientFd;
	_exitState = EXIT_OK;
}

const Client	&Client::operator=(const Client &copy)
{
	_exitState = copy._exitState;
	_reqType = copy._reqType;
	_pollFd = copy._pollFd;
	_request = copy._request;
	_method = copy._method;
	_path = copy._path;
	_HTTPVersion = copy._HTTPVersion;
	_headers = copy._headers;
	_body = copy._body;
	return (*this);
}

Client::Client(const Client &copy):_configuration(copy._configuration)
{
	*this = copy;
}

Client::~Client()
{
}
