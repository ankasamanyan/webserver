#ifndef CLIENT_HPP
	#define CLIENT_HPP

#include "Utils.hpp"
#include "Server.hpp"

enum requestType
{
	UNDEFINED,
	GET,
	POST,
	DELETE,
	INVALID
};

class Client
{
	private:
		requestType		_reqType;
		pollfd			_pollFd;/* int	_fd; */
		// configStruct	_config;

	public:
		Client();
		~Client();

        std::string     request;
        std::string     method;
        std::string     path;
        std::string     HTTPVersion;

};

#endif