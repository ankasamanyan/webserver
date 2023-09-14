#ifndef CLIENT_HPP
	#define CLIENT_HPP

#include "Utils.hpp"
#include "Server.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <sys/stat.h>
#include <cstdio>
#include <dirent.h>

#define STANDARD_404 "./html/errorHtml/404.html"
#define SUCCESS_HTML "./html/Success.html"
#define DELETED_HTML "./html/Deleted.html"
#define STANDARD_HTML "./html/index.html"

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
	CONTENT_TOO_LARGE = 413,
	FORBIDDEN = 403,
	NO_CONTENT = 204,
	INTERNAL_SERVER_ERROR = 500
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
        std::string                             _requestTarget;
        bool                                    _directoryListingCase;
        bool                                    _CGICase;
		clientState								_clientState;
		parsingStruct							&_configuration;
		size_t									_responsePos;
		responseState							_responseState;
		std::string								_errorPagePath;
		FILE                                    *_fileToPost;


		bool                isRequestValid();
        bool                isRequestEmpty();
		void                parseRequest();
        void                parseRequestLine();
        void                parseHeaders();
        void                parseBody();
        bool                areAllPartsOfRequestValid();
        void                defineRequestTarget();
        bool                isDirectory(std::string path);
        void                assignContent();
        void                assignCGIFlag();
        bool                isMethodAllowed();
        bool                isHTTPVersionValid();
        bool                isContentOfAllowedSize();
        void                prepareResponse();
        void                handleGet();
        void                handlePost();
        void                handleDelete();
        bool                isAllowedToDelete();
        bool                isInsideUploads();
        bool                exists(std::string filePath);
        void                attemptToRemove(std::string filePath);
		void				configureSocket(int newSocket);
		void				checkHeaders(std::string &headres);
		void				sendHeaders();
		std::string		    getHttpMsg(int code);
		void				handleGetResponse();
		void				handleDELETEResponse();
		void				handlePOSTResponse();
		void				directoryListing();



	public:
		Client(int clientFd, parsingStruct &config);
		const Client	&operator=(const Client &copy);
		Client(const Client &copy);
		~Client();

        clientState         					getState();
		void									receiveRequest();
		void									sendResponse();

};

#endif