#ifndef CLIENT_HPP
	#define CLIENT_HPP

#include "Utils.hpp"
#include "Server.hpp"
#include <map>

enum requestType
{
	UNDEFINED,
	GET,
	POST,
	DELETE,
	INVALID
};

enum error_state
{
	POST_PATH_NOT_ALLOWED,
	NO_ERROR,
	ERROR_404 = 404,
};

class Client
{
	private:
		requestType		_reqType;
		pollfd			_pollFd;/* int	_fd; */
		// error_state		_err;

	public:
		Client();
		~Client();

        std::string                             request;
        std::string                             method;
        std::string                             path;
        std::string                             HTTPVersion;
        std::map<std::string, std::string>		headers;
        std::string                             body;
		/*  */
		int										exitStatus;
		// std::ifstream							inputFile;
};

#endif