#ifndef CLIENT_HPP
	#define CLIENT_HPP

#include "Utils.hpp"
#include "Server.hpp"
#include <fstream>
#include <iostream>
#include <string>
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

enum responseState
{
	INITIALIZED,
	PARTIALLY_SENT,
	FULLY_SENT,
};

enum exitState
{
	POST_PATH_NOT_ALLOWED,
	NO_ERROR,
	EXIT_OK = 200,
	ERROR_404 = 404,
	METHOD_NOT_ALLOWED = 405,
	INVALID_HHTPV = 505,
	BAD_REQUEST = 400,
	CONTENT_TOO_LARGE = 413
};

struct	parsingStruct;
class Client
{
	friend class Server;

	private:
		int										_clientFd;
		exitState								_exitState;
		requestType								_reqType;
		pollfd									_pollFd;
        std::string                             _request;
        std::string                             _method;
        std::string                             _path;
        std::string                             _HTTPVersion;
        std::map<std::string, std::string>		_headers;
        std::string                             _body;
		clientState								_clientState;
		parsingStruct							&_configuration;
		size_t									_responsePos;
		responseState							_responseState;


		bool                isRequestValid();
        bool                isRequestEmpty();
        void                parseRequestLine();
        void                parseHeaders();
        void                parseBody();
        bool                areAllPartsOfRequestValid();
        bool                isMethodAllowed();
        bool                isHTTPVersionValid();
        bool                isContentOfAllowedSize();
		void				configureSocket(int newSocket);
		void				checkHeaders(std::string &headres);
		void				sendHeaders();

	public:
		Client(int clientFd, parsingStruct &config);
		const Client	&operator=(const Client &copy);
		Client(const Client &copy);
		~Client();
		/*  */
        clientState         getState();
		/*  */
		void				receiveRequest();
		void				sendResponse();

};

#endif