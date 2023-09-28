#ifndef CLIENT_HPP
	#define CLIENT_HPP

#include "Utils.hpp"
#include "Server_handler.hpp"
#include "Configuration.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <sys/stat.h>
#include <cstdio>
#include <dirent.h>
#include <signal.h>

#define STANDARD_404 "./html/errorHtml/404.html"
#define SUCCESS_HTML "./html/Success.html"
#define DELETED_HTML "./html/Deleted.html"
#define STANDARD_HTML "./html/index.html"
#define DEBUG 0
#define	PID_INITIALIZED -42
#define TIMEOUT 3
#define PAINFULLY_TRUE true

enum clientState
{
	INVALID_,
	VALID_,
	SHOULD_DISCONNECT_,
	PARTLY_READ_,
	INITIALIZED_
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

struct  location;
struct	configuration;
class Client
{
	friend class Server_handler;

	private:
		int										_clientFd;
		exitState								_exitState;
		// requestType								_reqType;
		pollfd									_pollFd;
        std::string                             _request;
        std::string                             _requestedServerName; /* add from the request headers */
        std::string                             _method;
        std::string                             _path;
        std::string                             _directory;
        std::string                             _file;
        std::string                             _query;
        std::string                             _HTTPVersion;
        std::map<std::string, std::string>		_headers;
        std::string                             _body;
        location                                _location;
        std::string                             _defaultFile;
        std::string                             _requestTarget;
        bool                                    _directoryListingCase;
        bool                                    _CGICase;
		clientState								_clientState;
		std::map<std::string, configuration>	&_configMap;
		size_t									_responsePos;
		responseState							_responseState;
		std::string								_errorPagePath;
		FILE                                    *_fileToPost;
		size_t									_responseLength;

		std::string								_cgiOutFile;
		pid_t									_cgiChildId;
		time_t								 	_cgiChildTimer;


		bool                					isRequestValid();
        bool                					isRequestEmpty();
		void                					parseRequest();
        void                					parseRequestLine();
        std::string         					getDirectory();
        std::string         					getFile();
        std::string         					getQuery();
        void                					parseHeaders();
        void                					defineServerName();
        void                					parseBody();
        void                					updateDirectoryIfUploading();
        void                					setDefaultFile();
        bool                					areAllPartsOfRequestValid();
        void                					defineRequestTarget();
        void                					redirectIfNeeded();
        bool                					isDirectory(std::string path);
        void                					assignContent();
        void                					assignCGIFlag();
        bool                					isPathAllowed();
        void                					assignErrorForInvalidPath();
        bool                					isMethodAllowed();
        bool                					isHTTPVersionValid();
        bool                					isContentOfAllowedSize();
        void                					prepareResponse();
        void                					handleGet();
        void                					handlePost();
        void                					handleDelete();
        bool                					isAllowedToDelete();
        bool                					isInsideUploads();
        bool                					exists(std::string filePath);
        void                					attemptToRemove(std::string filePath);
		void									configureSocket(int newSocket);
		void									checkHeaders(std::stringstream &headres);
		void									sendHeaders();
		std::string		    					getHttpMsg(int code);
		void									handleGetResponse();
		void									handleDELETEResponse();
		void									handlePOSTResponse();
		void									directoryListing();
		void									configureResponseFile(std::stringstream &fileName);
		configuration							&getConfig();
		void									handleCGI();
		void									createEnv(std::vector<char*> &env);
		void									pushBackEnv(const char *varName, std::map<std::string, std::string>::iterator iter, std::vector<char *> &env);
		void									startCgiThingy();


	public:
		Client(int _serverSocket, std::map<std::string, configuration> &config);
		const Client	&operator=(const Client &copy);
		Client(const Client &copy);
		~Client();

        clientState         					getState();
		void									receiveRequest();
		void									sendResponse();

};

#endif