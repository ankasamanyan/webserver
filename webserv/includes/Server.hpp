#ifndef SERVER_HPP
	#define SERVER_HPP

#include <poll.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include "Utils.hpp"
#include <sys/select.h>
#include <vector>
#include <fcntl.h>
#include <set>
#include "Client.hpp"
#include <map>
#include <sstream>
#include <fstream>
#include <cstdlib>

#define CHUNK_SIZE 	8000
#define HTTP_V 		"HTTP/1.1"
#define EXIT_STATUS	200


using std::string;

struct parsingStruct {
	string	host;
	int		port;
    bool    methodGet;
    bool    methodPost;
    bool    methodDelete;
    string  maxBody;
};

class Client;

class Server
{
	private:
		struct sockaddr_in		_serverAddress;
		int						_serverSocket;
		std::vector<pollfd>		_fdVector;
		std::map<int, Client>   _clients;
        parsingStruct           _configuration;


		/* defines */
		enum requestState
		{
			VALID,
            PARTLY_READ,
            INVALID,
			DISCONNECTED
		};
		typedef std::vector<pollfd>::iterator fdIter;
		/* functions */
        void                acceptClient(fdIter iter);
		void				disconnectClient(fdIter iter);
		requestState		receiveRequest(fdIter iter);
		int					highestFd(std::set<int> activeClients);
        bool                isRequestValid(fdIter iter);
        bool                isRequestEmpty(fdIter iter);
        void                parseRequestLine(fdIter iter);
        void                parseHeaders(fdIter iter);
        void                parseBody(fdIter iter);
        bool                areAllPartsOfRequestValid(fdIter iter);
        bool                isMethodAllowed(fdIter iter);
        bool                isHTTPVersionValid(fdIter iter);
        bool                isContentOfAllowedSize(fdIter iter);
		/*	  */
		void				sendResponse(fdIter iter);



        public:
		/* Server(parsingStruct init) */
		Server(parsingStruct innit);
		~Server();
		/* functions */
		int		getSocket();
		void	serverLoop();
		void	configureSocket(int newSocket);


};




#endif
