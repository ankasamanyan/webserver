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
#define DEBUG 1
#define	PID_INITIALIZED -42
#define TIMEOUT 3
#define PAINFULLY_TRUE true

enum clientState
{
	DONE_,
	SHOULD_DISCONNECT_,
	INITIALIZED_,
	CGI_LOOPING_TIME,
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
        std::string                             _requestLine;
        std::string                             _requestedServerName; /* add from the request headers */
        std::string                             _method;
        std::string                             _path;
        std::string                             _directory;
        std::string                             _file;
        std::string                             _query;
        std::string                             _HTTPVersion;
        std::string                             _headersChunk;
        std::map<std::string, std::string>		_headers;
        size_t                                  _bytesWritten;
        size_t                                  _contentLength;
        bool                                    _shouldAppend;
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
		std::vector<char*>						_env;
		std::vector<char*>						_argv;


		void                					processRequest();
        void                					parseRequestIfNotEmpty();
        void                					parseRequestLine();
        void                                    extractRequestLine();
        void                                    splitRequestLine();
        void                                    definePartsOfURL();
        std::string         					getDirectory();
        std::string         					getFile();
        std::string         					getQuery();
        void                					parseHeaders();
        void                                    extractHeaders();
        void                					defineServerName();
        void                					assignPossibleErrorCodes();
        void                					checkPathIsAllowed();
        void                					assignErrorForInvalidPath();
        void                					checkMethodIsAllowed();
        void                					checkHTTPVersionIsValid();
        void                					checkContentIsOfAllowedSize();
        void                                    continueToProcessIfStillValid();
        void                					setDefaultFile();
        void                					defineRequestTarget();
        void                					redirectIfNeeded();
        bool                					isDirectory(std::string path);
        void                					assignContent();
        void                					assignCGIFlag();
        void                					prepareResponse();
        void                					handleGet();
        void                					handlePost();
        void                                    createFileIfAllowed();
        void                					handleDelete();
        bool                					isAllowedToDelete();
        void                					checkHasRights();
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
		void									createEnv(std::vector<std::string> &env);
		void									pushBackEnv(const char *varName, std::map<std::string, std::string>::iterator iter, std::vector<std::string> &env);
		void									startCgiThingy();
		void									envFromHeaders(std::vector<std::string> &env);
		void									envFromFirstLine(std::vector<std::string> &env);
		void									castTheVector(std::vector<std::string> &src, std::vector<char *> &dest);
		void									CgiParentHandler();
		void									checkExtention();
		size_t									fileSize(const std::string &filePath);

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