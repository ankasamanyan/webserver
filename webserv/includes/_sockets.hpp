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

		int				_serverSocket;
		std::set<int>	_activeClients;
		// map<hostname, config> ;

	/* functions */
		void	disconnectClient(int fd);
		int		recieveRequest(int fd);
		int		highestFd(std::set<int> activeClients);


	public:
		/* Server(parsingStruct init) */
		Server(parsingStruct innit);
		~Server();
		/* functions */
		int		getSocket();
		void	serverLoop();

};




#endif
