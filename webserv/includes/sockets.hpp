#ifndef SOCKETS
	#define SOCKETS

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
#include <set>

using std::string;

struct parsingStruct{
	string	host;
	int		port;
	// string	rootDir;
};

class Server
{
	private:
		// fd_set				_fdSet;
		struct sockaddr_in		_serverAddress;
		int						_serverSocket;
		std::vector<pollfd>		_fdVector;
		// std::set<int>			_activeClients;
		// map<hostname, config> ;


		/* defines */
		typedef std::vector<pollfd>::iterator fdIter;
		enum requestState
		{
			VALID,
			DISCONNECTED
		};
		/* functions */
		void				disconnectClient(fdIter iter);
		requestState		recieveRequest(fdIter iter);
		int					highestFd(std::set<int> activeClients);


	public:
		/* Server(parsingStruct init) */
		Server(parsingStruct innit);
		~Server();
	/* functions */
		int		getSocket();
		void	serverLoop();

};




#endif
