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

enum clientState
{
	INVALID_,
	VALID_,
	SHOULD_DISCONNECT_,
	PARTLY_READ_,

};

enum exitState
{
	POST_PATH_NOT_ALLOWED,
	NO_ERROR,
	ERROR_404 = 404,
	EXIT_OK = 200,
};

struct	parsingStruct;
class Client
{
	friend class Server;

	private:
		int										_clientFd;
		exitState								_exitState;
		requestType								_reqType;
		pollfd									_pollFd;/* int	_fd; */
        std::string                             _request;
        std::string                             _method;
        std::string                             _path;
        std::string                             _HTTPVersion;
        std::map<std::string, std::string>		_headers;
        std::string                             _body;
		clientState								_clientState;
		parsingStruct							&_configuration;

		bool                isRequestValid();
        bool                isRequestEmpty();
        void                parseRequestLine();
        void                parseHeaders();
        void                parseBody();
        bool                areAllPartsOfRequestValid();
        bool                isMethodAllowed();
        bool                isHTTPVersionValid();
        bool                isContentOfAllowedSize();

	public:
		// Client();
		Client(int clientFd, parsingStruct &config);
		const Client	&operator=(const Client &copy);
		Client(const Client &copy);
		~Client();
        // void         	setState(clientState state);
        clientState         getState();
		/*  */
		void	receiveRequest();

};

#endif