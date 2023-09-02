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

#define CHUNK_SIZE 8000

using std::string;

struct parsingStruct{
	string	host;
	int		port;
	// string	rootDir;
};

class Client;

class Server
{
	private:
		struct sockaddr_in		_serverAddress;
		int						_serverSocket;
		std::vector<pollfd>		_fdVector;
		std::map<int, Client>   _clients;

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
